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
#define WIFI_SCAN_DELAY 10000

calibr_struct calibr;


const char* DEVICE_NAME = "ESP";
static void loop();


static os_timer_t wifi_scan_timer;

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
	if(evt->event != EVENT_SOFTAPMODE_PROBEREQRECVED)os_printf("event: %d\n", evt->event);
	switch(evt->event){
		case EVENT_STAMODE_GOT_IP:
			os_printf("connected");
            os_timer_disarm(&wifi_scan_timer); //disable timer to ap mode
            char header[YANDEX_API_KEY_BUF_SIZE + 50];
            os_sprintf(header, "X-Yandex-API-Key: %s\r\n", calibr.yandex_api_key);
            //http_get("https://api.weather.yandex.ru/v1/informers?lat=55.7887&lon=49.1221", header, http_callback_nikita);
		break;
	}
}


void enable_softap()
{
    os_printf("---------------enabling softapp-------------");
    wifi_set_opmode(SOFTAP_MODE);
    struct softap_config softapConf;
    os_memcpy(&softapConf.ssid, "Wifarometr", 10);
    softapConf.ssid_len = 10;
    softapConf.authmode = AUTH_OPEN;
    softapConf.beacon_interval = 100;
   // softapConf.channel = 1;
    //wifi_softap_get_config_default(&softapConf);

    struct dhcps_lease dhcp_lease;
    struct ip_info info;

    wifi_softap_dhcps_stop();

    IP4_ADDR(&info.ip,	192, 168, 1, 1);
	IP4_ADDR(&info.gw,	192, 168, 1, 1);
	IP4_ADDR(&info.netmask, 255, 255, 255, 0);
    wifi_set_ip_info(SOFTAP_IF,	&info);

    const char* start_ip =	"192.168.1.100";
    const char* end_ip	= "192.168.1.105";
    dhcp_lease.start_ip.addr =	ipaddr_addr(start_ip);
    dhcp_lease.end_ip.addr	= ipaddr_addr(end_ip);
    wifi_softap_set_dhcps_lease(&dhcp_lease);

    wifi_softap_dhcps_start();

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
    user_tcpserver_init(80); 
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
	

    

     os_timer_disarm(&wifi_scan_timer);
     os_timer_setfn(&wifi_scan_timer, (os_timer_func_t *)enable_softap, NULL);
     os_timer_arm(&wifi_scan_timer, WIFI_SCAN_DELAY, 0); //timer for wait wifi connectinf else use softap mode

}
