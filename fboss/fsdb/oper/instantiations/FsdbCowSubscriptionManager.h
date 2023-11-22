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

#include <fboss/fsdb/oper/CowSubscriptionManager.h>
#include <fboss/fsdb/oper/instantiations/FsdbCowStorage.h>
#include "fboss/fsdb/if/FsdbModel.h"

namespace facebook::fboss::fsdb {

extern template class CowSubscriptionManager<CowStorage<FsdbOperStateRoot>>;

} // namespace facebook::fboss::fsdb
