#include "crc8_maxim.h"

uint8_t calculate_crc8_maxim(void* data, uint16_t size)
{
    uint8_t result = 0;
    uint8_t* pointer = (uint8_t*)data;

    for(uint16_t i = size; i != 0; i--)
    {
        uint8_t last_crc = *pointer ^ result;
        result = 0;
        if(last_crc & 0x01) result ^= 0x5e;
        if(last_crc & 0x02) result ^= 0xbc;
        if(last_crc & 0x04) result ^= 0x61;
        if(last_crc & 0x08) result ^= 0xc2;
        if(last_crc & 0x10) result ^= 0x9d;
        if(last_crc & 0x20) result ^= 0x23;
        if(last_crc & 0x40) result ^= 0x46;
        if(last_crc & 0x80) result ^= 0x8c;
        pointer++;
    }
    return result;
}
