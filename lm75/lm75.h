/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2010 Thomas Otto <tommi@viadmin.org>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LM75_H
#define LM75_H

#include <stdint.h>

#define LM75_SENSOR0		0x48
#define LM75_SENSOR1		0x49
#define LM75_SENSOR2		0x4a
#define LM75_SENSOR3		0x4b
#define LM75_SENSOR4		0x4c
#define LM75_SENSOR5		0x4d
#define LM75_SENSOR6		0x4e
#define LM75_SENSOR7		0x4f

void lm75_write_config(uint32_t i2c, uint8_t sensor);
void lm75_write_temp_os(uint32_t i2c, uint8_t sensor, uint16_t temp_os);
void lm75_write_temp_hyst(uint32_t i2c, uint8_t sensor, uint16_t temp_hyst);
uint16_t lm75_read_temperature(uint32_t i2c, uint8_t sensor);
uint16_t lm75temp();
#endif
