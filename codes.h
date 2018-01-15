const unsigned short RESERVED = 0x00;
const unsigned short STC_TIMEOUT = 100;

enum CMD {
  POWER_UP = 0x01,
  GET_REV = 0x02,
  GET_INT_STATUS = 0x14,
  TX_TUNE_FREQ = 0x30,
  TX_TUNE_POWER = 0x31,
  TX_TUNE_STATUS = 0x33
};

enum STATUS {
  CTS = 0x80,
  ERR = 0x40,
  STC = 0x80
};

namespace ARGS {
  enum POWER_UP {
    TX_MODE = 0xC2,
    OP_MODE = 0x50
  };

  enum TX_TUNE_STATUS {
    INTACK = 0x01
  };
}

enum RESP_LENS {
  GET_REV_LEN = 9,
  TX_TUNE_STATUS_LEN = 8
};
