#ifndef HTU21D_H
#define HTU21D_H

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <systemutils.h>

#define HTU21D_ADDRESS               0x40      //chip i2c address

#define HTU21D_USER_REGISTER_WRITE   0xE6      //write user register
#define HTU21D_USER_REGISTER_READ    0xE7      //read  user register

#define HTU21D_HEATER_REGISTER_WRITE 0x51      //write heater control register
#define HTU21D_HEATER_REGISTER_READ  0x11      //read  heater control register

#define HTU21D_SOFT_RESET            0xFE      //soft reset

#define HTU21D_SERIAL1_READ1         0xFA      //read 1-st two serial bytes
#define HTU21D_SERIAL1_READ2         0x0F      //read 2-nd two serial bytes
#define HTU21D_SERIAL2_READ1         0xFC      //read 3-rd two serial bytes
#define HTU21D_SERIAL2_READ2         0xC9      //read 4-th two serial bytes

#define SI7013_CHIPID                0x0D      //device id SI7013
#define SI7020_CHIPID                0x14      //device id SI7020
#define SI7021_CHIPID                0x15      //device id SI7021
#define HTU21D_CHIPID                0x32      //device id HTU21D/SHT21 

#define HTU21D_FIRMWARE_READ1        0x84      //read firmware revision, 1-st part of the command
#define HTU21D_FIRMWARE_READ2        0xB8      //read firmware revision, 2-nd part of the command

#define HTU21D_FIRMWARE_V1           0xFF      //sensor firmware v1.0
#define HTU21D_FIRMWARE_V2           0x20      //sensor firmware 2.0

#define HTU21D_TEMP_COEFFICIENT      -0.15     //temperature coefficient for RH compensation at range 0°C..80°C, for HTU21D & SHT21 only 
#define HTU21D_CRC8_POLYNOMINAL      0x13100   //crc8 polynomial for 16bit value, CRC8 -> x^8 + x^5 + x^4 + 1


#define HTU21D_SOFT_RESET_DELAY      15        //in milliseconds

#define HTU21D_FORCE_READ_TEMP       0xFE      //force to read temperature, see https://github.com/enjoyneering/HTU21D/pull/3
#define HTU21D_ERROR                 0xFF      //returns 255, if CRC8 or communication error is occurred

typedef enum : uint8_t
{
  HTU21D_RES_RH12_TEMP14 = 0x00,               //resolution, temperature: 14-Bit & humidity: 12-Bit
  HTU21D_RES_RH8_TEMP12  = 0x01,               //resolution, temperature: 12-Bit & humidity: 8-Bit
  HTU21D_RES_RH10_TEMP13 = 0x80,               //resolution, temperature: 13-Bit & humidity: 10-Bit
  HTU21D_RES_RH11_TEMP11 = 0x81                //resolution, temperature: 11-Bit & humidity: 11-Bit
}
HTU21D_RESOLUTION;

typedef enum : uint8_t
{
  HTU21D_TRIGGER_HUMD_MEASURE_HOLD   = 0xE5,   //humidity measurement with hold master
  HTU21D_TRIGGER_HUMD_MEASURE_NOHOLD = 0xF5    //temperature measurement with no hold master
}
HTU21D_HUMD_OPERATION_MODE;

typedef enum : uint8_t
{
  HTU21D_TRIGGER_TEMP_MEASURE_HOLD     = 0xE3, //temperature measurement with hold master
  HTU21D_TRIGGER_TEMP_MEASURE_NOHOLD   = 0xF3, //temperature measurement with no hold master
  SI70xx_TEMP_READ_AFTER_RH_MEASURMENT = 0xE0  //read temperature value from previous RH measurement, for Si7021 only
}
HTU21D_TEMP_OPERATION_MODE;

typedef enum : uint8_t
{
  HTU21D_ON  = 0x04,                           //heater ON
  HTU21D_OFF = 0xFB                            //heater OFF
}
HTU21D_HEATER_SWITCH;


class HTU21D
{
  public:
   HTU21D(HTU21D_RESOLUTION = HTU21D_RES_RH12_TEMP14);


   bool     begin(void);
   float    readHumidity(HTU21D_HUMD_OPERATION_MODE = HTU21D_TRIGGER_HUMD_MEASURE_HOLD);    //max accuracy ±2%RH  in range 20%..80% at 25°C
   float    readCompensatedHumidity(float temperature = HTU21D_FORCE_READ_TEMP);            //max accuracy ±2%RH  in range 0%..100% at 0°C..80°C
   float    readTemperature(HTU21D_TEMP_OPERATION_MODE = HTU21D_TRIGGER_TEMP_MEASURE_HOLD); //max accuracy ±0.3°C in range 0°C..60°C
   void     setResolution(HTU21D_RESOLUTION sensorResolution);
   void     softReset(void);
   bool     batteryStatus(void);
   void     setHeater(HTU21D_HEATER_SWITCH heaterSwitch);
   uint16_t readDeviceID(void);
   uint8_t  readFirmwareVersion(void);

  private:
   HTU21D_RESOLUTION _resolution;

   bool    write8(uint8_t reg, uint8_t value);
   uint8_t read8(uint8_t reg);
   uint8_t checkCRC8(uint16_t data);
};

#endif
