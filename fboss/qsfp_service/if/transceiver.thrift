namespace cpp2 facebook.fboss
namespace go neteng.fboss.transceiver
namespace php fboss
namespace py neteng.fboss.transceiver
namespace py3 neteng.fboss
namespace py.asyncio neteng.fboss.asyncio.transceiver
namespace rust facebook.fboss

struct Vendor {
  1: string name,
  2: binary oui,
  3: string partNumber,
  4: string rev,
  5: string serialNumber,
  6: string dateCode,
}

struct Thresholds {
  1: double low,
  2: double high
}

struct ThresholdLevels {
  1: Thresholds alarm,
  2: Thresholds warn
}

struct AlarmThreshold {
  1: ThresholdLevels temp,
  2: ThresholdLevels vcc,
  3: ThresholdLevels rxPwr,
  4: ThresholdLevels txBias,
  5: ThresholdLevels txPwr,
}

struct Flags {
  1: bool high,
  2: bool low,
}

struct FlagLevels {
  1: Flags alarm,
  2: Flags warn,
}

struct Sensor {
  1: double value,
  2: optional FlagLevels flags,
}

struct GlobalSensors {
  1: Sensor temp,
  2: Sensor vcc,
}

struct ChannelSensors {
  1: Sensor rxPwr,
  2: Sensor txBias,
  3: Sensor txPwr,
  4: optional Sensor txSnr,
  5: optional Sensor rxSnr,
  6: optional Sensor rxPwrdBm,
  7: optional Sensor txPwrdBm,
}

struct SignalFlags {
  1: i32 txLos,
  2: i32 rxLos,
  3: i32 txLol,
  4: i32 rxLol,
}

enum TransmitterTechnology {
  UNKNOWN = 0,
  COPPER = 1,
  OPTICAL = 2,
  BACKPLANE = 3,
}
/*
 * QSFP and SFP units specify length as a byte;  a value of 255 indicates
 * that the cable is longer than can be represented.  Each has a different
 * scaling factor for different lengths.  We represent "longer than
 * can be represented" with a negative integer value of the appropriate
 * "max length" size -- i.e. -255000 for more than 255 km on a single
 * mode fiber.
 */
struct Cable {
  1: optional i32 singleModeKm,
  2: optional i32 singleMode,
  3: optional i32 om3,
  4: optional i32 om2,
  5: optional i32 om1,
  6: optional i32 copper,
  7: TransmitterTechnology transmitterTech,
  8: optional double length,
  9: optional i32 gauge,
  10: optional i32 om4,
  11: optional i32 om5,
}

struct Channel {
  1: i32 channel,
  6: ChannelSensors sensors,
}

enum TransceiverType {
  SFP = 0,
  QSFP = 1,
}

enum TransceiverManagementInterface {
  SFF = 0,
  CMIS = 1,
  NONE = 2,
}

enum FeatureState {
  UNSUPPORTED = 0,
  ENABLED = 1,
  DISABLED = 2,
}

enum PowerControlState {
  POWER_LPMODE = 0,
  POWER_OVERRIDE = 1,
  POWER_SET = 2, // Deprecated!
  HIGH_POWER_OVERRIDE = 3,
  POWER_SET_BY_HW = 4,
}

enum RateSelectState {
  UNSUPPORTED = 0,
  // Depending on which of the rate selects are set, the Extended Rate
  // Compliance bits are read differently:
  // ftp://ftp.seagate.com/sff/SFF-8636.PDF page 36
  EXTENDED_RATE_SELECT_V1 = 1,
  EXTENDED_RATE_SELECT_V2 = 2,
  APPLICATION_RATE_SELECT = 3,
}

enum RateSelectSetting {
  LESS_THAN_2_2GB = 0,
  FROM_2_2GB_TO_6_6GB = 1,
  FROM_6_6GB_AND_ABOVE = 2,
  LESS_THAN_12GB = 3,
  FROM_12GB_TO_24GB = 4,
  FROM_24GB_to_26GB = 5,
  FROM_26GB_AND_ABOVE = 6,
  UNSUPPORTED = 7,
  UNKNOWN = 8,
}

// The extended specification compliance code of the transceiver module.
// This is the field of Byte 192 on page00 and following table 4-4
// Extended Specification Compliance Codes of SFF-8024.
enum ExtendedSpecComplianceCode {
  UNKNOWN = 0,
  CWDM4_100G = 6,
  CR4_100G = 11,
  FR1_100G = 38,
}

// Transceiver identifier as read from module page 0 reg 0
enum TransceiverModuleIdentifier {
  UNKNOWN = 0,
  QSFP = 0xC,
  QSFP_PLUS = 0xD,
  QSFP28 = 0x11,
  QSFP_DD = 0x18,
  QSFP_PLUS_CMIS = 0x1E,
  MINIPHOTON_OBO = 0x5B,
}

enum CmisModuleState {
  LOW_POWER = 0x1,
  POWERING_UP = 0x2,
  READY = 0x3,
  POWERING_DOWN = 0x4,
  FAULT = 0x5,
}

enum SMFMediaInterfaceCode {
  CWDM4_100G = 0x10,
  FR4_200G = 0x18,
  FR4_400G = 0x1D,
}

union MediaInterfaceUnion {
  1: SMFMediaInterfaceCode smfCode
}

enum MediaTypeEncodings {
  UNDEFINED = 0x0,
  OPTICAL_MMF = 0x1,
  OPTICAL_SMF = 0x2,
  PASSIVE_CU = 0x3,
  ACTIVE_CABLES = 0x4
}

struct MediaInterfaceId {
  1: i32 lane,
  2: MediaInterfaceUnion media,
}

