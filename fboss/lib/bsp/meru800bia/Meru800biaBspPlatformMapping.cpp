// (c) Meta Platforms, Inc. and affiliates. Confidential and proprietary.

#include "fboss/lib/bsp/meru800bia/Meru800biaBspPlatformMapping.h"
#include <thrift/lib/cpp2/protocol/Serializer.h>
#include "fboss/lib/bsp/BspPlatformMapping.h"
#include "fboss/lib/bsp/gen-cpp2/bsp_platform_mapping_types.h"

using namespace facebook::fboss;

namespace {

constexpr auto kJsonBspPlatformMappingStr = R"(
{
  "pimMapping": {
    "1": {
        "pimID": 1,
        "tcvrMapping": {
          "1": {
              "tcvrId": 1,
              "accessControl": {
                "controllerId": "1",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp1_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp1_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS1_CH0"
              },
              "tcvrLaneToLedId": {
                "1": 1,
                "2": 1,
                "3": 1,
                "4": 1,
                "5": 2,
                "6": 2,
                "7": 2,
                "8": 2
              }
          },
          "2": {
              "tcvrId": 2,
              "accessControl": {
                "controllerId": "2",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp2_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp2_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS1_CH1"
              },
              "tcvrLaneToLedId": {
                "1": 3,
                "2": 3,
                "3": 3,
                "4": 3,
                "5": 4,
                "6": 4,
                "7": 4,
                "8": 4
              }
          },
          "3": {
              "tcvrId": 3,
              "accessControl": {
                "controllerId": "3",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp3_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp3_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS1_CH2"
              },
              "tcvrLaneToLedId": {
                "1": 5,
                "2": 5,
                "3": 5,
                "4": 5,
                "5": 6,
                "6": 6,
                "7": 6,
                "8": 6
              }
          },
          "4": {
              "tcvrId": 4,
              "accessControl": {
                "controllerId": "4",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp4_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp4_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS1_CH3"
              },
              "tcvrLaneToLedId": {
                "1": 7,
                "2": 7,
                "3": 7,
                "4": 7,
                "5": 8,
                "6": 8,
                "7": 8,
                "8": 8
              }
          },
          "5": {
              "tcvrId": 5,
              "accessControl": {
                "controllerId": "5",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp5_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp5_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS1_CH4"
              },
              "tcvrLaneToLedId": {
                "1": 9,
                "2": 9,
                "3": 9,
                "4": 9,
                "5": 10,
                "6": 10,
                "7": 10,
                "8": 10
              }
          },
          "6": {
              "tcvrId": 6,
              "accessControl": {
                "controllerId": "6",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp6_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp6_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS1_CH5"
              },
              "tcvrLaneToLedId": {
                "1": 11,
                "2": 11,
                "3": 11,
                "4": 11,
                "5": 12,
                "6": 12,
                "7": 12,
                "8": 12
              }
          },
          "7": {
              "tcvrId": 7,
              "accessControl": {
                "controllerId": "7",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp7_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp7_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS1_CH6"
              },
              "tcvrLaneToLedId": {
                "1": 13,
                "2": 13,
                "3": 13,
                "4": 13,
                "5": 14,
                "6": 14,
                "7": 14,
                "8": 14
              }
          },
          "8": {
              "tcvrId": 8,
              "accessControl": {
                "controllerId": "8",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp8_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp8_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS1_CH7"
              },
              "tcvrLaneToLedId": {
                "1": 15,
                "2": 15,
                "3": 15,
                "4": 15,
                "5": 16,
                "6": 16,
                "7": 16,
                "8": 16
              }
          },
          "9": {
              "tcvrId": 9,
              "accessControl": {
                "controllerId": "9",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp9_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp9_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS2_CH0"
              },
              "tcvrLaneToLedId": {
                "1": 17,
                "2": 17,
                "3": 17,
                "4": 17,
                "5": 18,
                "6": 18,
                "7": 18,
                "8": 18
              }
          },
          "10": {
              "tcvrId": 10,
              "accessControl": {
                "controllerId": "10",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp10_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp10_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS2_CH1"
              },
              "tcvrLaneToLedId": {
                "1": 19,
                "2": 19,
                "3": 19,
                "4": 19,
                "5": 20,
                "6": 20,
                "7": 20,
                "8": 20
              }
          },
          "11": {
              "tcvrId": 11,
              "accessControl": {
                "controllerId": "11",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp11_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp11_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS2_CH2"
              },
              "tcvrLaneToLedId": {
                "1": 21,
                "2": 21,
                "3": 21,
                "4": 21,
                "5": 22,
                "6": 22,
                "7": 22,
                "8": 22
              }
          },
          "12": {
              "tcvrId": 12,
              "accessControl": {
                "controllerId": "12",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp12_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp12_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS2_CH3"
              },
              "tcvrLaneToLedId": {
                "1": 23,
                "2": 23,
                "3": 23,
                "4": 23,
                "5": 24,
                "6": 24,
                "7": 24,
                "8": 24
              }
          },
          "13": {
              "tcvrId": 13,
              "accessControl": {
                "controllerId": "13",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp13_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp13_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS2_CH4"
              },
              "tcvrLaneToLedId": {
                "1": 25,
                "2": 25,
                "3": 25,
                "4": 25,
                "5": 26,
                "6": 26,
                "7": 26,
                "8": 26
              }
          },
          "14": {
              "tcvrId": 14,
              "accessControl": {
                "controllerId": "14",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp14_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp14_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS2_CH5"
              },
              "tcvrLaneToLedId": {
                "1": 27,
                "2": 27,
                "3": 27,
                "4": 27,
                "5": 28,
                "6": 28,
                "7": 28,
                "8": 28
              }
          },
          "15": {
              "tcvrId": 15,
              "accessControl": {
                "controllerId": "15",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp15_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp15_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS2_CH6"
              },
              "tcvrLaneToLedId": {
                "1": 29,
                "2": 29,
                "3": 29,
                "4": 29,
                "5": 30,
                "6": 30,
                "7": 30,
                "8": 30
              }
          },
          "16": {
              "tcvrId": 16,
              "accessControl": {
                "controllerId": "16",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp16_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp16_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS2_CH7"
              },
              "tcvrLaneToLedId": {
                "1": 31,
                "2": 31,
                "3": 31,
                "4": 31,
                "5": 32,
                "6": 32,
                "7": 32,
                "8": 32
              }
          },
          "17": {
              "tcvrId": 17,
              "accessControl": {
                "controllerId": "17",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp17_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp17_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS3_CH0"
              },
              "tcvrLaneToLedId": {
                "1": 33,
                "2": 33,
                "3": 33,
                "4": 33,
                "5": 34,
                "6": 34,
                "7": 34,
                "8": 34
              }
          },
          "18": {
              "tcvrId": 18,
              "accessControl": {
                "controllerId": "18",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp18_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp18_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS3_CH1"
              },
              "tcvrLaneToLedId": {
                "1": 35,
                "2": 35,
                "3": 35,
                "4": 35,
                "5": 36,
                "6": 36,
                "7": 36,
                "8": 36
              }
          },
          "19": {
              "tcvrId": 19,
              "accessControl": {
                "controllerId": "19",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp19_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp19_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS3_CH2"
              },
              "tcvrLaneToLedId": {
                "1": 37,
                "2": 37,
                "3": 37,
                "4": 37,
                "5": 38,
                "6": 38,
                "7": 38,
                "8": 38
              }
          },
          "20": {
              "tcvrId": 20,
              "accessControl": {
                "controllerId": "20",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp20_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp20_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS3_CH3"
              },
              "tcvrLaneToLedId": {
                "1": 39,
                "2": 39,
                "3": 39,
                "4": 39,
                "5": 40,
                "6": 40,
                "7": 40,
                "8": 40
              }
          },
          "21": {
              "tcvrId": 21,
              "accessControl": {
                "controllerId": "21",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp21_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp21_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS3_CH4"
              },
              "tcvrLaneToLedId": {
                "1": 41,
                "2": 41,
                "3": 41,
                "4": 41,
                "5": 42,
                "6": 42,
                "7": 42,
                "8": 42
              }
          },
          "22": {
              "tcvrId": 22,
              "accessControl": {
                "controllerId": "22",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp22_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp22_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS3_CH5"
              },
              "tcvrLaneToLedId": {
                "1": 43,
                "2": 43,
                "3": 43,
                "4": 43,
                "5": 44,
                "6": 44,
                "7": 44,
                "8": 44
              }
          },
          "23": {
              "tcvrId": 23,
              "accessControl": {
                "controllerId": "23",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp23_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp23_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS3_CH6"
              },
              "tcvrLaneToLedId": {
                "1": 45,
                "2": 45,
                "3": 45,
                "4": 45,
                "5": 46,
                "6": 46,
                "7": 46,
                "8": 46
              }
          },
          "24": {
              "tcvrId": 24,
              "accessControl": {
                "controllerId": "24",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp24_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp24_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS3_CH7"
              },
              "tcvrLaneToLedId": {
                "1": 47,
                "2": 47,
                "3": 47,
                "4": 47,
                "5": 48,
                "6": 48,
                "7": 48,
                "8": 48
              }
          },
          "25": {
              "tcvrId": 25,
              "accessControl": {
                "controllerId": "25",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp25_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp25_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS4_CH0"
              },
              "tcvrLaneToLedId": {
                "1": 49,
                "2": 49,
                "3": 49,
                "4": 49,
                "5": 50,
                "6": 50,
                "7": 50,
                "8": 50
              }
          },
          "26": {
              "tcvrId": 26,
              "accessControl": {
                "controllerId": "26",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp26_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp26_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS4_CH1"
              },
              "tcvrLaneToLedId": {
                "1": 51,
                "2": 51,
                "3": 51,
                "4": 51,
                "5": 52,
                "6": 52,
                "7": 52,
                "8": 52
              }
          },
          "27": {
              "tcvrId": 27,
              "accessControl": {
                "controllerId": "27",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp27_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp27_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS4_CH2"
              },
              "tcvrLaneToLedId": {
                "1": 53,
                "2": 53,
                "3": 53,
                "4": 53,
                "5": 54,
                "6": 54,
                "7": 54,
                "8": 54
              }
          },
          "28": {
              "tcvrId": 28,
              "accessControl": {
                "controllerId": "28",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp28_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp28_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS4_CH3"
              },
              "tcvrLaneToLedId": {
                "1": 55,
                "2": 55,
                "3": 55,
                "4": 55,
                "5": 56,
                "6": 56,
                "7": 56,
                "8": 56
              }
          },
          "29": {
              "tcvrId": 29,
              "accessControl": {
                "controllerId": "29",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp29_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp29_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS4_CH4"
              },
              "tcvrLaneToLedId": {
                "1": 57,
                "2": 57,
                "3": 57,
                "4": 57,
                "5": 58,
                "6": 58,
                "7": 58,
                "8": 58
              }
          },
          "30": {
              "tcvrId": 30,
              "accessControl": {
                "controllerId": "30",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp30_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp30_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS4_CH5"
              },
              "tcvrLaneToLedId": {
                "1": 59,
                "2": 59,
                "3": 59,
                "4": 59,
                "5": 60,
                "6": 60,
                "7": 60,
                "8": 60
              }
          },
          "31": {
              "tcvrId": 31,
              "accessControl": {
                "controllerId": "31",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp31_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp31_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS4_CH6"
              },
              "tcvrLaneToLedId": {
                "1": 61,
                "2": 61,
                "3": 61,
                "4": 61,
                "5": 62,
                "6": 62,
                "7": 62,
                "8": 62
              }
          },
          "32": {
              "tcvrId": 32,
              "accessControl": {
                "controllerId": "32",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp32_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp32_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS4_CH7"
              },
              "tcvrLaneToLedId": {
                "1": 63,
                "2": 63,
                "3": 63,
                "4": 63,
                "5": 64,
                "6": 64,
                "7": 64,
                "8": 64
              }
          },
          "33": {
              "tcvrId": 33,
              "accessControl": {
                "controllerId": "33",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp33_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp33_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS5_CH0"
              },
              "tcvrLaneToLedId": {
                "1": 65,
                "2": 65,
                "3": 65,
                "4": 65,
                "5": 66,
                "6": 66,
                "7": 66,
                "8": 66
              }
          },
          "34": {
              "tcvrId": 34,
              "accessControl": {
                "controllerId": "34",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp34_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp34_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS5_CH1"
              },
              "tcvrLaneToLedId": {
                "1": 67,
                "2": 67,
                "3": 67,
                "4": 67,
                "5": 68,
                "6": 68,
                "7": 68,
                "8": 68
              }
          },
          "35": {
              "tcvrId": 35,
              "accessControl": {
                "controllerId": "35",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp35_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp35_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS5_CH2"
              },
              "tcvrLaneToLedId": {
                "1": 69,
                "2": 69,
                "3": 69,
                "4": 69,
                "5": 70,
                "6": 70,
                "7": 70,
                "8": 70
              }
          },
          "36": {
              "tcvrId": 36,
              "accessControl": {
                "controllerId": "36",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp36_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp36_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS5_CH3"
              },
              "tcvrLaneToLedId": {
                "1": 71,
                "2": 71,
                "3": 71,
                "4": 71,
                "5": 72,
                "6": 72,
                "7": 72,
                "8": 72
              }
          },
          "37": {
              "tcvrId": 37,
              "accessControl": {
                "controllerId": "37",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp37_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp37_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS5_CH4"
              },
              "tcvrLaneToLedId": {
                "1": 73,
                "2": 73,
                "3": 73,
                "4": 73,
                "5": 74,
                "6": 74,
                "7": 74,
                "8": 74
              }
          },
          "38": {
              "tcvrId": 38,
              "accessControl": {
                "controllerId": "38",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp38_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/osfp38_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS5_CH5"
              },
              "tcvrLaneToLedId": {
                "1": 75,
                "2": 75,
                "3": 75,
                "4": 75,
                "5": 76,
                "6": 76,
                "7": 76,
                "8": 76
              }
          },
          "39": {
              "tcvrId": 39,
              "accessControl": {
                "controllerId": "39",
                "type": 2,
                "reset": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/qsfp1_reset",
                  "mask": 1,
                  "gpioOffset": 0,
                  "resetHoldHi": 0
                },
                "presence": {
                  "sysfsPath": "/run/devmap/fpgas/MERU800BIA_SMB_FPGA/scd-xcvr.0/qsfp1_present",
                  "mask": 1,
                  "gpioOffset": 0,
                  "presentHoldHi": 1
                },
                "gpioChip": ""
              },
              "io": {
                "controllerId": "1",
                "type": 1,
                "devicePath": "/run/devmap/i2c-busses/MERU800BIA_SMB_FPGA_SMBUS0_CH4"
              },
              "tcvrLaneToLedId": {
                "1": 77,
                "2": 78,
                "3": 79,
                "4": 80
              }
          }
        },
        "phyMapping": {

        },
        "phyIOControllers": {

        },
        "ledMapping": {
          "1": {
              "id": 1,
              "bluePath": "/sys/class/leds/osfp_port1_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port1_led1:amber:status/brightness",
              "transceiverId": 1
          },
          "2": {
              "id": 2,
              "bluePath": "/sys/class/leds/osfp_port1_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port1_led2:amber:status/brightness",
              "transceiverId": 1
          },
          "3": {
              "id": 3,
              "bluePath": "/sys/class/leds/osfp_port2_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port2_led1:amber:status/brightness",
              "transceiverId": 2
          },
          "4": {
              "id": 4,
              "bluePath": "/sys/class/leds/osfp_port2_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port2_led2:amber:status/brightness",
              "transceiverId": 2
          },
          "5": {
              "id": 5,
              "bluePath": "/sys/class/leds/osfp_port3_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port3_led1:amber:status/brightness",
              "transceiverId": 3
          },
          "6": {
              "id": 6,
              "bluePath": "/sys/class/leds/osfp_port3_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port3_led2:amber:status/brightness",
              "transceiverId": 3
          },
          "7": {
              "id": 7,
              "bluePath": "/sys/class/leds/osfp_port4_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port4_led1:amber:status/brightness",
              "transceiverId": 4
          },
          "8": {
              "id": 8,
              "bluePath": "/sys/class/leds/osfp_port4_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port4_led2:amber:status/brightness",
              "transceiverId": 4
          },
          "9": {
              "id": 9,
              "bluePath": "/sys/class/leds/osfp_port5_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port5_led1:amber:status/brightness",
              "transceiverId": 5
          },
          "10": {
              "id": 10,
              "bluePath": "/sys/class/leds/osfp_port5_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port5_led2:amber:status/brightness",
              "transceiverId": 5
          },
          "11": {
              "id": 11,
              "bluePath": "/sys/class/leds/osfp_port6_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port6_led1:amber:status/brightness",
              "transceiverId": 6
          },
          "12": {
              "id": 12,
              "bluePath": "/sys/class/leds/osfp_port6_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port6_led2:amber:status/brightness",
              "transceiverId": 6
          },
          "13": {
              "id": 13,
              "bluePath": "/sys/class/leds/osfp_port7_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port7_led1:amber:status/brightness",
              "transceiverId": 7
          },
          "14": {
              "id": 14,
              "bluePath": "/sys/class/leds/osfp_port7_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port7_led2:amber:status/brightness",
              "transceiverId": 7
          },
          "15": {
              "id": 15,
              "bluePath": "/sys/class/leds/osfp_port8_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port8_led1:amber:status/brightness",
              "transceiverId": 8
          },
          "16": {
              "id": 16,
              "bluePath": "/sys/class/leds/osfp_port8_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port8_led2:amber:status/brightness",
              "transceiverId": 8
          },
          "17": {
              "id": 17,
              "bluePath": "/sys/class/leds/osfp_port9_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port9_led1:amber:status/brightness",
              "transceiverId": 9
          },
          "18": {
              "id": 18,
              "bluePath": "/sys/class/leds/osfp_port9_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port9_led2:amber:status/brightness",
              "transceiverId": 9
          },
          "19": {
              "id": 19,
              "bluePath": "/sys/class/leds/osfp_port10_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port10_led1:amber:status/brightness",
              "transceiverId": 10
          },
          "20": {
              "id": 20,
              "bluePath": "/sys/class/leds/osfp_port10_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port10_led2:amber:status/brightness",
              "transceiverId": 10
          },
          "21": {
              "id": 21,
              "bluePath": "/sys/class/leds/osfp_port11_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port11_led1:amber:status/brightness",
              "transceiverId": 11
          },
          "22": {
              "id": 22,
              "bluePath": "/sys/class/leds/osfp_port11_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port11_led2:amber:status/brightness",
              "transceiverId": 11
          },
          "23": {
              "id": 23,
              "bluePath": "/sys/class/leds/osfp_port12_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port12_led1:amber:status/brightness",
              "transceiverId": 12
          },
          "24": {
              "id": 24,
              "bluePath": "/sys/class/leds/osfp_port12_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port12_led2:amber:status/brightness",
              "transceiverId": 12
          },
          "25": {
              "id": 25,
              "bluePath": "/sys/class/leds/osfp_port13_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port13_led1:amber:status/brightness",
              "transceiverId": 13
          },
          "26": {
              "id": 26,
              "bluePath": "/sys/class/leds/osfp_port13_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port13_led2:amber:status/brightness",
              "transceiverId": 13
          },
          "27": {
              "id": 27,
              "bluePath": "/sys/class/leds/osfp_port14_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port14_led1:amber:status/brightness",
              "transceiverId": 14
          },
          "28": {
              "id": 28,
              "bluePath": "/sys/class/leds/osfp_port14_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port14_led2:amber:status/brightness",
              "transceiverId": 14
          },
          "29": {
              "id": 29,
              "bluePath": "/sys/class/leds/osfp_port15_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port15_led1:amber:status/brightness",
              "transceiverId": 15
          },
          "30": {
              "id": 30,
              "bluePath": "/sys/class/leds/osfp_port15_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port15_led2:amber:status/brightness",
              "transceiverId": 15
          },
          "31": {
              "id": 31,
              "bluePath": "/sys/class/leds/osfp_port16_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port16_led1:amber:status/brightness",
              "transceiverId": 16
          },
          "32": {
              "id": 32,
              "bluePath": "/sys/class/leds/osfp_port16_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port16_led2:amber:status/brightness",
              "transceiverId": 16
          },
          "33": {
              "id": 33,
              "bluePath": "/sys/class/leds/osfp_port17_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port17_led1:amber:status/brightness",
              "transceiverId": 17
          },
          "34": {
              "id": 34,
              "bluePath": "/sys/class/leds/osfp_port17_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port17_led2:amber:status/brightness",
              "transceiverId": 17
          },
          "35": {
              "id": 35,
              "bluePath": "/sys/class/leds/osfp_port18_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port18_led1:amber:status/brightness",
              "transceiverId": 18
          },
          "36": {
              "id": 36,
              "bluePath": "/sys/class/leds/osfp_port18_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port18_led2:amber:status/brightness",
              "transceiverId": 18
          },
          "37": {
              "id": 37,
              "bluePath": "/sys/class/leds/osfp_port19_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port19_led1:amber:status/brightness",
              "transceiverId": 19
          },
          "38": {
              "id": 38,
              "bluePath": "/sys/class/leds/osfp_port19_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port19_led2:amber:status/brightness",
              "transceiverId": 19
          },
          "39": {
              "id": 39,
              "bluePath": "/sys/class/leds/osfp_port20_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port20_led1:amber:status/brightness",
              "transceiverId": 20
          },
          "40": {
              "id": 40,
              "bluePath": "/sys/class/leds/osfp_port20_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port20_led2:amber:status/brightness",
              "transceiverId": 20
          },
          "41": {
              "id": 41,
              "bluePath": "/sys/class/leds/osfp_port21_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port21_led1:amber:status/brightness",
              "transceiverId": 21
          },
          "42": {
              "id": 42,
              "bluePath": "/sys/class/leds/osfp_port21_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port21_led2:amber:status/brightness",
              "transceiverId": 21
          },
          "43": {
              "id": 43,
              "bluePath": "/sys/class/leds/osfp_port22_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port22_led1:amber:status/brightness",
              "transceiverId": 22
          },
          "44": {
              "id": 44,
              "bluePath": "/sys/class/leds/osfp_port22_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port22_led2:amber:status/brightness",
              "transceiverId": 22
          },
          "45": {
              "id": 45,
              "bluePath": "/sys/class/leds/osfp_port23_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port23_led1:amber:status/brightness",
              "transceiverId": 23
          },
          "46": {
              "id": 46,
              "bluePath": "/sys/class/leds/osfp_port23_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port23_led2:amber:status/brightness",
              "transceiverId": 23
          },
          "47": {
              "id": 47,
              "bluePath": "/sys/class/leds/osfp_port24_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port24_led1:amber:status/brightness",
              "transceiverId": 24
          },
          "48": {
              "id": 48,
              "bluePath": "/sys/class/leds/osfp_port24_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port24_led2:amber:status/brightness",
              "transceiverId": 24
          },
          "49": {
              "id": 49,
              "bluePath": "/sys/class/leds/osfp_port25_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port25_led1:amber:status/brightness",
              "transceiverId": 25
          },
          "50": {
              "id": 50,
              "bluePath": "/sys/class/leds/osfp_port25_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port25_led2:amber:status/brightness",
              "transceiverId": 25
          },
          "51": {
              "id": 51,
              "bluePath": "/sys/class/leds/osfp_port26_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port26_led1:amber:status/brightness",
              "transceiverId": 26
          },
          "52": {
              "id": 52,
              "bluePath": "/sys/class/leds/osfp_port26_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port26_led2:amber:status/brightness",
              "transceiverId": 26
          },
          "53": {
              "id": 53,
              "bluePath": "/sys/class/leds/osfp_port27_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port27_led1:amber:status/brightness",
              "transceiverId": 27
          },
          "54": {
              "id": 54,
              "bluePath": "/sys/class/leds/osfp_port27_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port27_led2:amber:status/brightness",
              "transceiverId": 27
          },
          "55": {
              "id": 55,
              "bluePath": "/sys/class/leds/osfp_port28_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port28_led1:amber:status/brightness",
              "transceiverId": 28
          },
          "56": {
              "id": 56,
              "bluePath": "/sys/class/leds/osfp_port28_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port28_led2:amber:status/brightness",
              "transceiverId": 28
          },
          "57": {
              "id": 57,
              "bluePath": "/sys/class/leds/osfp_port29_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port29_led1:amber:status/brightness",
              "transceiverId": 29
          },
          "58": {
              "id": 58,
              "bluePath": "/sys/class/leds/osfp_port29_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port29_led2:amber:status/brightness",
              "transceiverId": 29
          },
          "59": {
              "id": 59,
              "bluePath": "/sys/class/leds/osfp_port30_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port30_led1:amber:status/brightness",
              "transceiverId": 30
          },
          "60": {
              "id": 60,
              "bluePath": "/sys/class/leds/osfp_port30_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port30_led2:amber:status/brightness",
              "transceiverId": 30
          },
          "61": {
              "id": 61,
              "bluePath": "/sys/class/leds/osfp_port31_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port31_led1:amber:status/brightness",
              "transceiverId": 31
          },
          "62": {
              "id": 62,
              "bluePath": "/sys/class/leds/osfp_port31_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port31_led2:amber:status/brightness",
              "transceiverId": 31
          },
          "63": {
              "id": 63,
              "bluePath": "/sys/class/leds/osfp_port32_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port32_led1:amber:status/brightness",
              "transceiverId": 32
          },
          "64": {
              "id": 64,
              "bluePath": "/sys/class/leds/osfp_port32_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port32_led2:amber:status/brightness",
              "transceiverId": 32
          },
          "65": {
              "id": 65,
              "bluePath": "/sys/class/leds/osfp_port33_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port33_led1:amber:status/brightness",
              "transceiverId": 33
          },
          "66": {
              "id": 66,
              "bluePath": "/sys/class/leds/osfp_port33_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port33_led2:amber:status/brightness",
              "transceiverId": 33
          },
          "67": {
              "id": 67,
              "bluePath": "/sys/class/leds/osfp_port34_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port34_led1:amber:status/brightness",
              "transceiverId": 34
          },
          "68": {
              "id": 68,
              "bluePath": "/sys/class/leds/osfp_port34_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port34_led2:amber:status/brightness",
              "transceiverId": 34
          },
          "69": {
              "id": 69,
              "bluePath": "/sys/class/leds/osfp_port35_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port35_led1:amber:status/brightness",
              "transceiverId": 35
          },
          "70": {
              "id": 70,
              "bluePath": "/sys/class/leds/osfp_port35_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port35_led2:amber:status/brightness",
              "transceiverId": 35
          },
          "71": {
              "id": 71,
              "bluePath": "/sys/class/leds/osfp_port36_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port36_led1:amber:status/brightness",
              "transceiverId": 36
          },
          "72": {
              "id": 72,
              "bluePath": "/sys/class/leds/osfp_port36_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port36_led2:amber:status/brightness",
              "transceiverId": 36
          },
          "73": {
              "id": 73,
              "bluePath": "/sys/class/leds/osfp_port37_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port37_led1:amber:status/brightness",
              "transceiverId": 37
          },
          "74": {
              "id": 74,
              "bluePath": "/sys/class/leds/osfp_port37_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port37_led2:amber:status/brightness",
              "transceiverId": 37
          },
          "75": {
              "id": 75,
              "bluePath": "/sys/class/leds/osfp_port38_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port38_led1:amber:status/brightness",
              "transceiverId": 38
          },
          "76": {
              "id": 76,
              "bluePath": "/sys/class/leds/osfp_port38_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/osfp_port38_led2:amber:status/brightness",
              "transceiverId": 38
          },
          "77": {
              "id": 77,
              "bluePath": "/sys/class/leds/qsfp_port1_led1:blue:status/brightness",
              "yellowPath": "/sys/class/leds/qsfp_port1_led1:amber:status/brightness",
              "transceiverId": 39
          },
          "78": {
              "id": 78,
              "bluePath": "/sys/class/leds/qsfp_port1_led2:blue:status/brightness",
              "yellowPath": "/sys/class/leds/qsfp_port1_led2:amber:status/brightness",
              "transceiverId": 39
          },
          "79": {
              "id": 79,
              "bluePath": "/sys/class/leds/qsfp_port1_led3:blue:status/brightness",
              "yellowPath": "/sys/class/leds/qsfp_port1_led3:amber:status/brightness",
              "transceiverId": 39
          },
          "80": {
              "id": 80,
              "bluePath": "/sys/class/leds/qsfp_port1_led4:blue:status/brightness",
              "yellowPath": "/sys/class/leds/qsfp_port1_led4:amber:status/brightness",
              "transceiverId": 39
          }
        }
    }
  }
}
)";

static BspPlatformMappingThrift buildMeru800biaPlatformMapping() {
  return apache::thrift::SimpleJSONSerializer::deserialize<
      BspPlatformMappingThrift>(kJsonBspPlatformMappingStr);
}

} // namespace

namespace facebook {
namespace fboss {

Meru800biaBspPlatformMapping::Meru800biaBspPlatformMapping()
    : BspPlatformMapping(buildMeru800biaPlatformMapping()) {}

} // namespace fboss
} // namespace facebook
