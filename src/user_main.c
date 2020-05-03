#include "osapi.h"
#include "user_interface.h"
#include "ets_sys.h"
#include "gpio.h"
#include "os_type.h"
#include "espconn.h"
#include "upgrade.h"
#include "espconn.h"
#include "mem.h"

#include "TM1637.h"
#include "calibr_struct.h"
#include "httpclient.h"
#include "yandex_weather.h"



#define LOCAL_PORT 1025
#define REMOTE_PORT 1024


calibr_struct calibr;


const char* DEVICE_NAME = "ESP";
static void loop();


static os_timer_t ptimer;
static os_timer_t ptimer1;

/******************************************************************************
 * FunctionName : user_rf_cal_sector_set
 * Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
 *                We add this function to force users to set rf cal sector, since
 *                we don't know which sector is free in user's application.
 *                sector map for last several sectors : ABBBCDDD
 *                A : rf cal
 *                B : at parameters
 *                C : rf init data
 *                D : sdk parameters
 * Parameters   : none
 * Returns      : rf cal sector
*******************************************************************************/
uint32 ICACHE_FLASH_ATTR
user_rf_cal_sector_set(void)
{
    enum flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        case FLASH_SIZE_64M_MAP_1024_1024:
            rf_cal_sec = 2048 - 5;
            break;
        case FLASH_SIZE_128M_MAP_1024_1024:
            rf_cal_sec = 4096 - 5;
            break;
        default:
            rf_cal_sec = 0;
            break;
    }
    return rf_cal_sec;
}


void ICACHE_FLASH_ATTR http_callback_nikita(char * response, int http_status, char * full_response)
{
	os_printf("http_status=%d\n", http_status);	
	if (http_status != HTTP_STATUS_GENERIC_ERROR) {
		os_printf("strlen(full_response)=%d\n", strlen(full_response));
		os_printf("response=%s<EOF>\n", full_response);
        yandex_wheather_t weather = {0};
        parse_yandex_weather(full_response, &weather);
	}
}


void wifi_handle_event_cb(System_Event_t *evt)
{
	os_printf("event: %d\n", evt->event);
	switch(evt->event){
		case EVENT_STAMODE_GOT_IP:
			os_printf("connected");

            user_tcpserver_init(80); // start tcp server
            char header[YANDEX_API_KEY_BUF_SIZE + 50];
            os_sprintf(header, "X-Yandex-API-Key: %s\r\n", calibr.yandex_api_key);
            http_get("https://api.weather.yandex.ru/v1/informers?lat=55.7887&lon=49.1221", header, http_callback_nikita);
		break;
	}
}





void ICACHE_FLASH_ATTR 
user_init(void)
{
    
   // gpio_init();
    uart_init(115200, 115200);   
    tm1637_init();
    uint8_t arr[] = {16,2,3,4};
    tm1637_display(arr);
    spi_flash_read(0x8c000, &calibr, sizeof(calibr_struct));
    print_calibr();
    // for(uint8_t i = 0; i < 255; i++)
    // {
    //   //  tm1637_write_byte(i);
    //     tm1637_dots_state(1);
    //     system_soft_wdt_feed();
    //     for(uint16_t j = 0; j < 300; j++) os_delay_us(1000);
    //     tm1637_dots_state(0);
    //     system_soft_wdt_feed();
    //     for(uint16_t j = 0; j < 300; j++) os_delay_us(1000);
    // } 
    wifi_set_opmode(STATION_MODE);
	struct station_config stationConf;
	stationConf.bssid_set = 0;
	os_memcpy(&stationConf.ssid, calibr.ssid, SSID_BUF_SIZE);
	os_memcpy(&stationConf.password, calibr.passwd, PASSWD_BUF_SIZE);
	
	wifi_station_set_config(&stationConf);
	wifi_set_event_handler_cb(wifi_handle_event_cb);
	wifi_station_set_hostname("tstESP");
	
    // PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
    // PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4);
    // PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5);
    // GPIO_DIS_OUTPUT(5);
    // PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO5_U);
    // GPIO_OUTPUT_SET(4, 1);
    
    // lastState = GPIO_INPUT_GET(5);
    

    // os_timer_disarm(&ptimer1);
    // os_timer_setfn(&ptimer1, (os_timer_func_t *)button_handler, NULL);
    // os_timer_arm(&ptimer1, 100, 1);

}
