#ifndef NTP_CLIENT_H
#define NTP_CLIENT_H

#include "osapi.h"
#include "user_interface.h"

void ICACHE_FLASH_ATTR 
ntp_get_time(const char * hostname, int port);

bool ICACHE_FLASH_ATTR
get_isUpdated();

extern uint32_t ntp_epoch;

#endif
