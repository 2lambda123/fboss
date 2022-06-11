// Copyright 2004-present Facebook. All Rights Reserved.
#include "fboss/agent/lldp/LinkNeighbor.h"
#include <folly/io/Cursor.h>
#include <folly/io/IOBuf.h>
#include "fboss/agent/packet/PktUtil.h"

#include <gtest/gtest.h>
#include <chrono>

using namespace facebook::fboss;
using folly::IOBuf;
using folly::MacAddress;
using folly::io::Cursor;
using std::chrono::duration_cast;
using std::chrono::seconds;
using std::chrono::steady_clock;

const uint8_t basicLldpPacket[] = {
    0x01, 0x80, 0xc2, 0x00, 0x00, 0x0e, 0x2c, 0x54, 0x2d, 0xf5, 0x89, 0x3e,
    0x88, 0xcc, 0x02, 0x07, 0x04, 0x2c, 0x54, 0x2d, 0xf5, 0x89, 0x3e, 0x04,
    0x0d, 0x05, 0x45, 0x74, 0x68, 0x65, 0x72, 0x6e, 0x65, 0x74, 0x31, 0x2f,
    0x32, 0x33, 0x06, 0x02, 0x00, 0x78, 0x08, 0x07, 0x53, 0x45, 0x52, 0x56,
    0x45, 0x52, 0x53, 0x0a, 0x28, 0x72, 0x73, 0x77, 0x31, 0x62, 0x72, 0x2e,
    0x30, 0x37, 0x2e, 0x70, 0x72, 0x6e, 0x32, 0x2e, 0x66, 0x61, 0x63, 0x65,
    0x62, 0x6f, 0x6f, 0x6b, 0x2e, 0x63, 0x6f, 0x6d, 0x2e, 0x66, 0x61, 0x63,
    0x65, 0x62, 0x6f, 0x6f, 0x6b, 0x2e, 0x63, 0x6f, 0x6d, 0x0c, 0xa1, 0x43,
    0x69, 0x73, 0x63, 0x6f, 0x20, 0x4e, 0x65, 0x78, 0x75, 0x73, 0x20, 0x4f,
    0x70, 0x65, 0x72, 0x61, 0x74, 0x69, 0x6e, 0x67, 0x20, 0x53, 0x79, 0x73,
    0x74, 0x65, 0x6d, 0x20, 0x28, 0x4e, 0x58, 0x2d, 0x4f, 0x53, 0x29, 0x20,
    0x53, 0x6f, 0x66, 0x74, 0x77, 0x61, 0x72, 0x65, 0x20, 0x36, 0x2e, 0x30,
    0x28, 0x32, 0x29, 0x55, 0x32, 0x28, 0x34, 0x29, 0x0a, 0x54, 0x41, 0x43,
    0x20, 0x73, 0x75, 0x70, 0x70, 0x6f, 0x72, 0x74, 0x3a, 0x20, 0x68, 0x74,
    0x74, 0x70, 0x3a, 0x2f, 0x2f, 0x77, 0x77, 0x77, 0x2e, 0x63, 0x69, 0x73,
    0x63, 0x6f, 0x2e, 0x63, 0x6f, 0x6d, 0x2f, 0x74, 0x61, 0x63, 0x0a, 0x43,
    0x6f, 0x70, 0x79, 0x72, 0x69, 0x67, 0x68, 0x74, 0x20, 0x28, 0x63, 0x29,
    0x20, 0x32, 0x30, 0x30, 0x32, 0x2d, 0x32, 0x30, 0x31, 0x34, 0x2c, 0x20,
    0x43, 0x69, 0x73, 0x63, 0x6f, 0x20, 0x53, 0x79, 0x73, 0x74, 0x65, 0x6d,
    0x73, 0x2c, 0x20, 0x49, 0x6e, 0x63, 0x2e, 0x20, 0x41, 0x6c, 0x6c, 0x20,
    0x72, 0x69, 0x67, 0x68, 0x74, 0x73, 0x20, 0x72, 0x65, 0x73, 0x65, 0x72,
    0x76, 0x65, 0x64, 0x2e, 0x0e, 0x04, 0x00, 0x14, 0x00, 0x14, 0x10, 0x0e,
    0x07, 0x06, 0x2c, 0x54, 0x2d, 0xf5, 0x89, 0x3e, 0x02, 0x05, 0x00, 0x00,
    0x00, 0x00, 0xfe, 0x4b, 0x00, 0x1b, 0x21, 0x01, 0x02, 0x0a, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x10, 0x14, 0x00, 0x00,
    0x80, 0x00, 0x23, 0x28, 0x23, 0x28, 0x23, 0x28, 0x23, 0x28, 0x23, 0x28,
    0x23, 0x28, 0x23, 0x28, 0x23, 0x28, 0x04, 0x1c, 0x00, 0x00, 0x80, 0x00,
    0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x0d,
    0x00, 0x0d, 0x00, 0x0d, 0x00, 0x0c, 0x00, 0x0c, 0x00, 0x0c, 0x00, 0x0c,
    0x0c, 0x05, 0x00, 0x00, 0x80, 0x01, 0x80, 0xfe, 0x05, 0x00, 0x01, 0x42,
    0x01, 0x01, 0xfe, 0x06, 0x00, 0x80, 0xc2, 0x01, 0x07, 0xfc, 0x00, 0x00,
};

