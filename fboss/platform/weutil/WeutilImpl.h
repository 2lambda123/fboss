// (c) Facebook, Inc. and its affiliates. Confidential and proprietary.

#pragma once
#include <gflags/gflags.h>
#include <optional>
#include "fboss/platform/weutil/WeutilInterface.h"

namespace facebook::fboss::platform {
class WeutilImpl : public WeutilInterface {
  enum entryType {
    FIELD_INVALID,
    FIELD_UINT,
    FIELD_HEX,
    FIELD_STRING,
    FIELD_MAC,
    FIELD_LEGACY_MAC,
    FIELD_DATE
  };

  typedef struct {
    int typeCode;
    std::string fieldName;
    entryType fieldType;
    std::optional<int> length;
    std::optional<int> offset;
  } EepromFieldEntry;

 private:
  const std::optional<int> VARIABLE = std::nullopt;
  static const int MAXEEPROMSIZE = 2048;
  std::vector<EepromFieldEntry> fieldDictionaryV3_;
  std::vector<EepromFieldEntry> fieldDictionaryV4_;
  void initializeFieldDictionaryV3();
  void initializeFieldDictionaryV4();
  std::string eepromPath;
  std::string translatedFrom;
  PlainWeutilConfig config_;
  int loadEeprom(
      const std::string& eeprom,
      unsigned char* output,
      int offset,
      int max);
  std::unordered_map<int, std::string> parseEepromBlobV3(
      const unsigned char* buffer);
  std::unordered_map<int, std::string> parseEepromBlobV4(
      const unsigned char* buffer,
      const int readCount);
  std::string parseUint(int len, unsigned char* ptr);
  std::string parseHex(int len, unsigned char* ptr);
  std::string parseString(int len, unsigned char* ptr);
  std::string parseMac(int len, unsigned char* ptr);
  std::string parseLegacyMac(int len, unsigned char* ptr);
  // Note that this parseDate method is for EEPROM v3 format.
  // In EEPROM v4, the date is stored as a plain string.
  std::string parseDate(int len, unsigned char* ptr);

 public:
  WeutilImpl(const std::string& eeprom = "", PlainWeutilConfig config = {});
  // This method will translate EEPROM blob to human readable format by
  // firstly translate it into <field_id, value> pair, then to
  // <key, value> pair
  std::vector<std::pair<std::string, std::string>> getInfo(
      const std::string& eeprom = "") override;
  void printInfo() override;
  void printInfoJson() override;
  bool verifyOptions(void) override;
  void printUsage(void) override;
};

} // namespace facebook::fboss::platform