enum CmisLaneState {
  DEACTIVATED = 0x1,
  DATAPATHINIT = 0x2,
  DEINIT = 0x3,
  ACTIVATED = 0x4,
  TX_ON = 0x5,
  TX_OFF = 0x6,
  DATAPATH_INITIALIZED = 0x7,
}

struct FirmwareStatus {
  1: optional string version,
  2: optional i32 fwFault,
  3: optional string dspFwVer,
  4: optional string buildRev,
}

struct MediaLaneSettings {
  1: i32 lane,
  2: optional bool txDisable,
  3: optional bool txSquelch,
  4: optional bool txAdaptiveEqControl, // Only applicable for Sff
  5: optional bool txSquelchForce, // Only applicable for Cmis
}

struct HostLaneSettings {
  1: i32 lane,
  2: optional i32 txInputEqualization,
  3: optional i32 rxOutputEmphasis,
  4: optional i32 rxOutputAmplitude,
  5: optional bool rxOutput,
  6: optional bool rxSquelch,
}

struct MediaLaneSignals {
  1: i32 lane,
  2: optional bool txLos,
  3: optional bool rxLos,
  4: optional bool txLol,
  5: optional bool rxLol,
  6: optional bool txFault,
  7: optional bool txAdaptEqFault,
}

struct HostLaneSignals {
  1: i32 lane,
  2: optional bool dataPathDeInit,
  3: optional CmisLaneState cmisLaneState,
}

struct TransceiverSettings {
  1: FeatureState cdrTx,
  2: FeatureState cdrRx,
  3: RateSelectState rateSelect,
  4: FeatureState powerMeasurement,
  5: PowerControlState powerControl,
  6: RateSelectSetting rateSelectSetting,
  7: optional list<MediaLaneSettings> mediaLaneSettings,
  8: optional list<HostLaneSettings> hostLaneSettings,
  9: optional list<MediaInterfaceId> mediaInterface,
}

// maintained and populated by qsfp service
struct TransceiverStats {
  // duration between last read and last successful read
  1: double readDownTime,
  // duration between last write and last successful write
  2: double writeDownTime,
}

struct ModuleStatus {
  1: optional bool dataNotReady,
  2: optional bool interruptL,
  3: optional CmisModuleState cmisModuleState,
  4: optional FirmwareStatus fwStatus,
}

struct TransceiverInfo {
  1: bool present,
  2: TransceiverType transceiver,
  3: i32 port,  // physical port number
  4: optional GlobalSensors sensor,
  5: optional AlarmThreshold thresholds,
  9: optional Vendor vendor,
  10: optional Cable cable,
  12: list<Channel> channels,
  13: optional TransceiverSettings settings,
  14: optional TransceiverStats stats,
  15: optional SignalFlags signalFlag,
  16: optional ExtendedSpecComplianceCode extendedSpecificationComplianceCode,
  17: optional TransceiverManagementInterface transceiverManagementInterface,
  18: optional TransceiverModuleIdentifier identifier,
  19: optional ModuleStatus status,
  20: optional list<MediaLaneSignals> mediaLaneSignals,
  21: optional list<HostLaneSignals> hostLaneSignals,
  22: optional i64 timeCollected,
}

typedef binary (cpp2.type = "folly::IOBuf") IOBuf

struct RawDOMData {
  // The SFF DOM exposes at most 256 bytes at a time and is divided in
  // to two 128 byte "pages". The lower page is always the same, but
  // you can swap in different pages for the upper 128 bytes. The only
  // ones we use now are upper pages 0 and 3. Page 0 is required of
  // every transceiver, but the rest are optional. If we need other
  // fields in the future we can add support for other pages.
  // Added page10 and page11 for cmis optics
  1: IOBuf lower,
  2: IOBuf page0,
  3: optional IOBuf page3,
  4: optional IOBuf page10,
  5: optional IOBuf page11,
}

// Create a union data structure where we can store SffData and
// CMISData as well. After this been fully deployed we can remove the
// old RawDOMData structure.

union DOMDataUnion {
   1: Sff8636Data sff8636,
   2: CmisData cmis,
 }

struct Sff8636Data {
  // The SFF DOM exposes at most 256 bytes at a time and is divided in
  // to two 128 byte "pages". The lower page is always the same, but
  // you can swap in different pages for the upper 128 bytes. The only
  // ones we use now are upper pages 0 and 3. Page 0 is required of
  // every transceiver, but the rest are optional. If we need other
  // fields in the future we can add support for other pages.
  1: IOBuf lower,
  2: IOBuf page0,
  3: optional IOBuf page3,
  4: optional i64 timeCollected,
}

struct CmisData {
  // Similar to SFF Data format, 256 bytes are exposed at a time and is
  // also divided into two 128 byte pages. Lower page stay the same but
  // we have more upper pages this time.
  1: IOBuf lower,
  2: IOBuf page0,
  4: optional IOBuf page01,
  5: optional IOBuf page02,
  6: optional IOBuf page10,
  7: optional IOBuf page11,
  8: optional IOBuf page13,
  9: optional IOBuf page14,
  10: optional i64 timeCollected,
}

struct TransceiverIOParameters {
  1: i32 offset,  // should range from 0 - 255
  2: optional i32 page, // can be used to access bytes 128-255 from a different page than page0
  3: optional i32 length, // Number of bytes to read. Not applicable for a write
}

struct ReadRequest {
  1: list<i32> ids,
  2: TransceiverIOParameters parameter,
}

struct ReadResponse {
  1: bool valid,
  2: IOBuf data,
}

struct WriteRequest {
  1: list<i32> ids,
  2: TransceiverIOParameters parameter,
  3: byte data, // The data to write for a write request
}

struct WriteResponse {
  1: bool success,
}
