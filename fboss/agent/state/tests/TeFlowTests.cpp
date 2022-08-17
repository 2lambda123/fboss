// (c) Meta Platforms, Inc. and affiliates. Confidential and proprietary.

#include <gtest/gtest.h>
#include "fboss/agent/state/SwitchState.h"
#include "fboss/agent/state/TeFlowEntry.h"
#include "fboss/agent/test/HwTestHandle.h"
#include "fboss/agent/test/TestUtils.h"
#include "folly/IPAddressV6.h"

using namespace facebook::fboss;

using facebook::network::toBinaryAddress;
using facebook::network::thrift::BinaryAddress;
using folly::IPAddress;
using folly::IPAddressV4;
using folly::IPAddressV6;
using folly::StringPiece;

namespace {
static TeCounterID kCounterID("counter0");
static std::string kNhopAddrA("2401:db00:2110:3001::0002");
static std::string kNhopAddrB("2401:db00:2110:3055::0002");
static folly::MacAddress kMacAddress("01:02:03:04:05:06");
static VlanID kVlanA(1);
static VlanID kVlanB(55);
static PortID kPortIDA(1);
static PortID kPortIDB(11);
} // namespace

class TeFlowTest : public ::testing::Test {
 public:
  void SetUp() override {
    auto config = testConfigA();
    handle_ = createTestHandle(&config);
    sw_ = handle_->getSw();
    sw_->getNeighborUpdater()->receivedNdpMine(
        kVlanA,
        folly::IPAddressV6(kNhopAddrA),
        kMacAddress,
        PortDescriptor(kPortIDA),
        ICMPv6Type::ICMPV6_TYPE_NDP_NEIGHBOR_ADVERTISEMENT,
        0);
    sw_->getNeighborUpdater()->receivedNdpMine(
        kVlanB,
        folly::IPAddressV6(kNhopAddrB),
        kMacAddress,
        PortDescriptor(kPortIDB),
        ICMPv6Type::ICMPV6_TYPE_NDP_NEIGHBOR_ADVERTISEMENT,
        0);

    sw_->getNeighborUpdater()->waitForPendingUpdates();
    waitForBackgroundThread(sw_);
    waitForStateUpdates(sw_);
  }

 protected:
  IpPrefix ipPrefix(StringPiece ip, int length) {
    IpPrefix result;
    result.ip() = toBinaryAddress(IPAddress(ip));
    result.prefixLength() = length;
    return result;
  }

  TeFlow makeFlowKey(std::string dstIp) {
    TeFlow flow;
    flow.srcPort() = 100;
    flow.dstPrefix() = ipPrefix(dstIp, 64);
    return flow;
  }

  std::shared_ptr<TeFlowEntry> makeFlowEntry(std::string dstIp) {
    auto flow = makeFlowKey(dstIp);
    auto flowEntry = std::make_shared<TeFlowEntry>(flow);
    std::vector<NextHopThrift> nexthops;
    NextHopThrift nhop;
    nhop.address() = toBinaryAddress(IPAddress(kNhopAddrA));
    nexthops.push_back(nhop);
    flowEntry->setEnabled(true);
    flowEntry->setCounterID(kCounterID);
    flowEntry->setNextHops(nexthops);
    flowEntry->setResolvedNextHops(nexthops);
    return flowEntry;
  }

  FlowEntry makeFlow(
      std::string dstIp,
      std::string nhop = kNhopAddrA,
      std::string ifname = "fboss1") {
    FlowEntry flowEntry;
    flowEntry.flow()->srcPort() = 100;
    flowEntry.flow()->dstPrefix() = ipPrefix(dstIp, 64);
    flowEntry.counterID() = kCounterID;
    flowEntry.nextHops()->resize(1);
    flowEntry.nextHops()[0].address() = toBinaryAddress(IPAddress(nhop));
    flowEntry.nextHops()[0].address()->ifName() = ifname;
    return flowEntry;
  }

  void verifyFlowEntry(
      std::shared_ptr<TeFlowEntry> entry,
      std::string nhop = kNhopAddrA,
      std::string counterID = kCounterID,
      std::string ifname = "fboss1") {
    EXPECT_NE(entry, nullptr);
    EXPECT_TRUE(entry->getEnabled());
    EXPECT_EQ(entry->getCounterID(), counterID);
    EXPECT_EQ(entry->getNextHops().size(), 1);
    auto expectedNhop = toBinaryAddress(IPAddress(nhop));
    expectedNhop.ifName() = ifname;
    EXPECT_EQ(entry->getNextHops()[0].address(), expectedNhop);
    EXPECT_EQ(entry->getResolvedNextHops().size(), 1);
    EXPECT_EQ(entry->getResolvedNextHops()[0].address(), expectedNhop);
  }
  std::unique_ptr<HwTestHandle> handle_;
  SwSwitch* sw_;
};

TEST_F(TeFlowTest, SerDeserFlowEntry) {
  auto flowEntry = makeFlowEntry("100::");
  auto serialized = flowEntry->toFollyDynamic();
  auto entryBack = TeFlowEntry::fromFollyDynamic(serialized);
  EXPECT_TRUE(*flowEntry == *entryBack);
}

TEST_F(TeFlowTest, serDeserSwitchState) {
  auto state = this->sw_->getState();
  state->modify(&state);
  auto flowTable = state->getTeFlowTable().get()->modify(&state);
  auto flowEntry1 = makeFlow("100::");
  flowTable->addTeFlowEntry(&state, flowEntry1);
  auto flowEntry2 = makeFlow("101::");
  flowTable->addTeFlowEntry(&state, flowEntry2);
  EXPECT_EQ(state->getTeFlowTable()->size(), 2);
  auto serialized = state->toFollyDynamic();
  auto stateBack = SwitchState::fromFollyDynamic(serialized);
  EXPECT_EQ(*state, *stateBack);
}

TEST_F(TeFlowTest, AddDeleteTeFlow) {
  auto state = sw_->getState();
  state->modify(&state);
  auto flowTable = state->getTeFlowTable().get()->modify(&state);
  auto flowId = makeFlowKey("100::");

  // No NDP entry should result in throw
  auto flowEntry = makeFlow("100::", kNhopAddrA, "fboss55");
  EXPECT_THROW(flowTable->addTeFlowEntry(&state, flowEntry), FbossError);

  // Valid nexthop should not throw
  flowEntry = makeFlow("100::");
  EXPECT_NO_THROW(flowTable->addTeFlowEntry(&state, flowEntry));
  auto tableEntry = flowTable->getTeFlowIf(flowId);
  verifyFlowEntry(tableEntry);

  // change flow entry
  flowEntry.nextHops()[0].address() = toBinaryAddress(IPAddress(kNhopAddrB));
  flowEntry.nextHops()[0].address()->ifName() = "fboss55";
  flowEntry.counterID() = "counter1";
  EXPECT_NO_THROW(flowTable->addTeFlowEntry(&state, flowEntry));
  tableEntry = flowTable->getTeFlowIf(flowId);
  verifyFlowEntry(tableEntry, kNhopAddrB, "counter1", "fboss55");
  // delete the entry
  flowTable->removeTeFlowEntry(&state, flowId);
  EXPECT_EQ(flowTable->getTeFlowIf(flowId), nullptr);
}
