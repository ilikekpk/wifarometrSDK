#ifndef YANDEX_WEATHER_H
#define YANDEX_WEATHER_H

#include "osapi.h"

typedef struct
{
    uint32_t timestamp;

    int8_t temp;
    int8_t feels_like;
    int8_t temp_water;
    uint8_t wind_speed;
    uint8_t wind_gust;
    uint16_t pressure_mm;
    uint16_t pressure_pa;
    uint8_t humidity;
} yandex_wheather_t;

bool parse_yandex_weather(char* data, yandex_wheather_t* weather);
void print_weather(yandex_wheather_t* weather);


#endif
