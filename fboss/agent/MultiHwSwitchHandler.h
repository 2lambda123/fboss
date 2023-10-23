// (c) Meta Platforms, Inc. and affiliates. Confidential and proprietary.

#pragma once

#include "fboss/agent/types.h"

#include <folly/futures/Future.h>
#include <memory>
#include "fboss/agent/AgentConfig.h"
#include "fboss/agent/HwSwitchCallback.h"
#include "fboss/agent/HwSwitchConnectionStatusTable.h"
#include "fboss/agent/HwSwitchHandler.h"
#include "fboss/agent/if/gen-cpp2/MultiSwitchCtrl.h"

namespace facebook::fboss {

class HwSwitchHandler;
class SwitchState;
class StateDelta;
class TxPacket;
class SwitchStats;
class HwSwitchFb303Stats;
struct HwSwitchStateUpdate;
class SwSwitch;

using HwSwitchHandlerInitFn = std::function<std::unique_ptr<HwSwitchHandler>(
    const SwitchID& switchId,
    const cfg::SwitchInfo& info,
    SwSwitch* sw)>;

class MultiHwSwitchHandler {
 public:
  MultiHwSwitchHandler(
      const std::map<int64_t, cfg::SwitchInfo>& switchInfoMap,
      HwSwitchHandlerInitFn hwSwitchHandlerInitFn,
      SwSwitch* sw,
      std::optional<cfg::SdkVersion> sdkVersion);

  ~MultiHwSwitchHandler();

  void start();

  void stop();

  bool isRunning() const {
    return !stopped_.load();
  }

  multiswitch::StateOperDelta getNextStateOperDelta(
      int64_t switchId,
      std::unique_ptr<multiswitch::StateOperDelta> prevOperResult,
      bool initialSync);

  void notifyHwSwitchGracefulExit(int64_t switchId);

  void notifyHwSwitchDisconnected(int64_t switchId);

  std::shared_ptr<SwitchState> stateChanged(
      const StateDelta& delta,
      bool transaction);

  void exitFatal();

  std::unique_ptr<TxPacket> allocatePacket(uint32_t size);

  bool sendPacketOutOfPortAsync(
      std::unique_ptr<TxPacket> pkt,
      PortID portID,
      std::optional<uint8_t> queue = std::nullopt) noexcept;

  bool sendPacketSwitchedSync(std::unique_ptr<TxPacket> pkt) noexcept;

  bool sendPacketSwitchedAsync(std::unique_ptr<TxPacket> pkt) noexcept;

  bool isValidStateUpdate(const StateDelta& delta);

  void unregisterCallbacks();

  void gracefulExit(state::WarmbootState& thriftSwitchState);

  bool getAndClearNeighborHit(RouterID vrf, folly::IPAddress& ip);

  folly::dynamic toFollyDynamic();

  std::optional<uint32_t> getHwLogicalPortId(PortID portID);

  bool transactionsSupported();

  bool transactionsSupported(std::optional<cfg::SdkVersion> sdkVersion);

  HwSwitchFb303Stats* getSwitchStats();

  folly::F14FastMap<std::string, HwPortStats> getPortStats();

  CpuPortStats getCpuPortStats();

  std::map<std::string, HwSysPortStats> getSysPortStats();

  void updateStats();

  std::map<PortID, phy::PhyInfo> updateAllPhyInfo();

  uint64_t getDeviceWatermarkBytes();

  void clearPortStats(const std::unique_ptr<std::vector<int32_t>>& ports);

  std::vector<phy::PrbsLaneStats> getPortAsicPrbsStats(int32_t portId);

  void clearPortAsicPrbsStats(int32_t portId);

  std::vector<prbs::PrbsPolynomial> getPortPrbsPolynomials(int32_t portId);

  prbs::InterfacePrbsState getPortPrbsState(PortID portId);

  void switchRunStateChanged(SwitchRunState newState);

  // platform access apis
  void onHwInitialized(HwSwitchCallback* callback);

  void onInitialConfigApplied(HwSwitchCallback* sw);

  void platformStop();

  std::map<PortID, FabricEndpoint> getFabricReachability();

  FabricReachabilityStats getFabricReachabilityStats();

  std::vector<PortID> getSwitchReachability(SwitchID switchId);

  std::string getDebugDump();

  void fetchL2Table(std::vector<L2EntryThrift>* l2Table);

  std::string listObjects(const std::vector<HwObjectType>& types, bool cached);

  bool needL2EntryForNeighbor(const cfg::SwitchConfig* config) const;

  // For test purpose
  std::map<SwitchID, HwSwitchHandler*> getHwSwitchHandlers();

  /*
   * blocks till atleast one HwSwitch is connected.
   * returns false if wait is cancelled
   */
  bool waitUntilHwSwitchConnected();

  std::map<int32_t, SwitchRunState> getHwSwitchRunStates();

 private:
  HwSwitchHandler* getHwSwitchHandler(SwitchID id);

  folly::Future<HwSwitchStateUpdateResult> stateChanged(
      SwitchID switchId,
      const HwSwitchStateUpdate& update);

  std::map<SwitchID, HwSwitchStateUpdateResult> stateChanged(
      std::map<SwitchID, const StateDelta&>& deltas,
      bool transaction);

  std::map<SwitchID, HwSwitchStateUpdateResult> getStateUpdateResult(
      std::vector<SwitchID>& switchIds,
      std::vector<folly::Future<HwSwitchStateUpdateResult>>& futures);

  std::shared_ptr<SwitchState> rollbackStateChange(
      std::map<SwitchID, HwSwitchStateUpdateResult>& updateResults,
      std::shared_ptr<SwitchState> desiredState,
      bool transaction);

  SwSwitch* sw_;
  std::map<SwitchID, std::unique_ptr<HwSwitchHandler>> hwSwitchSyncers_;
  std::atomic<bool> stopped_{true};
  HwSwitchConnectionStatusTable connectionStatusTable_;
  bool transactionsSupported_;
};

} // namespace facebook::fboss
