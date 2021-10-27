#ifndef SYSTEM_H
#define SYSTEM_H
#include <stdint.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <algorithm>
#include <cstring>

volatile void robust_delay( uint32_t);
volatile void robust_us( uint32_t);

#define TIMDELAY TIM4
#define RCC_TIMDELAY RCC_TIM4
void delay_setup(void);
void delay_us ( uint16_t );
void delay_ms ( uint16_t );
#endif
