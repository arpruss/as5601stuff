#include <Wire.h>

// Read angle and AGC from as5601
// 
// Mega: 20 SDA, 21 SCL, 
//
// AS5601: 
//  1 VDD5V (0.1uF to ground; +5V)   8 A   
//  2 VDD3V3 (1uF to ground)         7 SCL
//  3 PUSH                           6 SDA
//  4 GND                            5 B
///////////////////////////////////////////////

const uint8_t deviceAddress = 0x36;
const uint8_t reg_rawAngleHigh = 0x0C;
const uint8_t reg_rawAngleLow = 0x0D;
const uint8_t reg_agc = 0x1A;

void setup() {
  Serial.begin(115200);
  Wire.begin();
}

String formatAngle(uint16_t angle) {
// convert 12 bit angle to decimal
//  Serial.println("angle="+String(angle));
  uint32_t angle360 = (uint32_t)angle * 360;
  uint16_t deg = (angle360>>12);
  uint8_t frac = (100*(angle360&0xFFF))>>12;
  return String(deg)+"."+String(frac/10)+String(frac%10);
}

int getByte(uint8_t reg) {
  Wire.beginTransmission(deviceAddress);
  Wire.write(reg); // angle high
  Wire.endTransmission();
  Wire.requestFrom(deviceAddress,(uint8_t)1);
  if (1<=Wire.available()) {
    return 0xFF & Wire.read();
  }
  else {
    return -1;
  }
}

void loop() {
  int angleHigh = getByte(reg_rawAngleHigh);
  if (0 <= angleHigh) {
    int angleLow = getByte(reg_rawAngleLow);
    if (0 <= angleLow) {
      uint16_t angle = (angleHigh<<8) | angleLow;
      Serial.print(formatAngle(angle));
    }
  }
  int agc = getByte(reg_agc);
  if (0 <= agc) {
    Serial.print(" "+String(agc));
  }
  Serial.println("");
}
