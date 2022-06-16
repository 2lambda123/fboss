// (c) Facebook, Inc. and its affiliates. Confidential and proprietary.

#include "fboss/agent/test/agent_hw_test/ProdInvariantTests.h"
#include <folly/gen/Base.h>
#include <chrono>
#include <thread>
#include "fboss/agent/AgentConfig.h"
#include "fboss/agent/SwSwitch.h"
#include "fboss/agent/gen-cpp2/validated_shell_commands_constants.h"
#include "fboss/agent/hw/test/ConfigFactory.h"
#include "fboss/agent/hw/test/HwTestCoppUtils.h"
#include "fboss/agent/hw/test/HwTestPacketUtils.h"
#include "fboss/agent/hw/test/HwTestProdConfigUtils.h"
#include "fboss/agent/hw/test/LoadBalancerUtils.h"
#include "fboss/agent/hw/test/ProdConfigFactory.h"
#include "fboss/agent/hw/test/dataplane_tests/HwTestDscpMarkingUtils.h"
#include "fboss/agent/hw/test/dataplane_tests/HwTestQosUtils.h"
#include "fboss/agent/hw/test/dataplane_tests/HwTestQueuePerHostUtils.h"
#include "fboss/agent/hw/test/dataplane_tests/HwTestUtils.h"
#include "fboss/agent/state/Port.h"
#include "fboss/agent/state/SwitchState.h"
#include "fboss/agent/test/EcmpSetupHelper.h"
#include "fboss/lib/config/PlatformConfigUtils.h"

namespace {
using facebook::fboss::PortDescriptor;
using facebook::fboss::PortID;
auto constexpr kEcmpWidth = 4;
} // namespace

