// (c) Meta Platforms, Inc. and affiliates. Confidential and proprietary.

#include "fboss/agent/test/MonoAgentEnsemble.h"

namespace facebook::fboss {

MonoAgentEnsemble::~MonoAgentEnsemble() {
  agentInitializer_.stopAgent(false);
}

const SwAgentInitializer* MonoAgentEnsemble::agentInitializer() const {
  return &agentInitializer_;
}

SwAgentInitializer* MonoAgentEnsemble::agentInitializer() {
  return &agentInitializer_;
}

void MonoAgentEnsemble::createSwitch(
    std::unique_ptr<AgentConfig> config,
    uint32_t hwFeaturesDesired,
    PlatformInitFn initPlatform) {
  agentInitializer_.createSwitch(
      std::move(config), hwFeaturesDesired, initPlatform);
}

void MonoAgentEnsemble::reloadPlatformConfig() {
  agentInitializer_.platform()->reloadConfig();
}
} // namespace facebook::fboss
