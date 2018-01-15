#include "Radio.h"
#include <Wire.h>

const ushort ADDR = 0x63;
const ushort RESET = 12;
const ushort TX_PWR = 115;
const uint FREQ = 10790;
const uint BAUD = 9600;
const ushort REV = 16;

Radio radio = Radio(ADDR, RESET);

void reset() {
  Serial.println();
  Serial.println("Resetting Radio");
  radio.reset();
  Serial.println("Done");
}

bool power_up() {
  Serial.println();
  Serial.println("Powering Radio Up");
  if (radio.power_up()) {
    Serial.println("Done");
    return true;
  }
  else {
    Serial.println("Failed");
    return false;
  }
}

bool get_rev() {
  Serial.println();
  Serial.println("Checking Rev");
  ushort rev = radio.get_rev();
  Serial.print("\tRev: ");
  Serial.println(rev);
  if (rev == REV) {
    Serial.println("Pass");
    return true;
  }
  else {
    Serial.println("Fail");
    return false;
  }
}

bool set_freq() {
  Serial.println();
  Serial.print("Setting Frequency to ");
  Serial.print(FREQ / 100);
  Serial.print(".");
  Serial.println(FREQ % 100);
  if (radio.set_freq(FREQ)) {
    Serial.println("Done");
    return true;
  }
  else {
    Serial.println("Fail");
    return false;
  }
}

bool stc_loop() {
  Serial.println();
  Serial.println("Waiting for STC bit");
  if (radio.stc_loop()) {
    Serial.println("Pass");
    return true;
  }
  else {
    Serial.println("Fail");
    return false;
  }
}

bool check_tune_status() {
  Serial.println();
  Serial.println("Checking Tune Status");
  if (radio.check_tx_tune()) {
    Serial.println("Pass");
    return true;
  }
  else {
    Serial.println("Fail");
    return false;
  }
}

void setup() {
  Serial.begin(BAUD);
  Wire.begin();
  pinMode(RESET, OUTPUT);

  reset();

  if (!power_up()) {
    return;
  }

  if (!get_rev()) {
    return;
  }

  if (!set_freq()) {
    return;
  }

  if (!stc_loop()) {
    return;
  }

  if (!check_tune_status()) {
    return;
  }
}

void loop() {

}
