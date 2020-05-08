#include "display_api.h"
#include "TM1637.h"

#define BLINK_DOTS_TIME 500

static os_timer_t dots_blink_timer;

LOCAL void ICACHE_FLASH_ATTR 
blink_dots_timer_start()
{
    static bool dots_state = 1;

    os_timer_arm(&dots_blink_timer, BLINK_DOTS_TIME, 0);

    tm1637_dots_state(dots_state);
    dots_state = !dots_state;
}

void ICACHE_FLASH_ATTR
display_init()
{
    tm1637_init();

    os_timer_disarm(&dots_blink_timer);
    os_timer_setfn(&dots_blink_timer, (os_timer_func_t *)blink_dots_timer_start, NULL);
}

LOCAL void ICACHE_FLASH_ATTR 
blinking_dots(bool state)
{
    if(state) blink_dots_timer_start();
    else
    {
        os_timer_disarm(&dots_blink_timer);
        tm1637_dots_state(0);
    }
     
}

void ICACHE_FLASH_ATTR 
display_time(uint32_t epoch)
{
    blinking_dots(true);
    uint8_t hour = (epoch % 86400) / 3600;
    uint8_t minute = (epoch  % 3600) / 60;

    uint8_t arr[4] = {0};

    if(hour / 10 == 0) arr[0] = EMPTY_SYMBOL;
    else arr[0] = hour / 10;
    arr[1] = hour % 10;
    arr[2] = minute / 10;
    arr[3] = minute % 10;

    tm1637_display(arr);
}

void ICACHE_FLASH_ATTR 
display_temp(int8_t temp)
{
    blinking_dots(false);

    uint8_t arr[4] = {0};

    if(temp / 10 != 0)
    {
        arr[0] = temp < 0 ? MINUS_SYMBOL : EMPTY_SYMBOL;
        arr[1] = abs(temp / 10);
    } 
    else
    {
        arr[0] = EMPTY_SYMBOL;
        arr[1] = temp < 0 ? MINUS_SYMBOL : EMPTY_SYMBOL;
    } 
    arr[2] = abs(temp) % 10;
    arr[3] = GRADUS_SYMBOL;

    tm1637_display(arr);
}

void ICACHE_FLASH_ATTR 
display_feels_like(int8_t feels_like)
{
    blinking_dots(false);

    uint8_t arr[4] = {0};

    if(feels_like / 10 != 0)
    {
        arr[0] = feels_like < 0 ? MINUS_SYMBOL : EMPTY_SYMBOL;
        arr[1] = abs(feels_like / 10);
    } 
    else
    {
        arr[0] = EMPTY_SYMBOL;
        arr[1] = feels_like < 0 ? MINUS_SYMBOL : EMPTY_SYMBOL;
    } 
    arr[2] = abs(feels_like) % 10;
    arr[3] = F_SYMBOL;

    tm1637_display(arr);
}

void ICACHE_FLASH_ATTR 
display_temp_water(int8_t temp_water)
{
    blinking_dots(false);

    uint8_t arr[4] = {0};

    if(temp_water / 10 != 0)
    {
        arr[0] = temp_water < 0 ? MINUS_SYMBOL : EMPTY_SYMBOL;
        arr[1] = abs(temp_water / 10);
    } 
    else
    {
        arr[0] = EMPTY_SYMBOL;
        arr[1] = temp_water < 0 ? MINUS_SYMBOL : EMPTY_SYMBOL;
    } 
    arr[2] = abs(temp_water) % 10;
    arr[3] = C_SYMBOL;

    tm1637_display(arr);
}

void ICACHE_FLASH_ATTR 
display_wind_speed(uint8_t wind_speed)
{
    blinking_dots(false);
    
    uint8_t arr[4] = {0};
    
    arr[0] = EMPTY_SYMBOL;
    if(wind_speed / 10 == 0) arr[1] = EMPTY_SYMBOL;
    else arr[1] = wind_speed / 10;
    arr[2] = wind_speed % 10;
    arr[3] = WIND_SYMBOL;
    
    tm1637_display(arr);
}

void ICACHE_FLASH_ATTR 
display_wind_gust(uint8_t wind_gust)
{
    blinking_dots(false);
    
    uint8_t arr[4] = {0};

    if(wind_gust / 10 == 0) arr[0] = EMPTY_SYMBOL;
    else arr[0] = wind_gust / 10;
    arr[1] = wind_gust % 10;
    arr[2] = WIND_SYMBOL;
    arr[3] = WIND_SYMBOL;

    tm1637_display(arr);
}

void ICACHE_FLASH_ATTR 
display_pressure_mm(uint8_t pressure_mm)
{
    blinking_dots(false);
    
    uint8_t arr[4] = {0};

    if(pressure_mm / 100 == 0)
    {
        arr[0] = EMPTY_SYMBOL;
        arr[1] = pressure_mm / 10 == 0 ? EMPTY_SYMBOL : pressure_mm / 10;
    } 
    else
    {
        arr[0] = pressure_mm / 100;
        arr[1] = pressure_mm % 100 / 10;
    } 
    arr[2] = pressure_mm % 10;
    arr[3] = P_SYMBOL;

    tm1637_display(arr);
}

void ICACHE_FLASH_ATTR 
display_humidity(uint8_t humidity)
{
    blinking_dots(false);
    uint8_t arr[4] = {0};

    arr[0] = EMPTY_SYMBOL;
    arr[1] = humidity > 9 ? humidity / 10 : EMPTY_SYMBOL;
    arr[2] = humidity % 10;
    arr[3] = H_SYMBOL;

    tm1637_display(arr);
}