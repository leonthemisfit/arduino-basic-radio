#include "Radio.h"
#include "codes.h"
#include <Wire.h>
#include <Arduino.h>

Radio::Radio(short addr, uint reset) : _addr{addr}, _reset{reset} {}

void Radio::reset() {
    digitalWrite(_reset, HIGH);
    delay(10);
    digitalWrite(_reset, LOW);
    delay(10);
    digitalWrite(_reset, HIGH);
}

bool Radio::send(const ushort data[], int len) {
  if (len < 1 || sizeof(data) < 1) {
    return false;
  }

  Wire.beginTransmission(_addr);
  for (int i = 0; i < len; i++) {
    if (Wire.write(data[i]) < 1) {
      return false;
    }
  }
  Wire.endTransmission();

  ushort status = 0, timeout = 100;
  while ((status & STATUS::CTS) != STATUS::CTS) {
    Wire.requestFrom(_addr, 1);
    while(Wire.available() < 1 && timeout > 0) {
      delay(1);
      timeout--;
    }
    if (timeout == 0) {
      return false;
    }
    status = Wire.read();
  }

  return (status & STATUS::ERR) != STATUS::ERR;
}

bool Radio::power_up() {
  ushort data[] = { CMD::POWER_UP, ARGS::POWER_UP::TX_MODE, ARGS::POWER_UP::OP_MODE };
  if (send(data, 3)) {
    Wire.requestFrom(_addr, 1);
    ushort stat = Wire.read();
    return (stat & STATUS::CTS) == STATUS::CTS;
  }
  else {
    return false;
  }
}

ushort Radio::get_rev() {
  ushort data[] = { CMD::GET_REV };
  if (send(data, 1)) {
    Wire.requestFrom(_addr, RESP_LENS::GET_REV_LEN);
    ushort resp[RESP_LENS::GET_REV_LEN];
    for (int i = 0; i < RESP_LENS::GET_REV_LEN; i++) {
      resp[i] = Wire.read();
    }
    if ((resp[0] & STATUS::CTS) == STATUS::CTS) {
      return resp[1];
    }
    else {
      return 0;
    }
  }
  else {
    return 0;
  }
}

bool Radio::set_freq(uint freq) {
  ushort high = (freq >> 8) & 0xFF;
  ushort low = freq & 0xFF;
  ushort data[] = { CMD::TX_TUNE_FREQ, RESERVED, high, low };
  if (send(data, 4)) {
    Wire.requestFrom(_addr, 1);
    ushort stat = Wire.read();
    return (stat & STATUS::CTS) == STATUS::CTS;
  }
  else {
    return false;
  }
}

ushort Radio::get_int_status() {
  ushort data[] = { CMD::GET_INT_STATUS };
  if (send(data, 1)) {
    Wire.requestFrom(_addr, 1);
    while (Wire.available() < 1) {
      delay(1);
    }
    return Wire.read();
  }
  else {
    return 0;
  }
}

bool Radio::stc_loop() {
  ushort timeout = STC_TIMEOUT;
  while (((get_int_status() & STATUS::STC) != STATUS::STC) && (timeout > 0)) {
    delay(1);
    timeout--;
  }
  return (timeout > 0);
}

bool Radio::check_tx_tune() {
  ushort data[] = { CMD::TX_TUNE_STATUS, ARGS::TX_TUNE_STATUS::INTACK };
  if (send(data, 2)) {
    Wire.requestFrom(_addr, RESP_LENS::TX_TUNE_STATUS_LEN);
    ushort resp[RESP_LENS::TX_TUNE_STATUS_LEN];
    for (int i = 0; i < RESP_LENS::TX_TUNE_STATUS_LEN; i++) {
      resp[i] = Wire.read();
    }
    return (resp[0] & STATUS::CTS) == STATUS::CTS;
  }
  else {
    return false;
  }
}

bool Radio::set_tx_power(const ushort pwr) {
  ushort data[] = { CMD::TX_TUNE_POWER, RESERVED, RESERVED, pwr, RESERVED };
  if (send(data, 5)) {
    Wire.requestFrom(_addr, 1);
    ushort stat = Wire.read();
    return (stat & STATUS::CTS) == STATUS::CTS;
  }
  else {
    return false;
  }
}

bool Radio::set_property(const uint prop, const uint val) {
  ushort prop_high = (prop >> 8) & 0xFF;
  ushort prop_low = prop & 0xFF;
  ushort val_high = (val >> 8) & 0xFF;
  ushort val_low = val & 0xFF;

  ushort data[] = { CMD::SET_PROPERTY, RESERVED, prop_high, prop_low, val_high, val_low };
  return send(data, 6);
}