namespace facebook::fboss {

void ProdInvariantTest::setupAgentTestEcmp(
    const std::vector<PortDescriptor>& ecmpPorts) {
  ASSERT_GT(ecmpPorts.size(), 0);

  boost::container::flat_set<PortDescriptor> ports;
  std::for_each(ecmpPorts.begin(), ecmpPorts.end(), [&ports](auto ecmpPort) {
    ports.insert(ecmpPort);
  });

  sw()->updateStateBlocking("Resolve nhops", [&](auto state) {
    utility::EcmpSetupTargetedPorts6 ecmp6(state);
    return ecmp6.resolveNextHops(state, ports);
  });

  utility::EcmpSetupTargetedPorts6 ecmp6(sw()->getState());
  ecmp6.programRoutes(
      std::make_unique<SwSwitchRouteUpdateWrapper>(sw()->getRouteUpdater()),
      ports);
}

void ProdInvariantTest::SetUp() {
  AgentTest::SetUp();
  auto ecmpUplinlinkPorts =
      utility::getAllUplinkDownlinkPorts(
          platform()->getHwSwitch(), initialConfig(), kEcmpWidth, false)
          .first;
  for (auto& uplinkPort : ecmpUplinlinkPorts) {
    ecmpPorts_.push_back(PortDescriptor(uplinkPort));
  }
  setupAgentTestEcmp(ecmpPorts_);
  XLOG(INFO) << "ProdInvariantTest setup done";
}

std::vector<PortID> getAllPlatformPorts(
    const std::map<int32_t, cfg::PlatformPortEntry>& platformPorts) {
  std::vector<PortID> ports;
  ports.reserve(0);
  auto subsidiaryPortMap = utility::getSubsidiaryPortIDs(platformPorts);
  for (auto& port : subsidiaryPortMap) {
    ports.emplace_back(port.first);
  }
  return ports;
}
cfg::SwitchConfig ProdInvariantTest::initialConfig() {
  cfg::SwitchConfig cfg;
  std::vector<PortID> ports;
  ports.reserve(0);

  ports = getAllPlatformPorts(platform()->getPlatformPorts());
  cfg = utility::createProdRswConfig(platform()->getHwSwitch(), ports);
  return cfg;
}

void ProdInvariantTest::setupConfigFlag() {
  cfg::AgentConfig testConfig;
  utility::setPortToDefaultProfileIDMap(
      std::make_shared<PortMap>(), platform());
  testConfig.sw() = initialConfig();
  const auto& baseConfig = platform()->config();
  testConfig.platform() = *baseConfig->thrift.platform();
  auto newcfg = AgentConfig(
      testConfig,
      apache::thrift::SimpleJSONSerializer::serialize<std::string>(testConfig));
  auto newCfgFile = getTestConfigPath();
  newcfg.dumpConfig(newCfgFile);
  FLAGS_config = newCfgFile;
  // reload config so that test config is loaded
  platform()->reloadConfig();
}

void ProdInvariantTest::sendTraffic() {
  auto mac = utility::getInterfaceMac(
      sw()->getState(), (*sw()->getState()->getVlans()->begin())->getID());
  utility::pumpTraffic(
      true,
      sw()->getHw(),
      mac,
      (*sw()->getState()->getVlans()->begin())->getID(),
      getDownlinkPort());
}

PortID ProdInvariantTest::getDownlinkPort() {
  // pick the first downlink in the list
  auto downlinkPort = utility::getAllUplinkDownlinkPorts(
                          sw()->getHw(), initialConfig(), kEcmpWidth, false)
                          .second[0];
  return downlinkPort;
}

std::map<PortID, HwPortStats> ProdInvariantTest::getLatestPortStats(
    const std::vector<PortID>& ports) {
  std::map<PortID, HwPortStats> portIdStatsMap;
  auto portNameStatsMap = sw()->getHw()->getPortStats();
  for (auto [portName, stats] : portNameStatsMap) {
    auto portId = sw()->getState()->getPorts()->getPort(portName)->getID();
    if (std::find(ports.begin(), ports.end(), (PortID)portId) == ports.end()) {
      continue;
    }
    portIdStatsMap.emplace((PortID)portId, stats);
  }
  return portIdStatsMap;
}

std::vector<PortID> ProdInvariantTest::getEcmpPortIds() {
  std::vector<PortID> ecmpPortIds{};
  for (auto portDesc : ecmpPorts_) {
    EXPECT_TRUE(portDesc.isPhysicalPort());
    auto portId = portDesc.phyPortID();
    ecmpPortIds.emplace_back(portId);
  }
  return ecmpPortIds;
}

void ProdInvariantTest::verifyLoadBalancing() {
  auto getPortStatsFn =
      [&](const std::vector<PortID>& portIds) -> std::map<PortID, HwPortStats> {
    return getLatestPortStats(portIds);
  };
  bool loadBalanced = utility::pumpTrafficAndVerifyLoadBalanced(
      [=]() { sendTraffic(); },
      [=]() {
        auto ports = std::make_unique<std::vector<int32_t>>();
        auto ecmpPortIds = getEcmpPortIds();
        for (auto ecmpPortId : ecmpPortIds) {
          ports->push_back(static_cast<int32_t>(ecmpPortId));
        }
        sw()->getHw()->clearPortStats(ports);
      },
      [=]() {
        return utility::isLoadBalanced(
            ecmpPorts_,
            std::vector<NextHopWeight>(kEcmpWidth, 1),
            getPortStatsFn,
            25);
      });
  EXPECT_TRUE(loadBalanced);
  XLOG(INFO) << "Verify loadbalancing done";
}

void ProdInvariantTest::verifyCopp() {
  utility::verifyCoppInvariantHelper(
      sw()->getHw(),
      sw()->getPlatform()->getAsic(),
      sw()->getState(),
      getDownlinkPort());
  XLOG(INFO) << "Verify COPP done";
}

int ProdInvariantTestMain(
    int argc,
    char** argv,
    PlatformInitFn initPlatformFn) {
  ::testing::InitGoogleTest(&argc, argv);
  initAgentTest(argc, argv, initPlatformFn);
  return RUN_ALL_TESTS();
}

TEST_F(ProdInvariantTest, verifyInvariants) {
  auto setup = [&]() {};
  auto verify = [&]() {
    verifyCopp();
    verifyLoadBalancing();
    verifyDscpToQueueMapping();
    verifySafeDiagCommands();
  };
  verifyAcrossWarmBoots(setup, verify);
}

void ProdInvariantTest::verifySafeDiagCommands() {
  std::set<std::string> diagCmds;
  switch (sw()->getPlatform()->getAsic()->getAsicType()) {
    case HwAsic::AsicType::ASIC_TYPE_FAKE:
    case HwAsic::AsicType::ASIC_TYPE_MOCK:
    case HwAsic::AsicType::ASIC_TYPE_EBRO:
    case HwAsic::AsicType::ASIC_TYPE_GARONNE:
    case HwAsic::AsicType::ASIC_TYPE_ELBERT_8DD:
      break;

    case HwAsic::AsicType::ASIC_TYPE_TRIDENT2:
      diagCmds = validated_shell_commands_constants::TD2_TESTED_CMDS();
      break;
    case HwAsic::AsicType::ASIC_TYPE_TOMAHAWK:
      diagCmds = validated_shell_commands_constants::TH_TESTED_CMDS();
      break;
    case HwAsic::AsicType::ASIC_TYPE_TOMAHAWK3:
      diagCmds = validated_shell_commands_constants::TH3_TESTED_CMDS();
      break;
    case HwAsic::AsicType::ASIC_TYPE_TOMAHAWK4:
      diagCmds = validated_shell_commands_constants::TH4_TESTED_CMDS();
      break;
  }
  if (diagCmds.size()) {
    for (auto i = 0; i < 10; ++i) {
      for (auto cmd : diagCmds) {
        std::string out;
        platform()->getHwSwitch()->printDiagCmd(cmd + "\n");
      }
    }
    std::string out;
    platform()->getHwSwitch()->printDiagCmd("quit\n");
  }
}
void ProdInvariantTest::verifyQueuePerHostMapping(bool dscpMarkingTest) {
  auto vlanId = utility::firstVlanID(sw()->getState());
  auto intfMac = utility::getInterfaceMac(sw()->getState(), vlanId);
  auto srcMac = utility::MacAddressGenerator().get(intfMac.u64NBO());

  // if DscpMarkingTest is set, send unmarked packet matching DSCP marking ACL,
  // but expect queue-per-host to be honored, as the DSCP Marking ACL is listed
  // AFTER queue-per-host ACL by design.
  std::optional<uint16_t> l4SrcPort = std::nullopt;
  std::optional<uint8_t> dscp = std::nullopt;
  if (dscpMarkingTest) {
    l4SrcPort = utility::kUdpPorts().front();
    dscp = 0;
  }
  auto getHwPortStatsFn =
      [&](const std::vector<PortID>& portIds) -> std::map<PortID, HwPortStats> {
    return getLatestPortStats(portIds);
  };

  utility::verifyQueuePerHostMapping(
      platform()->getHwSwitch(),
      sw()->getState(),
      getEcmpPortIds(),
      vlanId,
      srcMac,
      intfMac,
      folly::IPAddressV4("1.0.0.1"),
      folly::IPAddressV4("10.10.1.2"),
      true /* useFrontPanel */,
      false /* blockNeighbor */,
      getHwPortStatsFn,
      l4SrcPort,
      std::nullopt, /* l4DstPort */
      dscp);
}
void ProdInvariantTest::verifyDscpToQueueMapping() {
  if (!sw()->getPlatform()->getAsic()->isSupported(HwAsic::Feature::L3_QOS)) {
    return;
  }

  auto uplinkDownlinkPorts = utility::getAllUplinkDownlinkPorts(
      platform()->getHwSwitch(), initialConfig(), kEcmpWidth, false);

  // pick the first one
  auto downlinkPortId = uplinkDownlinkPorts.second[0];
  // gather all uplink + downlink ports
  std::vector<PortID> portIds = uplinkDownlinkPorts.first;
  for (auto it = uplinkDownlinkPorts.second.begin();
       it != uplinkDownlinkPorts.second.end();
       ++it) {
    portIds.push_back(*it);
  }

  auto getPortStatsFn = [&]() -> std::map<PortID, HwPortStats> {
    return getLatestPortStats(portIds);
  };

  auto q2dscpMap = utility::getOlympicQosMaps(initialConfig());
  EXPECT_TRUE(utility::verifyQueueMappingsInvariantHelper(
      q2dscpMap,
      sw()->getHw(),
      sw()->getState(),
      getPortStatsFn,
      getEcmpPortIds(),
      downlinkPortId));

  XLOG(INFO) << "Verify DSCP to Queue mapping done";
}

class ProdInvariantRswMhnicTest : public ProdInvariantTest {
 protected:
  void SetUp() override {
    // Todo: Refractor could be done here to avoid code duplication.
    AgentTest::SetUp();
    auto ecmpUplinlinkPorts =
        utility::getAllUplinkDownlinkPorts(
            platform()->getHwSwitch(), initialConfig(), kEcmpWidth, false)
            .first;
    for (auto& uplinkPort : ecmpUplinlinkPorts) {
      ecmpPorts_.push_back(PortDescriptor(uplinkPort));
    }
    setupRSWMhnicEcmpV4(ecmpPorts_);
    XLOG(INFO) << "ProdInvariantTest setup done";
  }
  cfg::SwitchConfig initialConfig() override {
    // TODO: Currently ProdInvariantTests only has support for BCM switches.
    // That's why we're passing false in the call below.
    auto config = utility::createProdRswMhnicConfig(
        platform()->getHwSwitch(),
        getAllPlatformPorts(platform()->getPlatformPorts()),
        false /* isSai() */);
    return config;
  }
  RoutePrefix<folly::IPAddressV4> kGetRoutePrefix() {
    // Currently hardcoded to IPv4. Enabling IPv6 testing for all classes is on
    // the to-do list, after which we can choose v4 or v6 with the same model as
    // kGetRoutePrefix in HwQueuePerHostRouteTests.
    return RoutePrefix<folly::IPAddressV4>{folly::IPAddressV4{"10.10.1.0"}, 24};
  }

 private:
  void setupRSWMhnicEcmpV4(const std::vector<PortDescriptor>& ecmpPorts) {
    ASSERT_GT(ecmpPorts.size(), 0);

    boost::container::flat_set<PortDescriptor> ports;
    std::for_each(ecmpPorts.begin(), ecmpPorts.end(), [&ports](auto ecmpPort) {
      ports.insert(ecmpPort);
    });

    sw()->updateStateBlocking("Resolve nhops", [&](auto state) {
      utility::EcmpSetupTargetedPorts4 ecmp4(state);
      return ecmp4.resolveNextHops(state, ports);
    });

    utility::EcmpSetupTargetedPorts4 ecmp4(sw()->getState());
    ecmp4.programRoutes(
        std::make_unique<SwSwitchRouteUpdateWrapper>(sw()->getRouteUpdater()),
        ports,
        {kGetRoutePrefix()});
  }
};
} // namespace facebook::fboss
