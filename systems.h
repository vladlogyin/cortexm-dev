#ifndef SYSTEM_H
#define SYSTEM_H
#include <stdint.h>
#include <algorithm>
//delay_us and delay_ms shit themselves if called inside another class or function
volatile void delay_us( uint32_t);
volatile void delay_ms( uint32_t);
volatile void robust_delay( uint32_t);
volatile void robust_us( uint32_t);
#endif
