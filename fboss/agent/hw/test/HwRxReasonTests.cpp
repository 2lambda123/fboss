/*
 *  Copyright (c) 2004-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "fboss/agent/hw/test/ConfigFactory.h"
#include "fboss/agent/hw/test/HwLinkStateDependentTest.h"
#include "fboss/agent/hw/test/HwTestCoppUtils.h"

namespace facebook::fboss {

class HwRxReasonTests : public HwLinkStateDependentTest {
 protected:
  cfg::SwitchConfig initialConfig() const override {
    auto cfg = utility::oneL3IntfConfig(
        getHwSwitch(), masterLogicalPortIds()[0], cfg::PortLoopbackMode::MAC);
    utility::setDefaultCpuTrafficPolicyConfig(cfg, getAsic());
    // Remove DHCP from rxReason list
    auto rxReasonListWithoutDHCP = utility::getCoppRxReasonToQueues(getAsic());
    std::remove(
        rxReasonListWithoutDHCP.begin(),
        rxReasonListWithoutDHCP.end(),
        ControlPlane::makeRxReasonToQueueEntry(
            cfg::PacketRxReason::DHCP, utility::kCoppMidPriQueueId));
    cfg.cpuTrafficPolicy_ref()->rxReasonToQueueOrderedList_ref() =
        rxReasonListWithoutDHCP;
    return cfg;
  }
};

TEST_F(HwRxReasonTests, InsertAndRemoveRxReason) {
  auto setup = [=]() {};
  auto verify = [=]() {
    auto cfg = initialConfig();
    applyNewConfig(cfg);

    auto rxReasonListWithDHCP = utility::getCoppRxReasonToQueues(getAsic());
    rxReasonListWithDHCP.insert(
        rxReasonListWithDHCP.begin(),
        ControlPlane::makeRxReasonToQueueEntry(
            cfg::PacketRxReason::DHCP, utility::kCoppMidPriQueueId));
    cfg.cpuTrafficPolicy_ref()->rxReasonToQueueOrderedList_ref() =
        rxReasonListWithDHCP;
    applyNewConfig(cfg);
  };
  verifyAcrossWarmBoots(setup, verify);
}

} // namespace facebook::fboss
