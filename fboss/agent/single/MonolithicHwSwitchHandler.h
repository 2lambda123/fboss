// (c) Meta Platforms, Inc. and affiliates. Confidential and proprietary.

#pragma once

#include "fboss/agent/HwSwitchHandler.h"

namespace facebook::fboss {

class HwSwitch;
class Platform;
class TxPacket;

class MonolinithicHwSwitchHandler : public HwSwitchHandler {
 public:
  using PlatformInitFn = std::function<std::unique_ptr<Platform>(
      std::unique_ptr<AgentConfig>,
      uint32_t featuresDesired)>;

  explicit MonolinithicHwSwitchHandler(PlatformInitFn initPlatformFn);

  virtual ~MonolinithicHwSwitchHandler() override {}

  void initPlatform(std::unique_ptr<AgentConfig> config, uint32_t features)
      override;

  HwInitResult initHw(HwSwitchCallback* callback, bool failHwCallsOnWarmboot)
      override;

  void exitFatal() const override;

  std::unique_ptr<TxPacket> allocatePacket(uint32_t size) const override;

  bool sendPacketOutOfPortAsync(
      std::unique_ptr<TxPacket> pkt,
      PortID portID,
      std::optional<uint8_t> queue = std::nullopt) noexcept override;

  bool sendPacketSwitchedSync(std::unique_ptr<TxPacket> pkt) noexcept override;

  bool isValidStateUpdate(const StateDelta& delta) const override;

  void unregisterCallbacks() override;

  void gracefulExit(
      folly::dynamic& follySwitchState,
      state::WarmbootState& thriftSwitchState) override;

  bool getAndClearNeighborHit(RouterID vrf, folly::IPAddress& ip) override;

  folly::dynamic toFollyDynamic() const override;

  std::optional<uint32_t> getHwLogicalPortId(PortID portID) const override;

  void initPlatformData() override;

  void clearPortStats(
      const std::unique_ptr<std::vector<int32_t>>& ports) override;

  std::vector<phy::PrbsLaneStats> getPortAsicPrbsStats(int32_t portId) override;

  void clearPortAsicPrbsStats(int32_t portId) override;

  std::vector<prbs::PrbsPolynomial> getPortPrbsPolynomials(
      int32_t portId) override;

  prbs::InterfacePrbsState getPortPrbsState(PortID portId) override;

  std::vector<phy::PrbsLaneStats> getPortGearboxPrbsStats(
      int32_t portId,
      phy::Side side) override;

  void clearPortGearboxPrbsStats(int32_t portId, phy::Side side) override;

  void switchRunStateChanged(SwitchRunState newState) override;

  // platform access apis
  void onHwInitialized(HwSwitchCallback* callback) override;

  void onInitialConfigApplied(HwSwitchCallback* sw) override;

  void platformStop() override;

  const AgentConfig* config() override;

  const AgentConfig* reloadConfig() override;

  std::shared_ptr<SwitchState> stateChanged(
      const StateDelta& delta,
      bool transaction) override;

 private:
  PlatformInitFn initPlatformFn_;
  HwSwitch* hw_;
  std::unique_ptr<Platform> platform_;
};

} // namespace facebook::fboss
