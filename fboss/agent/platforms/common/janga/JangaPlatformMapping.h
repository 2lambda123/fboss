/*
 *  Copyright (c) 2004-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
#pragma once

#include "fboss/agent/platforms/common/PlatformMapping.h"

namespace facebook {
namespace fboss {

class JangaPlatformMapping : public PlatformMapping {
 public:
  JangaPlatformMapping();
  explicit JangaPlatformMapping(const std::string& platformMappingStr);

 private:
  // Forbidden copy constructor and assignment operator
  JangaPlatformMapping(JangaPlatformMapping const&) = delete;
  JangaPlatformMapping& operator=(JangaPlatformMapping const&) = delete;
};
} // namespace fboss
} // namespace facebook
