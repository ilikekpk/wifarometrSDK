#ifndef DISPLAY_API_H
#define DISPLAY_API_H

#include "osapi.h"

void ICACHE_FLASH_ATTR
display_init();

void ICACHE_FLASH_ATTR 
display_time(uint32_t epoch);

void ICACHE_FLASH_ATTR 
display_temp(int8_t time);

void ICACHE_FLASH_ATTR 
display_feels_like(int8_t feels_like);

void ICACHE_FLASH_ATTR 
display_temp_water(int8_t time_water);

void ICACHE_FLASH_ATTR 
display_wind_speed(uint8_t wind_speed);

void ICACHE_FLASH_ATTR 
display_wind_gust(uint8_t wind_gust);

void ICACHE_FLASH_ATTR 
display_pressure_mm(uint8_t pressure_mm);

void ICACHE_FLASH_ATTR 
display_humidity(uint8_t humidity);

#endif
