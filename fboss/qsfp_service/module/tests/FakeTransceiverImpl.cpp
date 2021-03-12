// Copyright 2004-present Facebook. All Rights Reserved.

#include "fboss/qsfp_service/module/tests/FakeTransceiverImpl.h"
#include "fboss/qsfp_service/module/QsfpModule.h"

#include <gtest/gtest.h>

namespace facebook {
namespace fboss {

bool FakeTransceiverImpl::detectTransceiver() {
  return true;
}

int FakeTransceiverImpl::readTransceiver(
    int dataAddress,
    int offset,
    int len,
    uint8_t* fieldValue) {
  int read = 0;
  EXPECT_EQ(0x50, dataAddress);
  if (offset < QsfpModule::MAX_QSFP_PAGE_SIZE) {
    read = len;
    if (QsfpModule::MAX_QSFP_PAGE_SIZE - offset < len) {
      read = QsfpModule::MAX_QSFP_PAGE_SIZE - offset;
    }
    std::copy(
        pageLower_.begin() + offset,
        pageLower_.begin() + offset + read,
        fieldValue);
    len -= read;
    offset = QsfpModule::MAX_QSFP_PAGE_SIZE;
  }
  if (len > 0 && offset >= QsfpModule::MAX_QSFP_PAGE_SIZE) {
    offset -= QsfpModule::MAX_QSFP_PAGE_SIZE;
    EXPECT_LE(len + offset, QsfpModule::MAX_QSFP_PAGE_SIZE);
    assert(upperPages_.find(page_) != upperPages_.end());
    std::copy(
        upperPages_[page_].begin() + offset,
        upperPages_[page_].begin() + offset + len,
        fieldValue + read);
    read += len;
  }
  return read;
}

int FakeTransceiverImpl::writeTransceiver(
    int /*dataAddress*/,
    int offset,
    int len,
    uint8_t* fieldValue) {
  if (offset == 127) {
    page_ = *fieldValue;
  }
  if (offset < QsfpModule::MAX_QSFP_PAGE_SIZE) {
    EXPECT_LE(offset + len, QsfpModule::MAX_QSFP_PAGE_SIZE);
    std::copy(fieldValue, fieldValue + len, pageLower_.begin() + offset);
  } else {
    EXPECT_LE(offset + len, 2 * QsfpModule::MAX_QSFP_PAGE_SIZE);
    offset -= QsfpModule::MAX_QSFP_PAGE_SIZE;
    std::copy(
        fieldValue, fieldValue + len, upperPages_[page_].begin() + offset);
  }

  return len;
}

folly::StringPiece FakeTransceiverImpl::getName() {
  return moduleName_;
}

int FakeTransceiverImpl::getNum() const {
  return module_;
}

// Below are randomly generated eeprom maps for testing purpose and doesn't
// accurately reflect an actual module

std::array<uint8_t, 128> kSffDacPageLower = {
    0x11, 0x07, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

std::array<uint8_t, 128> kSffDacPage0 = {
    0x11, 0x00, 0x23, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05,
    0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xa0, 0x46, 0x41, 0x43, 0x45,
    0x54, 0x45, 0x53, 0x54, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x00, 0x78, 0xa7, 0x14, 0x4e, 0x44, 0x41, 0x51, 0x47, 0x46, 0x2d, 0x46,
    0x33, 0x30, 0x32, 0x20, 0x20, 0x20, 0x20, 0x20, 0x41, 0x20, 0x05, 0x07,
    0x09, 0x0d, 0xff, 0xff, 0x0b, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x32, 0x30, 0x32, 0x33, 0x4a, 0x20, 0x20,
    0x31, 0x37, 0x30, 0x37, 0x32, 0x33, 0x20, 0x20, 0x00, 0x00, 0x67, 0x44,
    0x02, 0x00, 0x30, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};

std::array<uint8_t, 128> kSffCwdm4PageLower = {
    0x0d, 0x00, 0x02, 0xab, 0xbc, 0xcd, 0xc0, 0x30, 0x00, 0xc5, 0xf3, 0xc5,
    0x3c, 0xc5, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x04,
    0x00, 0x00, 0x80, 0xdd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x23,
    0x23, 0x34, 0x13, 0x31, 0x12, 0x12, 0x14, 0x14, 0x11, 0x11, 0x22, 0x22,
    0x33, 0x33, 0x41, 0x41, 0x42, 0x42, 0x43, 0x43, 0x44, 0x44, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
};

std::array<uint8_t, 128> kSffCwdm4BadPageLower = {
    0x0d, 0xff, 0xff, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x04,
    0x00, 0x00, 0x80, 0xdd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
};

std::array<uint8_t, 128> kSffCwdm4Page0 = {
    0x0d, 0x10, 0x0c, 0x04, 0x00, 0x00, 0x00, 0x40, 0x40, 0x02, 0x00, 0x05,
    0x67, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0x00, 0x46, 0x41, 0x43, 0x45,
    0x54, 0x45, 0x53, 0x54, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x07, 0x00, 0x00, 0x00, 0x46, 0x54, 0x4c, 0x34, 0x31, 0x30, 0x51, 0x45,
    0x32, 0x43, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x41, 0x20, 0x42, 0x68,
    0x07, 0xd0, 0x46, 0x97, 0x06, 0x01, 0x04, 0xd0, 0x4d, 0x52, 0x45, 0x30,
    0x31, 0x42, 0x30, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x31, 0x34, 0x30, 0x35, 0x30, 0x32, 0x20, 0x20, 0x0a, 0x00, 0x00, 0x22,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

std::array<uint8_t, 128> kSffCwdm4Page3 = {
    0x4b, 0x00, 0xfb, 0x00, 0x46, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x94, 0x70, 0x6e, 0xf0, 0x86, 0xc4, 0x7b, 0x0c,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x11, 0x12, 0x00, 0x11, 0x22, 0x33,
    0x44, 0x55, 0x66, 0x77, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x11, 0x11, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x12, 0x34,
    0x21, 0x13, 0x13, 0x23, 0x2c, 0x3d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

std::array<uint8_t, 128> kCmisPageLower = {
    0x1e, 0x40, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5b, 0x00, 0x00,
    0x00, 0x00, 0x28, 0x45, 0x80, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x02, 0x07, 0x00, 0x00, 0x00, 0x01, 0x6f, 0x00, 0x00,
    0x00, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x02, 0x0f, 0x18, 0x44, 0x01, 0x0b, 0x10, 0x44, 0x01, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x14};

std::array<uint8_t, 128> kCmisPage0 = {
    0x1e, 0x46, 0x41, 0x43, 0x45, 0x54, 0x45, 0x53, 0x54, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

std::array<uint8_t, 128> kCmisPage1 = {
    0x00, 0x00, 0x03, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0xa4,
    0x1b, 0xee, 0x20, 0xdf, 0x57, 0x00, 0x41, 0x0f, 0x00, 0x00, 0x9d, 0x38,
    0x00, 0xf0, 0x77, 0x2f, 0x07, 0x07, 0x06, 0x03, 0x07, 0x09, 0x3d, 0x60,
    0x0f, 0x9f, 0x80, 0x37, 0x37, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0a, 0x03, 0x65, 0x01, 0x0a,
    0x03, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41};

std::array<uint8_t, 128> kCmisPage2 = {
    0x46, 0x00, 0x0a, 0x00, 0x46, 0x00, 0x0a, 0x00, 0x8b, 0x42, 0x76, 0x8e,
    0x8b, 0x42, 0x76, 0x8e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xb6, 0xb6, 0x07, 0x71, 0xb6, 0xb6, 0x07, 0x71, 0x92, 0x7c, 0x1d, 0x4c,
    0x92, 0x7c, 0x1d, 0x4c, 0xb6, 0xb6, 0x00, 0x01, 0xb6, 0xb6, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd2};

std::array<uint8_t, 128> kCmisPage10 = {
    0x06, 0x00, 0x0a, 0x0b, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0c,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

std::array<uint8_t, 128> kCmisPage11 = {
    0x24, 0x35, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x0b, 0x0c, 0x0d, 0x02,
    0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x05, 0x06, 0x0a, 0x0b, 0x0c,
    0x0d, 0x00, 0x53, 0x34, 0x51, 0x0c, 0x50, 0x20, 0x52, 0x23, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5e, 0x9d, 0x61, 0xd1, 0x68, 0x86,
    0x61, 0xb6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xc0,
    0x0f, 0x81, 0x16, 0xb4, 0x14, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x08, 0x08,
    0x08, 0x08, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x22,
    0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x33, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x11, 0x21, 0x31, 0x41, 0x00, 0x00, 0x00, 0x00,
    0x11, 0x21, 0x31, 0x41, 0x00, 0x00, 0x00, 0x00};

std::array<uint8_t, 128> kCmisPage13 = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

std::map<int, std::array<uint8_t, 128>> kSffCwdm4UpperPages = {
    {0, kSffCwdm4Page0},
    {3, kSffCwdm4Page3}};

std::map<int, std::array<uint8_t, 128>> kSffDacUpperPages = {{0, kSffDacPage0}};

std::map<int, std::array<uint8_t, 128>> kCmisUpperPages = {
    {0, kCmisPage0},
    {1, kCmisPage1},
    {2, kCmisPage2},
    {0x10, kCmisPage10},
    {0x11, kCmisPage11},
    {0x13, kCmisPage13}};

SffDacTransceiver::SffDacTransceiver(int module)
    : FakeTransceiverImpl(module, kSffDacPageLower, kSffDacUpperPages) {}

SffCwdm4Transceiver::SffCwdm4Transceiver(int module)
    : FakeTransceiverImpl(module, kSffCwdm4PageLower, kSffCwdm4UpperPages) {}

BadSffCwdm4Transceiver::BadSffCwdm4Transceiver(int module)
    : FakeTransceiverImpl(module, kSffCwdm4BadPageLower, kSffCwdm4UpperPages) {}

Cmis200GTransceiver::Cmis200GTransceiver(int module)
    : FakeTransceiverImpl(module, kCmisPageLower, kCmisUpperPages) {}

} // namespace fboss
} // namespace facebook
