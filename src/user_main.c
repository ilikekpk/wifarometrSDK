#include "osapi.h"
#include "user_interface.h"
#include "ets_sys.h"
#include "gpio.h"
#include "os_type.h"
#include "espconn.h"
#include "upgrade.h"
#include "mem.h"

#include "TM1637.h"
#include "calibr_struct.h"
#include "httpclient.h"
#include "yandex_weather.h"
#include "ntp_client.h"
#include "display_api.h"



#define LOCAL_PORT 1025
#define REMOTE_PORT 1024
#define WIFI_SCAN_DELAY 10000

calibr_struct calibr;


const char* DEVICE_NAME = "ESP";
static void loop();


static os_timer_t wifi_scan_timer;
static os_timer_t ntp_sync_timer;
static os_timer_t meteo_sync_timer;
static os_timer_t display_timer;

static os_timer_t test_timer;

yandex_wheather_t weather = {0};

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


void ICACHE_FLASH_ATTR 
http_callback_nikita(char * response, int http_status, char * full_response)
{
	os_printf("http_status=%d\n", http_status);	
	if (http_status != HTTP_STATUS_GENERIC_ERROR) {
		os_printf("strlen(full_response)=%d\n", strlen(full_response));
		os_printf("response=%s<EOF>\n", full_response);
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
            meteo_sync_start();
            ntp_sync_start();
		break;
	}
}


void enable_softap()
{
    os_printf("---------------enabling softapp-------------\r\n");
    wifi_set_opmode(SOFTAP_MODE);
    struct softap_config softapConf;
    os_memcpy(&softapConf.ssid, "Wifarometr", 10);
    softapConf.ssid_len = 10;
    softapConf.authmode = AUTH_OPEN;
    softapConf.beacon_interval = 100;
   // softapConf.channel = 1;
    //wifi_softap_get_config_default(&softapConf);

    wifi_softap_set_config(&softapConf);

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
ntp_sync_start()
{
    ntp_get_time("0.ru.pool.ntp.org", 123);
    os_timer_arm(&ntp_sync_timer, calibr.clock_update_timer * 1000, 0);
    //display_time(ntp_epoch);
}

void ICACHE_FLASH_ATTR 
meteo_sync_start()
{
    char header[YANDEX_API_KEY_BUF_SIZE + 50];
    char url[100];
    os_sprintf(url, "https://api.weather.yandex.ru/v1/informers?lat=%s&lon=%s", calibr.latitude, calibr.longitude);
    os_sprintf(header, "X-Yandex-API-Key: %s\r\n", calibr.yandex_api_key);
    http_get(url, header, http_callback_nikita);
    os_timer_arm(&meteo_sync_timer, calibr.meteo_update_timer * 1000 * 60, 0);
}

bool ICACHE_FLASH_ATTR 
display_data(display_msg_t current_msg)
{
    os_printf("------------display_data--------------\r\n");
    os_printf("current_msg: %d\r\n", current_msg);
    switch (current_msg)
    {
    case DISABLED:
        return false;
        break;
    case TIME:
        display_time(ntp_epoch, calibr.GMT);
        break;
    case TEMP:
        display_temp(weather.temp);
        break;
    case FEELS_LIKE:
        display_feels_like(weather.feels_like);
        break;
    case TEMP_WATER:
        display_temp_water(weather.temp_water);
        break;
    case WIND_SPEED:
        display_wind_speed(weather.wind_speed);
        break;
    case WIND_GUST:
        display_wind_gust(weather.wind_gust);
        break;
    case PRESSURE_MM:
        display_pressure_mm(weather.pressure_mm);
        break;
    case PRESSURE_PA:
        return false;
        break;
    case HUMIDITY:
        display_humidity(weather.humidity);
        break;
    }

    return true;
}

void ICACHE_FLASH_ATTR 
display_timer_start()
{
    os_printf("------------display_timer_start--------------\r\n");

    

    static uint8_t msg_num = 1;

    os_printf("msg_num: %d\r\n", msg_num);

    os_timer_disarm(&display_timer);

    switch (msg_num)
    {
        case 1:
            msg_num = 2;
            if(display_data(calibr.first_msg)) os_timer_arm(&display_timer, calibr.first_msg_timer * 1000, 0); 
            else display_timer_start();
            break;
        case 2:
            msg_num = 3;
            if(display_data(calibr.second_msg)) os_timer_arm(&display_timer, calibr.second_msg_timer * 1000, 0);
            else display_timer_start();
            break;
        case 3:
            msg_num = 4;
            if(display_data(calibr.third_msg)) os_timer_arm(&display_timer, calibr.third_msg_timer * 1000, 0);
            else display_timer_start();
            break;
        case 4:
            msg_num = 1;
            if(display_data(calibr.fourth_msg)) os_timer_arm(&display_timer, calibr.fourth_msg_timer * 1000, 0);
            else display_timer_start();
            break;  
    }
}

void ICACHE_FLASH_ATTR 
test_timer_start()
{
    static int8_t count = 0;
    if(count > 120) os_timer_disarm(&test_timer);
    
    uint8_t arr[4] = {EMPTY_SYMBOL, EMPTY_SYMBOL, EMPTY_SYMBOL, count};
    display_humidity(count);

    count++;
}


void ICACHE_FLASH_ATTR 
user_init(void)
{
    
   // gpio_init();
    uart_init(115200, 115200);   
    //tm1637_init();
    display_init();
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
	wifi_station_set_hostname(calibr.hostname);
	

    

    os_timer_disarm(&wifi_scan_timer);
    os_timer_setfn(&wifi_scan_timer, (os_timer_func_t *)enable_softap, NULL);
    os_timer_arm(&wifi_scan_timer, WIFI_SCAN_DELAY, 0); //timer for wait wifi connectinf else use softap mode

    os_timer_disarm(&ntp_sync_timer);
    os_timer_setfn(&ntp_sync_timer, (os_timer_func_t *)ntp_sync_start, NULL);

    os_timer_disarm(&meteo_sync_timer);
    os_timer_setfn(&meteo_sync_timer, (os_timer_func_t *)meteo_sync_start, NULL);

    os_timer_disarm(&display_timer);
    os_timer_setfn(&display_timer, (os_timer_func_t *)display_timer_start, NULL);
    display_timer_start();

    os_timer_disarm(&test_timer);
    os_timer_setfn(&test_timer, (os_timer_func_t *)test_timer_start, NULL);
    //os_timer_arm(&test_timer, 200, 1); 

}
