// (c) Meta Platforms, Inc. and affiliates. Confidential and proprietary.

#include "fboss/agent/state/TeFlowTable.h"

#include <folly/IPAddressV6.h>
#include "fboss/agent/FbossError.h"
#include "fboss/agent/state/Interface.h"
#include "fboss/agent/state/NodeMap-defs.h"
#include "fboss/agent/state/SwitchState.h"
#include "fboss/agent/state/TeFlowEntry.h"
#include "fboss/agent/state/Vlan.h"
#include "folly/MacAddress.h"

namespace facebook::fboss {

TeFlowTable::TeFlowTable() {}

TeFlowTable::~TeFlowTable() {}

TeFlowTable* TeFlowTable::addTeFlowEntry(
    std::shared_ptr<SwitchState>* state,
    const FlowEntry& entry) {
  auto* writableTable = modify(state);
  auto oldFlowEntry = writableTable->getTeFlowIf(*entry.flow());

  auto fillFlowInfo = [&entry, &state](auto& tableEntry) {
    tableEntry->setNextHops(*entry.nextHops());
    std::vector<NextHopThrift> resolvedNextHops;
    for (const auto& nexthop : *entry.nextHops()) {
      if (!TeFlowTable::isNexthopResolved(nexthop, *state)) {
        std::string nhJson;
        apache::thrift::SimpleJSONSerializer::serialize(nexthop, &nhJson);
        throw FbossError(
            "Invalid redirection nexthop for TE flow. NH: ", nhJson);
      }
      resolvedNextHops.emplace_back(nexthop);
    }
    tableEntry->setResolvedNextHops(std::move(resolvedNextHops));
    if (entry.counterID().has_value()) {
      tableEntry->setCounterID(entry.counterID().value());
    } else {
      tableEntry->setCounterID(std::nullopt);
    }
    tableEntry->setEnabled(true);
  };

  if (!oldFlowEntry) {
    auto teFlowEntry = std::make_shared<TeFlowEntry>(*entry.flow());
    fillFlowInfo(teFlowEntry);
    writableTable->addNode(teFlowEntry);
  } else {
    auto* entryToUpdate = oldFlowEntry->modify(state);
    fillFlowInfo(entryToUpdate);
  }
  return writableTable;
}

TeFlowTable* TeFlowTable::removeTeFlowEntry(
    std::shared_ptr<SwitchState>* state,
    const TeFlow& flowId) {
  auto* writableTable = modify(state);
  auto oldFlowEntry = writableTable->getTeFlowIf(flowId);
  if (!oldFlowEntry) {
    std::string flowStr;
    toAppend(flowId, &flowStr);
    throw FbossError("Request to delete a non existing flow entry :", flowStr);
  }
  writableTable->removeNodeIf(flowId);
  return writableTable;
}

TeFlowTable* TeFlowTable::modify(std::shared_ptr<SwitchState>* state) {
  if (!isPublished()) {
    CHECK(!(*state)->isPublished());
    return this;
  }

  auto newTable = clone();
  auto* ptr = newTable.get();
  (*state)->resetTeFlowTable(std::move(newTable));
  return ptr;
}

bool TeFlowTable::isNexthopResolved(
    NextHopThrift nexthop,
    std::shared_ptr<SwitchState> state) {
  auto nhop = util::fromThrift(nexthop, true);
  if (!nhop.isResolved()) {
    XLOG(WARNING) << "Unresolved nexthop for TE flow " << nhop;
    return false;
  }
  auto vlanID = state->getInterfaces()
                    ->getInterfaceIf(nhop.intfID().value())
                    ->getVlanID();
  folly::MacAddress dstMac;
  if (nhop.addr().isV6()) {
    auto nhopAddr = folly::IPAddress::createIPv6(nhop.addr());
    auto ndpEntry =
        state->getVlans()->getVlanIf(vlanID)->getNdpTable()->getEntryIf(
            nhopAddr);
    if (!ndpEntry) {
      XLOG(WARNING) << "No NDP entry for TE flow redirection nexthop " << nhop;
      return false;
    }
    dstMac = ndpEntry->getMac();
  } else {
    auto nhopAddr = folly::IPAddress::createIPv4(nhop.addr());
    auto arpEntry =
        state->getVlans()->getVlanIf(vlanID)->getArpTable()->getEntryIf(
            nhopAddr);
    if (!arpEntry) {
      XLOG(WARNING) << "No ARP entry for TE flow redirection nexthop " << nhop;
      return false;
    }
    dstMac = arpEntry->getMac();
  }
  if (dstMac.isBroadcast()) {
    XLOG(WARNING)
        << "No resolved neighbor entry for TE flow redirection nexthop "
        << nhop;
    return false;
  }
  return true;
}

void toAppend(const TeFlow& flow, std::string* result) {
  std::string flowJson;
  apache::thrift::SimpleJSONSerializer::serialize(flow, &flowJson);
  result->append(flowJson);
}

FBOSS_INSTANTIATE_NODE_MAP(TeFlowTable, TeFlowTableTraits);

} // namespace facebook::fboss
