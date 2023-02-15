// (c) Meta Platforms, Inc. and affiliates. Confidential and proprietary.

#include "fboss/agent/test/AgentEnsemble.h"

#include "fboss/agent/AgentConfig.h"
#include "fboss/agent/Utils.h"
#include "fboss/agent/hw/test/ConfigFactory.h"
#include "fboss/lib/config/PlatformConfigUtils.h"

DECLARE_bool(tun_intf);
DEFINE_bool(
    setup_for_warmboot,
    false,
    "Set up test for SDK warmboot. Useful for testing individual "
    "tests doing a full process warmboot and verifying expectations");

namespace {
void initFlagDefaults(const std::map<std::string, std::string>& defaults) {
  for (auto item : defaults) {
    gflags::SetCommandLineOptionWithMode(
        item.first.c_str(), item.second.c_str(), gflags::SET_FLAGS_DEFAULT);
  }
}

int kArgc;
char** kArgv;
facebook::fboss::PlatformInitFn kPlatformInitFn;
static std::string kInputConfigFile;

} // namespace

namespace facebook::fboss {
AgentEnsemble::AgentEnsemble(const std::string& configFileName) {
  configFile_ = configFileName;
}

void AgentEnsemble::setupEnsemble(
    int argc,
    char** argv,
    uint32_t hwFeaturesDesired,
    PlatformInitFn initPlatform,
    AgentEnsembleSwitchConfigFn initialConfigFn,
    AgentEnsemblePlatformConfigFn platformConfigFn) {
  // to ensure FLAGS_config is set, as this is used in case platform config is
  // overriden by the application.
  gflags::ParseCommandLineFlags(&argc, &argv, false);

  if (platformConfigFn) {
    auto agentConf =
        AgentConfig::fromFile(AgentEnsemble::getInputConfigFile())->thrift;
    platformConfigFn(*(agentConf.platform()));
    // some platform config may need cold boots. so overwrite the config before
    // creating a switch
    writeConfig(agentConf, FLAGS_config);
  }
  auto* initializer = agentInitializer();
  initializer->createSwitch(argc, argv, hwFeaturesDesired, initPlatform);

  utility::setPortToDefaultProfileIDMap(
      std::make_shared<PortMap>(), getPlatform());
  auto portsByControllingPort =
      utility::getSubsidiaryPortIDs(getPlatform()->getPlatformPorts());

  for (const auto& port : portsByControllingPort) {
    masterLogicalPortIds_.push_back(port.first);
  }
  initialConfig_ = initialConfigFn(getHw(), masterLogicalPortIds_);
  writeConfig(initialConfig_);
  // reload the new config
  getPlatform()->reloadConfig();
}

void AgentEnsemble::startAgent() {
  // TODO: provide a way to enable tun intf, for now disable it expressedly,
  // this can also be done with CLI option, however netcastle runners can not
  // use that option, because hw tests and hw benchmarks using hwswitch ensemble
  // doesn't have CLI option to disable tun intf. Also get rid of explicit
  // setting this flag and emply CLI option to disable tun manager.
  FLAGS_tun_intf = false;
  auto* initializer = agentInitializer();
  asyncInitThread_.reset(
      new std::thread([initializer] { initializer->initAgent(); }));
  asyncInitThread_->detach();
  initializer->initializer()->waitForInitDone();
}

void AgentEnsemble::writeConfig(const cfg::SwitchConfig& config) {
  auto* initializer = agentInitializer();
  auto agentConfig = initializer->sw()->getPlatform()->config()->thrift;
  agentConfig.sw() = config;
  writeConfig(agentConfig);
}

void AgentEnsemble::writeConfig(const cfg::AgentConfig& agentConfig) {
  auto* initializer = agentInitializer();
  auto testConfigDir =
      initializer->sw()->getPlatform()->getPersistentStateDir() +
      "/agent_ensemble/";
  utilCreateDir(testConfigDir);
  auto fileName = testConfigDir + configFile_;
  writeConfig(agentConfig, fileName);
}

void AgentEnsemble::writeConfig(
    const cfg::AgentConfig& agentConfig,
    const std::string& fileName) {
  auto newAgentConfig = AgentConfig(
      agentConfig,
      apache::thrift::SimpleJSONSerializer::serialize<std::string>(
          agentConfig));
  newAgentConfig.dumpConfig(fileName);
  if (kInputConfigFile.empty()) {
    // saving the original config file.
    kInputConfigFile = FLAGS_config;
  }
  FLAGS_config = fileName;
  initFlagDefaults(*newAgentConfig.thrift.defaultCommandLineArgs());
}

AgentEnsemble::~AgentEnsemble() {
  auto* initializer = agentInitializer();
  initializer->stopAgent(FLAGS_setup_for_warmboot);
}

void AgentEnsemble::applyNewConfig(
    const cfg::SwitchConfig& config,
    bool activate) {
  writeConfig(config);
  if (activate) {
    getSw()->applyConfig("applying new config", config);
  }
}

const std::vector<PortID>& AgentEnsemble::masterLogicalPortIds() const {
  return masterLogicalPortIds_;
}

void AgentEnsemble::programRoutes(
    const RouterID& rid,
    const ClientID& client,
    const utility::RouteDistributionGenerator::ThriftRouteChunks& routeChunks) {
  auto updater = getSw()->getRouteUpdater();
  for (const auto& routeChunk : routeChunks) {
    std::for_each(
        routeChunk.begin(),
        routeChunk.end(),
        [&updater, client, rid](const auto& route) {
          updater.addRoute(rid, client, route);
        });
    updater.program();
  }
}

void AgentEnsemble::unprogramRoutes(
    const RouterID& rid,
    const ClientID& client,
    const utility::RouteDistributionGenerator::ThriftRouteChunks& routeChunks) {
  auto updater = getSw()->getRouteUpdater();
  for (const auto& routeChunk : routeChunks) {
    std::for_each(
        routeChunk.begin(),
        routeChunk.end(),
        [&updater, client, rid](const auto& route) {
          updater.delRoute(rid, *route.dest(), client);
        });
    updater.program();
  }
}

std::shared_ptr<SwitchState> AgentEnsemble::applyNewState(
    std::shared_ptr<SwitchState> state,
    bool transaction) {
  if (!state) {
    return getSw()->getState();
  }
  transaction
      ? getSw()->updateStateWithHwFailureProtection(
            "apply new state with failure protection",
            [state](const std::shared_ptr<SwitchState>&) { return state; })
      : getSw()->updateStateBlocking(
            "apply new state",
            [state](const std::shared_ptr<SwitchState>&) { return state; });
  return getSw()->getState();
}

void AgentEnsemble::enableExactMatch(bcm::BcmConfig& config) {
  if (auto yamlCfg = config.yamlConfig()) {
    // use common func
    facebook::fboss::enableExactMatch(*yamlCfg);
  } else {
    auto& cfg = *(config.config());
    cfg["fpem_mem_entries"] = "0x10000";
  }
}

std::string AgentEnsemble::getInputConfigFile() {
  if (kInputConfigFile.empty()) {
    return FLAGS_config;
  }
  return kInputConfigFile;
}

void ensembleMain(int argc, char* argv[], PlatformInitFn initPlatform) {
  kArgc = argc;
  kArgv = argv;
  kPlatformInitFn = std::move(initPlatform);
}

std::unique_ptr<AgentEnsemble> createAgentEnsemble(
    AgentEnsembleSwitchConfigFn initialConfigFn,
    AgentEnsemblePlatformConfigFn platformConfigFn,
    uint32_t featuresDesired) {
  auto ensemble = std::make_unique<AgentEnsemble>();
  ensemble->setupEnsemble(
      kArgc,
      kArgv,
      featuresDesired,
      kPlatformInitFn,
      initialConfigFn,
      platformConfigFn);
  return ensemble;
}

} // namespace facebook::fboss
