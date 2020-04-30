#ifndef CALIBR_STRUCT_H
#define CALIBR_STRUCT_H

#include <stdint.h>

typedef struct
{
    char hostname[20];
    char ssid[20];
    char passwd[20];

    uint8_t tmp_time;
    uint8_t prs_time;
    uint8_t clk_time;

    uint16_t dark_adc;
    uint16_t brigh_adc;

    uint8_t meteo_update_timer;
    uint8_t clock_update_timer;

    char GMT[20];

} calibr_struct;

extern calibr_struct calibr;

#endif
