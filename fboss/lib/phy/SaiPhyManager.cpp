/*
 *  Copyright (c) 2004-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "fboss/lib/phy/SaiPhyManager.h"

#include "fboss/agent/hw/sai/switch/SaiSwitch.h"
#include "fboss/agent/platforms/sai/SaiHwPlatform.h"

namespace facebook::fboss {
SaiPhyManager::SaiPhyManager(const PlatformMapping* platformMapping)
    : PhyManager(platformMapping) {}

SaiPhyManager::~SaiPhyManager() {}

SaiHwPlatform* SaiPhyManager::getSaiPlatform(GlobalXphyID xphyID) const {
  const auto& phyIDInfo = getPhyIDInfo(xphyID);
  auto pimPlatforms = saiPlatforms_.find(phyIDInfo.pimID);
  if (pimPlatforms == saiPlatforms_.end()) {
    throw FbossError("No SaiHwPlatform is created for pimID:", phyIDInfo.pimID);
  }
  auto platformItr = pimPlatforms->second.find(xphyID);
  if (platformItr == pimPlatforms->second.end()) {
    throw FbossError("SaiHwPlatform is not created for globalPhyID:", xphyID);
  }
  return platformItr->second.get();
}

SaiHwPlatform* SaiPhyManager::getSaiPlatform(PortID portID) const {
  return getSaiPlatform(getGlobalXphyIDbyPortID(portID));
}

SaiSwitch* SaiPhyManager::getSaiSwitch(GlobalXphyID xphyID) const {
  return static_cast<SaiSwitch*>(getSaiPlatform(xphyID)->getHwSwitch());
}
SaiSwitch* SaiPhyManager::getSaiSwitch(PortID portID) const {
  return static_cast<SaiSwitch*>(getSaiPlatform(portID)->getHwSwitch());
}

void SaiPhyManager::addSaiPlatform(
    GlobalXphyID xphyID,
    std::unique_ptr<SaiHwPlatform> platform) {
  const auto phyIDInfo = getPhyIDInfo(xphyID);
  saiPlatforms_[phyIDInfo.pimID][xphyID] = std::move(platform);
}

void SaiPhyManager::sakInstallTx(const mka::MKASak& sak) {
  auto portId = getPortId(*sak.l2Port_ref());
  auto macsecManager = getMacsecManager(portId);
  macsecManager->setupMacsec(
      portId, sak, *sak.sci_ref(), SAI_MACSEC_DIRECTION_EGRESS);
}

void SaiPhyManager::sakInstallRx(
    const mka::MKASak& sak,
    const mka::MKASci& sci) {
  auto portId = getPortId(*sak.l2Port_ref());
  auto macsecManager = getMacsecManager(portId);
  // Use the SCI of the peer
  macsecManager->setupMacsec(portId, sak, sci, SAI_MACSEC_DIRECTION_INGRESS);
}

SaiMacsecManager* SaiPhyManager::getMacsecManager(PortID portId) {
  auto saiSwitch = getSaiSwitch(portId);
  return &saiSwitch->managerTable()->macsecManager();
}

PortID SaiPhyManager::getPortId(std::string portName) {
  auto platPorts = getPlatformMapping()->getPlatformPorts();
  for (const auto& pair : platPorts) {
    if (folly::to<std::string>(*pair.second.mapping_ref()->name_ref()) ==
        portName) {
      return PortID(pair.first);
    }
  }
  throw FbossError("Could not find port ID for port ", portName);
}
} // namespace facebook::fboss
