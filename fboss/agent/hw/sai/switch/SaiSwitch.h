/*
 *  Copyright (c) 2004-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
#pragma once

#include "fboss/agent/HwSwitch.h"
#include "fboss/agent/L2Entry.h"
#include "fboss/agent/hw/HwSwitchStats.h"
#include "fboss/agent/hw/gen-cpp2/hardware_stats_types.h"
#include "fboss/agent/hw/sai/api/SaiApiTable.h"
#include "fboss/agent/hw/sai/switch/SaiManagerTable.h"
#include "fboss/agent/hw/sai/switch/SaiRxPacket.h"
#include "fboss/agent/platforms/sai/SaiPlatform.h"
#include "folly/MacAddress.h"

#include <folly/io/async/EventBase.h>
#include "fboss/agent/hw/switch_asics/HwAsic.h"

#include <memory>
#include <mutex>
#include <thread>

DECLARE_int32(update_watermark_stats_interval_s);
DECLARE_bool(force_recreate_acl_tables);

namespace facebook::fboss {

struct ConcurrentIndices;
class SaiStore;

/*
 * This is equivalent to sai_fdb_event_notification_data_t. Copy only the
 * necessary FDB event attributes from sai_fdb_event_notification_data_t.
 */
struct FdbEventNotificationData {
  sai_fdb_event_t eventType;
  sai_fdb_entry_t fdbEntry;
  BridgePortSaiId bridgePortSaiId;
  sai_uint32_t fdbMetaData;
  explicit FdbEventNotificationData(
      sai_fdb_event_t eventType,
      sai_fdb_entry_t fdbEntry,
      BridgePortSaiId bridgePortSaiId,
      sai_uint32_t fdbMetaData)
      : eventType(eventType),
        fdbEntry(fdbEntry),
        bridgePortSaiId(bridgePortSaiId),
        fdbMetaData(fdbMetaData) {}
};

class SaiSwitch : public HwSwitch {
 public:
  explicit SaiSwitch(
      SaiPlatform* platform,
      uint32_t featuresDesired =
          (FeaturesDesired::PACKET_RX_DESIRED |
           FeaturesDesired::LINKSCAN_DESIRED |
           FeaturesDesired::TAM_EVENT_NOTIFY_DESIRED));

  ~SaiSwitch() override;

  HwInitResult init(Callback* callback, bool failHwCallsOnWarmboot) noexcept
      override;

  void unregisterCallbacks() noexcept override;
  /*
   * SaiSwitch requires L2/FDB entries to be created for resolved
   * neighbors to be able to compute egress ports for nhops. A join
   * b/w the FDB (mac, port), neighbor (ip, mac) is done to figure
   * out the nhop ip-> mac, egress port information
   */
  bool needL2EntryForNeighbor() const override {
    return true;
  }

  std::shared_ptr<SwitchState> stateChanged(const StateDelta& delta) override;

  bool isValidStateUpdate(const StateDelta& delta) const override;

  std::unique_ptr<TxPacket> allocatePacket(uint32_t size) const override;

  bool sendPacketSwitchedAsync(std::unique_ptr<TxPacket> pkt) noexcept override;

  bool sendPacketOutOfPortAsync(
      std::unique_ptr<TxPacket> pkt,
      PortID portID,
      std::optional<uint8_t> queueId) noexcept override;

  bool sendPacketSwitchedSync(std::unique_ptr<TxPacket> pkt) noexcept override;

  bool sendPacketOutOfPortSync(
      std::unique_ptr<TxPacket> pkt,
      PortID portID,
      std::optional<uint8_t> queueId) noexcept override;

  folly::F14FastMap<std::string, HwPortStats> getPortStats() const override;

  uint64_t getDeviceWatermarkBytes() const override;

  void fetchL2Table(std::vector<L2EntryThrift>* l2Table) const override;

  folly::dynamic toFollyDynamic() const override;

  void exitFatal() const override;

  bool isPortUp(PortID port) const override;

  bool getAndClearNeighborHit(RouterID vrf, folly::IPAddress& ip) override;

