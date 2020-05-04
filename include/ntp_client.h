#ifndef NTP_CLIENT_H
#define NTP_CLIENT_H

#include "osapi.h"
#include "user_interface.h"

void ICACHE_FLASH_ATTR 
ntp_get_time(const char * hostname, int port);

extern uint32_t ntp_epoch;

#endif
