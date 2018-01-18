const unsigned short RESERVED = 0x00;
const unsigned short STC_TIMEOUT = 100;

enum CMD {
  POWER_UP = 0x01,
  GET_REV = 0x10,
  SET_PROPERTY = 0x12,
  GET_INT_STATUS = 0x14,
  TX_TUNE_FREQ = 0x30,
  TX_TUNE_POWER = 0x31,
  TX_TUNE_STATUS = 0x33
};

enum STATUS {
  CTS = 0x80,
  ERR = 0x40,
  STC = 0x81
};

namespace ARGS {
  enum POWER_UP {
    TX_MODE = 0x12,
    OP_MODE = 0x50
  };

  enum TX_TUNE_STATUS {
    INTACK = 0x01
  };

  enum TX_ACOMP_ENABLE {
    LIMITEN_ACEN = 0x3000
  };
}

enum RESP_LENS {
  GET_REV_LEN = 9,
  TX_TUNE_STATUS_LEN = 8
};

enum PROPS {
  TX_ACOMP_ENABLE = 0x2200,
  TX_ACOMP_THRESHOLD = 0x2201,
  TX_ACOMP_ATTACK_TIME = 0x2202,
  TX_ACOMP_GAIN = 0x2204,
  TX_LIMITER_RELEASE_TIME = 0x2205
};