  void clearPortStats(
      const std::unique_ptr<std::vector<int32_t>>& ports) override;

  cfg::PortSpeed getPortMaxSpeed(PortID port) const override;

  void linkStateChangedCallbackTopHalf(
      uint32_t count,
      const sai_port_oper_status_notification_t* data);
  void fdbEventCallback(
      uint32_t count,
      const sai_fdb_event_notification_data_t* data);

  void parityErrorSwitchEventCallback(
      sai_size_t buffer_size,
      const void* buffer,
      uint32_t event_type);

  void tamEventCallback(
      sai_object_id_t tam_event_id,
      sai_size_t buffer_size,
      const void* buffer,
      uint32_t attr_count,
      const sai_attribute_t* attr_list);

  BootType getBootType() const override;

  const SaiManagerTable* managerTable() const;

  SaiManagerTable* managerTable();

  /*
   * This method is not thread safe, it should only be used
   * from the SAI adapter's rx callback caller thread.
   */
  void packetRxCallback(
      SwitchSaiId switch_id,
      sai_size_t buffer_size,
      const void* buffer,
      uint32_t attr_count,
      const sai_attribute_t* attr_list);

  SwitchSaiId getSwitchId() const {
    return switchId_;
  }
  SaiPlatform* getPlatform() const override {
    return platform_;
  }

  SaiStore* getSaiStore() const {
    return saiStore_.get();
  }

  const ConcurrentIndices& concurrentIndices() const {
    return *concurrentIndices_;
  }
  SwitchRunState getSwitchRunState() const;
  bool isFullyInitialized() const;

  std::string listObjects(const std::vector<HwObjectType>& types, bool cached)
      const override;
  void dumpDebugState(const std::string& /*path*/) const override;

  std::shared_ptr<SwitchState> stateChangedTransaction(
      const StateDelta& delta) override;

  bool transactionsSupported() const override;
  bool l2LearningModeChangeProhibited() const;

  virtual std::map<PortID, phy::PhyInfo> updateAllPhyInfo() const override;

  uint32_t generateDeterministicSeed(
      LoadBalancerID loadBalancerID,
      folly::MacAddress mac) const override;

 private:
  void gracefulExitImpl(folly::dynamic& switchState) override;

  template <typename LockPolicyT>
  std::shared_ptr<SwitchState> stateChangedImpl(
      const StateDelta& delta,
      const LockPolicyT& lk);
  friend class SaiRollbackTest;
  void rollback(const std::shared_ptr<SwitchState>& knownGoodState) noexcept;
  std::string listObjectsLocked(
      const std::vector<sai_object_type_t>& objects,
      bool cached,
      const std::lock_guard<std::mutex>& lock) const;
  void listManagedObjectsLocked(
      std::string& output,
      const std::lock_guard<std::mutex>& lock) const;
  void switchRunStateChangedImpl(SwitchRunState newState) override;

  void updateStatsImpl(SwitchStats* switchStats) override;
  template <typename LockPolicyT>
  void updateResourceUsage(const LockPolicyT& lockPolicy);
  /*
   * To make SaiSwitch thread-safe, we mirror the public interface with
   * private functions with the same name followed by Locked.
   *
   * The private methods take an additional argument -- a const ref to
   * lock_guard -- which ensures that a lock is held during the call.
   * While it could be any lock, there is only saiSwitchMutex_ in this context.
   *
   * The public methods take saiSwitchMutex_ with an std::lock_guard and then
   * call the Locked version passing the const lock_guard ref. The Locked
   * versions don't take any additional locks.
   *
   * Within SaiSwitch, no method should call methods from the public interface
   * to avoid a deadlock trying to lock saiSwitchMutex_ twice.
   *
   * N.B., packet rx is handled slightly differently, which is documented
   * along with its methods.
   */
  HwInitResult initLocked(
      const std::lock_guard<std::mutex>& lk,
      HwWriteBehavior behavior,
      Callback* callback) noexcept;

  void initStoreAndManagersLocked(
      const std::lock_guard<std::mutex>& lk,
      HwWriteBehavior behavior,
      const folly::dynamic* adapterKeys,
      const folly::dynamic* adapterKeys2AdapterHostKeys);

