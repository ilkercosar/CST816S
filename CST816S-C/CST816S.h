#ifndef __CST816S__
#define __CST816S__

#ifdef __cplusplus
extern "C"
{
#endif 

/* include library */
#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>

/* private defines */
#define CST816S_ADDRESS     0x15
#define _sda                21
#define _scl                22
#define _rst                5
#define _irq                33
    

/* touch events */
enum GESTURE 
{
  NONE = 0x00,
  SWIPE_UP = 0x01,
  SWIPE_DOWN = 0x02,
  SWIPE_LEFT = 0x03,
  SWIPE_RIGHT = 0x04,
  SINGLE_CLICK = 0x05,
  DOUBLE_CLICK = 0x0B,
  LONG_PRESS = 0x0C

};


/* custom variables */
struct data_struct 
{
  uint8_t gestureID; 
  uint8_t points; 
  uint8_t event; 
  uint16_t x;
  uint16_t y;
  uint8_t version;
  uint8_t versionInfo[3];
  volatile bool _event_available
};


    /* functions */
    void IRAM_ATTR handleISR();
    void read_touch();
    void begin();
    void sleep();
    bool available();
    char* gesture();

    uint8_t i2c_read(uint16_t addr, uint8_t reg_addr, uint8_t * reg_data, uint32_t length);
    uint8_t i2c_write(uint8_t addr, uint8_t reg_addr, const uint8_t * reg_data, uint32_t length);
    


#ifdef __cplusplus
}
#endif
#endif