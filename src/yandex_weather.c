#include "yandex_weather.h"

LOCAL char* parse_to_comma_or_bracket(char* buf, const char* str);

bool parse_yandex_weather(char* data, yandex_wheather_t* weather) 
{
    char* buffer;

    char* start = os_strstr(data, "fact"); //find start and end of useful data
    char* end = os_strstr(data, "forecast"); 
    if(!end || !start) return false;

    buffer = parse_to_comma_or_bracket(data, "\"now\":"); // timestamp margin name is "now"
    if(!buffer) return false;
    weather->timestamp = atoi(buffer);

    data = start; //apply orders
    *end = '\0';

    

    buffer = parse_to_comma_or_bracket(data, "\"temp\":");
    if(!buffer) return false;
    weather->temp = atoi(buffer);

    buffer = parse_to_comma_or_bracket(data, "\"feels_like\":");
    if(!buffer) weather->feels_like = 0;
    else weather->feels_like = atoi(buffer);

    buffer = parse_to_comma_or_bracket(data, "\"temp_water\":");
    if(!buffer) weather->temp_water = 0;
    else weather->temp_water = atoi(buffer);

    buffer = parse_to_comma_or_bracket(data, "\"wind_speed\":");
    if(!buffer) weather->wind_speed = 0;
    else weather->wind_speed = atoi(buffer);

    buffer = parse_to_comma_or_bracket(data, "\"wind_gust\":");
    if(!buffer) weather->wind_gust = 0;
    else weather->wind_gust = atoi(buffer);

    buffer = parse_to_comma_or_bracket(data, "\"pressure_mm\":");
    if(!buffer) weather->pressure_mm = 0;
    else weather->pressure_mm = atoi(buffer);
    
    buffer = parse_to_comma_or_bracket(data, "\"pressure_pa\":");
    if(!buffer) weather->pressure_pa = 0;
    else weather->pressure_pa = atoi(buffer);

    buffer = parse_to_comma_or_bracket(data, "\"humidity\":");
    if(!buffer) weather->humidity = 0;
    else weather->humidity = atoi(buffer);
    
    print_weather(weather);
    
}


void print_weather(yandex_wheather_t* weather)
{
    os_printf("---------------------------\r\n");
    os_printf("timestamp: %d\r\n\r\n", weather->timestamp);
    os_printf("temp: %d\r\n", weather->temp);
    os_printf("feels_like: %d\r\n", weather->feels_like);
    os_printf("temp_water: %d\r\n", weather->temp_water);
    os_printf("wind_speed: %d\r\n", weather->wind_speed);
    os_printf("wind_gust: %d\r\n", weather->wind_gust);
    os_printf("pressure_mm: %d\r\n", weather->pressure_mm);
    os_printf("pressure_pa: %d\r\n", weather->pressure_pa);
    os_printf("humidity: %d\r\n", weather->humidity);
    os_printf("---------------------------\r\n");
}


LOCAL char* parse_to_comma_or_bracket(char* buf, const char* str)
{
    static char result_buf[10] = {0};
    memset(result_buf, 0, 10);
    buf = os_strstr(buf, str);
    if(!buf) return NULL;
    buf += os_strlen(str);
    
    for (uint8_t i = 0; i < 10; i++)
    {
        if(buf[i] == ',' || buf[i] == '}') return result_buf;
        result_buf[i] = buf[i];
    }
    

}