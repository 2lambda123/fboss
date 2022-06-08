// Copyright 2014-present Facebook. All Rights Reserved.

#include <fboss/platform/data_corral_service/ChassisManager.h>

namespace facebook::fboss::platform::data_corral_service {

class DarwinChassisManager : public ChassisManager {
 public:
  virtual void initFruModules() override;
  virtual void programChassis() override;
};

} // namespace facebook::fboss::platform::data_corral_service
