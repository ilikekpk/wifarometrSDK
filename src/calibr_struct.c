#include "calibr_struct.h"
#include "crc8_maxim.h"


void print_calibr()
{
    os_printf("------------------------\r\n");
    os_printf("hostname: %s\r\n", calibr.hostname);
    os_printf("ssid: %s\r\n", calibr.ssid);
    os_printf("passwd: %s\r\n", calibr.passwd);
    os_printf("yandex_api_key: %s\r\n", calibr.yandex_api_key);

    os_printf("first_msg: %d\r\n", calibr.first_msg);
    os_printf("second_msg: %d\r\n", calibr.second_msg);
    os_printf("third_msg: %d\r\n", calibr.third_msg);
    os_printf("fourth_msg: %d\r\n", calibr.fourth_msg);
    
    os_printf("first_msg_timer: %d\r\n", calibr.first_msg_timer);
    os_printf("second_msg_timer: %d\r\n", calibr.second_msg_timer);
    os_printf("third_msg_timer: %d\r\n", calibr.third_msg_timer);
    os_printf("fourth_msg_timer: %d\r\n", calibr.fourth_msg_timer);

    os_printf("dark_adc: %d\r\n", calibr.dark_adc);
    os_printf("brigh_adc: %d\r\n", calibr.brigh_adc);

    os_printf("meteo_update_timer: %d\r\n", calibr.meteo_update_timer);
    os_printf("clock_update_timer: %d\r\n", calibr.clock_update_timer);
    os_printf("GMT: %d\r\n", calibr.GMT);

    os_printf("lat: %s\r\n", calibr.latitude);
    os_printf("lon: %s\r\n", calibr.longitude);

    os_printf("crc8 maxim: %X\r\n", calibr.crc8_maxim);

    os_printf("structure size: %d\r\n", sizeof(calibr_struct));

    os_printf("------------------------\r\n");
}

void set_default_calibr()
{
    memset(&calibr, 0, sizeof(calibr_struct));
    
    os_strcpy(calibr.hostname, "Wifarometr");
    os_strcpy(calibr.ssid, "ssid");
    os_strcpy(calibr.passwd, "password");
    os_strcpy(calibr.yandex_api_key, "yandex_api_key");

    calibr.first_msg = 1;
    calibr.second_msg = 2;
    calibr.third_msg = 0;
    calibr.fourth_msg = 0;
    
    calibr.first_msg_timer = 3;
    calibr.second_msg_timer = 3;
    calibr.third_msg_timer = 0;
    calibr.fourth_msg_timer = 0;

    calibr.dark_adc = 20;
    calibr.brigh_adc = 80;

    calibr.meteo_update_timer = 60;
    calibr.clock_update_timer = 30;
    calibr.GMT = 3;

    os_strcpy(calibr.latitude, "55.7947054914884");
    os_strcpy(calibr.longitude, "49.117060904296885");

    calibr.crc8_maxim = 0;

}

bool check_calibr_crc()
{
    os_printf("\r\n-----------------checking crc-------------------\r\n");

    uint8_t tmp_crc = calibr.crc8_maxim;

    calibr.crc8_maxim = 0;

    uint8_t crc_result = calculate_crc8_maxim(&calibr, sizeof(calibr_struct));

    calibr.crc8_maxim = tmp_crc;

    os_printf("crc: %X\r\ncalculation crc: %X\r\n", calibr.crc8_maxim, crc_result);

    return calibr.crc8_maxim == crc_result ? true : false;
}

void calculate_calibr_crc()
{
    calibr.crc8_maxim = 0;
    calibr.crc8_maxim = calculate_crc8_maxim(&calibr, sizeof(calibr_struct));
}