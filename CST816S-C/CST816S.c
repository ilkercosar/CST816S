#include "CST816S.h"
#include <Wire.h>

void read_touch() 
{
  uint8_t data_raw[8];
  i2c_read(CST816S_ADDRESS, 0x01, data_raw, 6);

  data_struct.gestureID = data_raw[0];
  data_struct.points = data_raw[1];
  data_struct.event = data_raw[2] >> 6;
  data_struct.x = ((data_raw[2] & 0xF) << 8) + data_raw[3];
  data_struct.y = ((data_raw[4] & 0xF) << 8) + data_raw[5];
}

void IRAM_ATTR handleISR(void) 
{
  data_struct._event_available = true;

}

void begin() 
{
  Wire.begin(_sda, _scl);

  pinMode(_irq, INPUT);
  pinMode(_rst, OUTPUT);

  digitalWrite(_rst, HIGH );
  delay(50);
  digitalWrite(_rst, LOW);
  delay(5);
  digitalWrite(_rst, HIGH );
  delay(50);

  i2c_read(CST816S_ADDRESS, 0x15, &data.version, 1);
  delay(5);
  i2c_read(CST816S_ADDRESS, 0xA7, data.versionInfo, 3);

  attachInterrupt(_irq, handleISR, RISING);
}

bool available() 
{
  if (data_struct._event_available) 
  {
    read_touch();
    data_struct._event_available = false;
    return true;
  }
  return false;
}

void sleep() 
{
  digitalWrite(_rst, LOW);
  delay(5);
  digitalWrite(_rst, HIGH );
  delay(50);
  uint8_t standby_value = 0x03;
  i2c_write(CST816S_ADDRESS, 0xA5, &standby_value, 1);
}

char*gesture() 
{
  switch (data_struct.gestureID) 
  {
    case NONE:
      return "NONE";
      break;
    case SWIPE_DOWN:
      return "SWIPE DOWN";
      break;
    case SWIPE_UP:
      return "SWIPE UP";
      break;
    case SWIPE_LEFT:
      return "SWIPE LEFT";
      break;
    case SWIPE_RIGHT:
      return "SWIPE RIGHT";
      break;
    case SINGLE_CLICK:
      return "SINGLE CLICK";
      break;
    case DOUBLE_CLICK:
      return "DOUBLE CLICK";
      break;
    case LONG_PRESS:
      return "LONG PRESS";
      break;
    default:
      return "UNKNOWN";
      break;
  }
}

uint8_t i2c_read(uint16_t addr, uint8_t reg_addr, uint8_t *reg_data, uint32_t length)
{
  Wire.beginTransmission(addr);
  Wire.write(reg_addr);
  if ( Wire.endTransmission(true))  return -1;
  Wire.requestFrom(addr, length, true);
  for (int i = 0; i < length; i++) 
  {
    *reg_data++ = Wire.read();
  }
  return 0;
}

uint8_t i2c_write(uint8_t addr, uint8_t reg_addr, const uint8_t *reg_data, uint32_t length)
{
  Wire.beginTransmission(addr);
  Wire.write(reg_addr);
  for (int i = 0; i < length; i++) 
  {
    Wire.write(*reg_data++);
  }
  if ( Wire.endTransmission(true))  return -1;
  return 0;
}