  void unregisterCallbacksLocked(
      const std::lock_guard<std::mutex>& lock) noexcept;

  bool isValidStateUpdateLocked(
      const std::lock_guard<std::mutex>& lock,
      const StateDelta& delta) const;

  void fetchL2TableLocked(
      const std::lock_guard<std::mutex>& lock,
      std::vector<L2EntryThrift>* l2Table) const;

  void gracefulExitLocked(
      const std::lock_guard<std::mutex>& lock,
      folly::dynamic& switchState);

  folly::dynamic toFollyDynamicLocked(
      const std::lock_guard<std::mutex>& lock) const;

  void switchRunStateChangedImplLocked(
      const std::lock_guard<std::mutex>& lock,
      SwitchRunState newState);

  void exitFatalLocked(const std::lock_guard<std::mutex>& lock) const;

  bool isPortUpLocked(const std::lock_guard<std::mutex>& lock, PortID port)
      const;

  cfg::PortSpeed getPortMaxSpeedLocked(
      const std::lock_guard<std::mutex>& lock,
      PortID port) const;

  void fdbEventCallbackLockedBottomHalf(
      const std::lock_guard<std::mutex>& lock,
      std::vector<FdbEventNotificationData> data);

  const SaiManagerTable* managerTableLocked(
      const std::lock_guard<std::mutex>& lock) const;
  SaiManagerTable* managerTableLocked(const std::lock_guard<std::mutex>& lock);

  void gracefulExitLocked(
      folly::dynamic& switchState,
      const std::lock_guard<std::mutex>& lock);
  void initLinkScanLocked(const std::lock_guard<std::mutex>& lock);
  void initRxLocked(const std::lock_guard<std::mutex>& lock);

  bool isFeatureSetupLocked(
      FeaturesDesired feature,
      const std::lock_guard<std::mutex>& lock) const;

  folly::F14FastMap<std::string, HwPortStats> getPortStatsLocked(
      const std::lock_guard<std::mutex>& lock) const;

  std::map<PortID, phy::PhyInfo> updateAllPhyInfoLocked() const;

  void linkStateChangedCallbackBottomHalf(
      std::vector<sai_port_oper_status_notification_t> data);

  uint64_t getDeviceWatermarkBytesLocked(
      const std::lock_guard<std::mutex>& lock) const;

  void processSwitchSettingsChangedLocked(
      const std::lock_guard<std::mutex>& lock,
      const StateDelta& delta);

  template <typename ManagerT, typename LockPolicyT>
  void processDefaultDataPlanePolicyDelta(
      const StateDelta& delta,
      ManagerT& mgr,
      const LockPolicyT& lk);

  template <typename LockPolicyT>
  void processLinkStateChangeDelta(
      const StateDelta& delta,
      const LockPolicyT& lockPolicy);

  template <
      typename Delta,
      typename Manager,
      typename LockPolicyT,
      typename... Args,
      typename ChangeFunc = void (Manager::*)(
          const std::shared_ptr<typename Delta::Node>&,
          const std::shared_ptr<typename Delta::Node>&,
          Args...),
      typename AddedFunc = void (
          Manager::*)(const std::shared_ptr<typename Delta::Node>&, Args...),
      typename RemovedFunc = void (
          Manager::*)(const std::shared_ptr<typename Delta::Node>&, Args...)>
  void processDelta(
      Delta delta,
      Manager& manager,
      const LockPolicyT& lockPolicy,
      ChangeFunc changedFunc,
      AddedFunc addedFunc,
      RemovedFunc removedFunc,
      Args... args);

  template <
      typename Delta,
      typename Manager,
      typename LockPolicyT,
      typename... Args,
      typename ChangeFunc = void (Manager::*)(
          const std::shared_ptr<typename Delta::Node>&,
          const std::shared_ptr<typename Delta::Node>&,
          Args...)>
  void processChangedDelta(
      Delta delta,
      Manager& manager,
      const LockPolicyT& lockPolicy,
      ChangeFunc changedFunc,
      Args... args);