TEST(LinkNeighbor, parseLldp) {
  IOBuf iob(IOBuf::WRAP_BUFFER, basicLldpPacket, sizeof(basicLldpPacket));
  Cursor cursor(&iob);

  // Parse the data
  MacAddress destMac = PktUtil::readMac(&cursor);
  EXPECT_EQ(MacAddress("01:80:c2:00:00:0e"), destMac);
  MacAddress srcMac = PktUtil::readMac(&cursor);
  EXPECT_EQ(MacAddress("2c:54:2d:f5:89:3e"), srcMac);
  uint16_t ethertype = cursor.readBE<uint16_t>();
  EXPECT_EQ(0x88cc, ethertype);

  LinkNeighbor info;
  PortID srcPort{0};
  VlanID srcVlan{1};
  bool ret = info.parseLldpPdu(srcPort, srcVlan, srcMac, ethertype, &cursor);
  ASSERT_TRUE(ret);

  EXPECT_EQ(lldp::LinkProtocol::LLDP, info.getProtocol());
  EXPECT_EQ(lldp::LldpChassisIdType::MAC_ADDRESS, info.getChassisIdType());
  EXPECT_EQ("\x2c\x54\x2d\xf5\x89\x3e", info.getChassisId());

  EXPECT_EQ(lldp::LldpPortIdType::INTERFACE_NAME, info.getPortIdType());
  EXPECT_EQ("Ethernet1/23", info.getPortId());

  EXPECT_EQ(seconds(120), info.getTTL());
  auto expire = steady_clock::now() + seconds(120);
  auto delta = expire - info.getExpirationTime();
  EXPECT_LE(delta, seconds(1));
  EXPECT_GE(delta, seconds(0));

  EXPECT_EQ(0x0014, info.getCapabilities());
  EXPECT_EQ(0x0014, info.getEnabledCapabilities());

  EXPECT_EQ("rsw1br.07.prn2.facebook.com.facebook.com", info.getSystemName());
  EXPECT_EQ("SERVERS", info.getPortDescription());
}

const uint8_t badLldpPacket[] = {
    0x01, 0x80, 0xc2, 0x00, 0x00, 0x0e, 0x2c, 0x54, 0x2d, 0xf5, 0x89, 0x3e,
    0x88, 0xcc, 0x02, 0x97, 0x04, 0x2c, 0x54, 0x2d, 0xf5, 0x89, 0x3e, 0x04,
    0x0d, 0x05, 0x45, 0x74, 0x68, 0x65, 0x72, 0x6e, 0x65, 0x74, 0x31, 0x2f,
    0x32, 0x33, 0x06, 0x02, 0x00, 0x78, 0x08, 0x07, 0x53, 0x45, 0x52, 0x56,
    0x45, 0x52, 0x53, 0x0a, 0x28, 0x72, 0x73, 0x77, 0x31, 0x62, 0x72, 0x2e,
    0x30, 0x37, 0x2e, 0x70, 0x72, 0x6e, 0x32, 0x2e, 0x66, 0x61, 0x63, 0x65,
};

TEST(LinkNeighbor, badTlvLength) {
  IOBuf iob(IOBuf::WRAP_BUFFER, badLldpPacket, sizeof(badLldpPacket));
  Cursor cursor(&iob);

  // Parse the data
  MacAddress destMac = PktUtil::readMac(&cursor);
  EXPECT_EQ(MacAddress("01:80:c2:00:00:0e"), destMac);
  MacAddress srcMac = PktUtil::readMac(&cursor);
  EXPECT_EQ(MacAddress("2c:54:2d:f5:89:3e"), srcMac);
  uint16_t ethertype = cursor.readBE<uint16_t>();
  EXPECT_EQ(0x88cc, ethertype);

  LinkNeighbor info;
  PortID srcPort{0};
  VlanID srcVlan{1};
  bool ret = info.parseLldpPdu(srcPort, srcVlan, srcMac, ethertype, &cursor);
  EXPECT_FALSE(ret);
}

