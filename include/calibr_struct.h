#ifndef CALIBR_STRUCT_H
#define CALIBR_STRUCT_H

#include "osapi.h"

#define HOSTNAME_BUF_SIZE 20
#define SSID_BUF_SIZE 32
#define PASSWD_BUF_SIZE 64
#define YANDEX_API_KEY_BUF_SIZE 128

void print_calibr();


typedef enum
{
    DISABLED = 0,
    TEMP,
    FEELS_LIKE,
    TEMP_WATER,
    WIND_SPEED, 
    WIND_GUST,
    PRESSURE_MM,
    PRESSURE_PA,
    HUMIDITY
} display_msg_t;

typedef struct
{
    char hostname[HOSTNAME_BUF_SIZE];
    char ssid[SSID_BUF_SIZE];
    char passwd[PASSWD_BUF_SIZE];
    char yandex_api_key[YANDEX_API_KEY_BUF_SIZE];

    display_msg_t first_msg;
    display_msg_t second_msg;
    display_msg_t third_msg;
    display_msg_t fourth_msg;

    uint8_t first_msg_timer;
    uint8_t second_msg_timer;
    uint8_t third_msg_timer;
    uint8_t fourth_msg_timer;

    uint16_t dark_adc;
    uint16_t brigh_adc;

    uint8_t meteo_update_timer;
    uint8_t clock_update_timer;
    char GMT[20];

    char latitude[20];
    char longitude[20];

} calibr_struct;


extern calibr_struct calibr;

#endif
