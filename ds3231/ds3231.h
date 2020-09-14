#include <systems.h>
#include <libopencm3/stm32/i2c.h>
#include <ctime>
#define DS3231_ADDRESS 0x68


class ds3231
{
  public:
  
  ds3231();
  
  int getSec(void);
  uint8_t readReg(uint8_t);
    
};