const uint8_t basicCdpPacket[] = {
    0x01, 0x00, 0x0c, 0xcc, 0xcc, 0xcc, 0x2c, 0x54, 0x2d, 0xf5, 0x89, 0x3e,
    0x01, 0x27, 0xaa, 0xaa, 0x03, 0x00, 0x00, 0x0c, 0x20, 0x00, 0x02, 0xb4,
    0xfa, 0xe5, 0x00, 0x01, 0x00, 0x39, 0x72, 0x73, 0x77, 0x31, 0x62, 0x72,
    0x2e, 0x30, 0x37, 0x2e, 0x70, 0x72, 0x6e, 0x32, 0x2e, 0x66, 0x61, 0x63,
    0x65, 0x62, 0x6f, 0x6f, 0x6b, 0x2e, 0x63, 0x6f, 0x6d, 0x2e, 0x66, 0x61,
    0x63, 0x65, 0x62, 0x6f, 0x6f, 0x6b, 0x2e, 0x63, 0x6f, 0x6d, 0x28, 0x46,
    0x4f, 0x43, 0x31, 0x36, 0x31, 0x38, 0x52, 0x31, 0x30, 0x54, 0x29, 0x00,
    0x02, 0x00, 0x11, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0xcc, 0x00, 0x04,
    0x0a, 0x05, 0xe0, 0x57, 0x00, 0x03, 0x00, 0x10, 0x45, 0x74, 0x68, 0x65,
    0x72, 0x6e, 0x65, 0x74, 0x31, 0x2f, 0x32, 0x33, 0x00, 0x04, 0x00, 0x08,
    0x00, 0x00, 0x02, 0x29, 0x00, 0x05, 0x00, 0x46, 0x43, 0x69, 0x73, 0x63,
    0x6f, 0x20, 0x4e, 0x65, 0x78, 0x75, 0x73, 0x20, 0x4f, 0x70, 0x65, 0x72,
    0x61, 0x74, 0x69, 0x6e, 0x67, 0x20, 0x53, 0x79, 0x73, 0x74, 0x65, 0x6d,
    0x20, 0x28, 0x4e, 0x58, 0x2d, 0x4f, 0x53, 0x29, 0x20, 0x53, 0x6f, 0x66,
    0x74, 0x77, 0x61, 0x72, 0x65, 0x2c, 0x20, 0x56, 0x65, 0x72, 0x73, 0x69,
    0x6f, 0x6e, 0x20, 0x36, 0x2e, 0x30, 0x28, 0x32, 0x29, 0x55, 0x32, 0x28,
    0x34, 0x29, 0x00, 0x06, 0x00, 0x14, 0x4e, 0x33, 0x4b, 0x2d, 0x43, 0x33,
    0x30, 0x36, 0x34, 0x50, 0x51, 0x2d, 0x31, 0x30, 0x47, 0x45, 0x00, 0x0a,
    0x00, 0x06, 0x07, 0xfc, 0x00, 0x12, 0x00, 0x05, 0x00, 0x00, 0x13, 0x00,
    0x05, 0x00, 0x00, 0x0b, 0x00, 0x05, 0x01, 0x00, 0x11, 0x00, 0x08, 0x00,
    0x00, 0x05, 0xdc, 0x00, 0x14, 0x00, 0x1f, 0x72, 0x73, 0x77, 0x31, 0x62,
    0x72, 0x2e, 0x30, 0x37, 0x2e, 0x70, 0x72, 0x6e, 0x32, 0x2e, 0x66, 0x61,
    0x63, 0x65, 0x62, 0x6f, 0x6f, 0x6b, 0x2e, 0x63, 0x6f, 0x6d, 0x00, 0x15,
    0x00, 0x12, 0x06, 0x0c, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x09, 0x0c, 0x03,
    0x01, 0x03, 0x88, 0x51, 0x00, 0x17, 0x00, 0x11, 0x00, 0x73, 0x6e, 0x6d,
    0x70, 0x6c, 0x6f, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e,
};

TEST(LinkNeighbor, parseCdp) {
  IOBuf iob(IOBuf::WRAP_BUFFER, basicCdpPacket, sizeof(basicCdpPacket));
  Cursor cursor(&iob);

  // Parse the data
  MacAddress destMac = PktUtil::readMac(&cursor);
  EXPECT_EQ(MacAddress("01:00:0c:cc:cc:cc"), destMac);
  MacAddress srcMac = PktUtil::readMac(&cursor);
  EXPECT_EQ(MacAddress("2c:54:2d:f5:89:3e"), srcMac);
  uint16_t ethertype = cursor.readBE<uint16_t>();
  EXPECT_EQ(295, ethertype);

  LinkNeighbor info;
  PortID srcPort{0};
  VlanID srcVlan{1};
  bool ret = info.parseCdpPdu(srcPort, srcVlan, srcMac, ethertype, &cursor);
  ASSERT_TRUE(ret);

  EXPECT_EQ(lldp::LinkProtocol::CDP, info.getProtocol());
  EXPECT_EQ(lldp::LldpChassisIdType::RESERVED, info.getChassisIdType());
  EXPECT_EQ(
      "rsw1br.07.prn2.facebook.com.facebook.com(FOC1618R10T)",
      info.getChassisId());

  EXPECT_EQ(lldp::LldpPortIdType::RESERVED, info.getPortIdType());
  EXPECT_EQ("Ethernet1/23", info.getPortId());

  EXPECT_EQ(seconds(180), info.getTTL());
  auto expire = steady_clock::now() + seconds(180);
  auto delta = expire - info.getExpirationTime();
  EXPECT_LE(delta, seconds(1));
  EXPECT_GE(delta, seconds(0));

  EXPECT_EQ("rsw1br.07.prn2.facebook.com", info.getSystemName());
}
