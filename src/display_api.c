#include "display_api.h"
#include "TM1637.h"

#define BLINK_DOTS_TIME 500

static os_timer_t dots_blink_timer;

static bool dots_blink_flag = false; // if true, than dots blinking in blink_dots_timer_start()

LOCAL void ICACHE_FLASH_ATTR 
blink_dots_timer_start()
{
    static bool dots_state = 1;

    os_timer_arm(&dots_blink_timer, BLINK_DOTS_TIME, 0);

    if(dots_blink_flag) tm1637_dots_state(dots_state);

    dots_state = !dots_state;
}

void ICACHE_FLASH_ATTR
display_init()
{
    tm1637_init();

    os_timer_disarm(&dots_blink_timer);
    os_timer_setfn(&dots_blink_timer, (os_timer_func_t *)blink_dots_timer_start, NULL);
    os_timer_arm(&dots_blink_timer, BLINK_DOTS_TIME, 1);
}

void ICACHE_FLASH_ATTR 
display_time(uint32_t epoch, uint8_t GMT)
{
    epoch += GMT * 60 * 60;
    dots_blink_flag = true;
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
    dots_blink_flag = false;

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
    dots_blink_flag = false;

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
    dots_blink_flag = false;

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
    dots_blink_flag = false;
    
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
    dots_blink_flag = false;
    
    uint8_t arr[4] = {0};

    if(wind_gust / 10 == 0) arr[0] = EMPTY_SYMBOL;
    else arr[0] = wind_gust / 10;
    arr[1] = wind_gust % 10;
    arr[2] = WIND_SYMBOL;
    arr[3] = WIND_SYMBOL;

    tm1637_display(arr);
}

void ICACHE_FLASH_ATTR 
display_pressure_mm(uint16_t pressure_mm)
{
    dots_blink_flag = false;
    
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
    dots_blink_flag = false;
    uint8_t arr[4] = {0};

    arr[0] = EMPTY_SYMBOL;
    arr[1] = humidity > 9 ? humidity / 10 : EMPTY_SYMBOL;
    arr[2] = humidity % 10;
    arr[3] = H_SYMBOL;

    tm1637_display(arr);
}

void ICACHE_FLASH_ATTR 
wind_animation_stage(bool new_start)
{
    static uint8_t stage = 0;
    static uint8_t k = 1;
    if(new_start) stage = 0;
    uint8_t arr[] = {EMPTY_SYMBOL, EMPTY_SYMBOL, EMPTY_SYMBOL, EMPTY_SYMBOL};
    arr[stage] = WIND_SYMBOL;
    tm1637_display(arr);
    stage += k;
    if(stage == 3 || stage == 0) k = -k; 
}

void ICACHE_FLASH_ATTR 
down_line_animation_stage(bool new_start)
{
    dots_blink_flag = false;
    tm1637_dots_state(false);
    static uint8_t stage = 1;

    static uint8_t raw_arr[] = {0x00, 0x00, 0x00, 0x00};

    if(new_start) stage = 1;

    switch(stage)
    {
        case 2:
            raw_arr[0] |= 0x08;
            break;
        case 3:
            raw_arr[1] |= 0x08;
            break;
        case 4:
            raw_arr[2] |= 0x08;
            break;
        case 5:
            raw_arr[3] |= 0x08;
            break;
            
    }
    tm1637_display_raw(raw_arr);
    if(++stage == 5)
    {
        stage = 1; 
        os_memset(raw_arr, 0, 4);
    }
}

void ICACHE_FLASH_ATTR 
display_soft_ap(uint8_t clients_quantity)
{
    dots_blink_flag = false;
    uint8_t arr[] = {A_SYMBOL, P_SYMBOL, EMPTY_SYMBOL, clients_quantity};
    tm1637_dots_state(true);
    tm1637_display(arr);
}

void ICACHE_FLASH_ATTR 
display_ip()
{
    dots_blink_flag = false;
    tm1637_dots_state(true);
    uint8_t arr[] = {1, P_SYMBOL, EMPTY_SYMBOL, EMPTY_SYMBOL};
    tm1637_display(arr);
}

void ICACHE_FLASH_ATTR 
display_dec_byte(uint8_t byte)
{
    dots_blink_flag = false;
    tm1637_dots_state(false);
    uint8_t arr[] = {EMPTY_SYMBOL, EMPTY_SYMBOL, EMPTY_SYMBOL, EMPTY_SYMBOL};
    arr[1] = byte > 99 ? byte / 100 : EMPTY_SYMBOL;
    arr[2] = byte > 9 ? byte % 100 / 10 : EMPTY_SYMBOL;
    arr[3] = byte % 10;
    tm1637_display(arr);
}

void ICACHE_FLASH_ATTR
display_no_ap()
{
    dots_blink_flag = false;
    tm1637_dots_state(false);
    uint8_t arr[] = {n_SYMBOL, o_SYMBOL, A_SYMBOL, P_SYMBOL};
    tm1637_display(arr);
}

void ICACHE_FLASH_ATTR 
display_clear()
{
    dots_blink_flag = false;
    tm1637_dots_state(false);
    uint8_t arr[] = {EMPTY_SYMBOL, EMPTY_SYMBOL, EMPTY_SYMBOL, EMPTY_SYMBOL};
    tm1637_display(arr);
}