  template <
      typename Delta,
      typename Manager,
      typename LockPolicyT,
      typename... Args,
      typename AddedFunc = void (
          Manager::*)(const std::shared_ptr<typename Delta::Node>&, Args...)>
  void processAddedDelta(
      Delta delta,
      Manager& manager,
      const LockPolicyT& lockPolicy,
      AddedFunc addedFunc,
      Args... args);

  template <
      typename Delta,
      typename Manager,
      typename LockPolicyT,
      typename... Args,
      typename RemovedFunc = void (
          Manager::*)(const std::shared_ptr<typename Delta::Node>&, Args...)>
  void processRemovedDelta(
      Delta delta,
      Manager& manager,
      const LockPolicyT& lockPolicy,
      RemovedFunc removedFunc,
      Args... args);

  template <typename LockPolicyT>
  void processSwitchSettingsChanged(
      const StateDelta& delta,
      const LockPolicyT& lockPolicy);

  PortSaiId getCPUPortSaiId() const;

  void packetRxCallbackPort(
      sai_size_t buffer_size,
      const void* buffer,
      PortSaiId inPort,
      bool allowMissingSrcPort,
      cfg::PacketRxReason rxReason);

  void packetRxCallbackLag(
      sai_size_t buffer_size,
      const void* buffer,
      LagSaiId inAggPort,
      PortSaiId inPort,
      bool allowMissingSrcPort,
      cfg::PacketRxReason rxReason);

  std::shared_ptr<SwitchState> getColdBootSwitchState();

  std::optional<L2Entry> getL2Entry(
      const FdbEventNotificationData& fdbEvent) const;

  bool isMissingSrcPortAllowed(HostifTrapSaiId hostifTrapSaiId);

  /*
   * SaiSwitch must support a few varieties of concurrent access:
   * 1. state updates on the SwSwitch update thread calling stateChanged
   * 2. packet rx callback
   * 3. async tx thread
   * 4. port state event callback (i.e., linkscan)
   * 5. stats collection
   * 6. getters exposed to thrift or other threads
   *
   * It is critical that 2, 3, and 4 are not blocked by other, possibly slower
   * operations. Ideally, 1 and 5 are able to make progress relatively freely
   * as well. To that end, we synchronize most access (1, 6) with a global
   * lock, but give a fast-path for 2, 3, 4, 5 in the form of possibly out
   * of date indices stored in folly::ConcurrentHashMaps in ConcurrentIndices
   * e.g., rx can look up the PortID from the sai_object_id_t on the
   * packet without blocking normal hardware programming.
   *
   * By using the concurent hashmap, Rx and Tx path is lock free. Running Tx/Rx
   * in a separate eventbase thread severely affects the slow path performance.
   * Handling Rx in single thread improved the performance to be on-par with
   * native bcm. Handling Tx without eventbase thread improved the
   * performance by 2000 pps.
   */
  mutable std::mutex saiSwitchMutex_;
  std::unique_ptr<ConcurrentIndices> concurrentIndices_;

  SaiPlatform* platform_;
  // Instead of using singleton for SaiStore, we assign one SaiStore to one
  // SaiSwitch to support multiple SaiSwitch in one single service.
  std::unique_ptr<SaiStore> saiStore_;
  std::unique_ptr<SaiManagerTable> managerTable_;
  std::atomic<BootType> bootType_{BootType::UNINITIALIZED};
  Callback* callback_{nullptr};

  SwitchSaiId switchId_;

  std::unique_ptr<std::thread> linkStateBottomHalfThread_;
  folly::EventBase linkStateBottomHalfEventBase_;
  std::unique_ptr<std::thread> fdbEventBottomHalfThread_;
  folly::EventBase fdbEventBottomHalfEventBase_;

  HwResourceStats hwResourceStats_;
  std::atomic<SwitchRunState> runState_{SwitchRunState::UNINITIALIZED};

  int64_t watermarkStatsUpdateTime_{0};
  HwAsic::AsicType asicType_;
};

} // namespace facebook::fboss
