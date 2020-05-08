#include "calibr_struct.h"

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

    os_printf("------------------------\r\n");
}