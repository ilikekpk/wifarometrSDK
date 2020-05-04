#include "ntp_client.h"
#include "espconn.h"

#define NTP_PACKET_SIZE 48

static struct espconn udp_client;
static esp_udp udp;

uint32_t ntp_epoch = 0;

int ntp_port;

void ICACHE_FLASH_ATTR
UdpRecvCb(void *arg, char *pdata, unsigned short len)
{
	os_printf("---------------ntp time loaded!----------------\r\n");
    ntp_epoch = pdata[40] << 24 | pdata[41] << 16 | pdata[42] << 8 | pdata[43];
    ntp_epoch -= 2208988800;
    os_printf("epoch: %u", ntp_epoch);
}

void ICACHE_FLASH_ATTR
UdpSentCb(void *arg)
{
	os_printf("udp message sended!\n");
}

void ICACHE_FLASH_ATTR
send_ntp_packet(ip_addr_t* addr, int port)
{
    os_printf("ntp send\r\n");

    udp_client.type = ESPCONN_UDP;
	udp_client.proto.udp = &udp;
	udp.local_port = 8080;
    udp.remote_port = port;

    os_memcpy(udp.remote_ip, addr, 4);
    
    char packetBuffer[NTP_PACKET_SIZE];

    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    // Initialize values needed to form NTP request
    // (see URL above for details on the packets)
    packetBuffer[0] = 0b11100011;   // LI, Version, Mode
    packetBuffer[1] = 0;     // Stratum, or type of clock
    packetBuffer[2] = 6;     // Polling Interval
    packetBuffer[3] = 0xEC;  // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12]  = 49;
    packetBuffer[13]  = 0x4E;
    packetBuffer[14]  = 49;
    packetBuffer[15]  = 52;


	espconn_regist_recvcb(&udp_client, UdpRecvCb);
    espconn_regist_sentcb(&udp_client, UdpSentCb);
    espconn_create(&udp_client);
    espconn_sent(&udp_client, packetBuffer, NTP_PACKET_SIZE);
}

LOCAL void ICACHE_FLASH_ATTR 
dns_callback (const char * hostname, ip_addr_t * addr, void * arg)
{
	if (addr == NULL) 
    {
		os_printf("DNS failed for %s\n", hostname);
	}
	else 
    {
		os_printf("DNS found %s " IPSTR "\n", hostname, IP2STR(addr));
        send_ntp_packet(addr, ntp_port);
	}
}

void ICACHE_FLASH_ATTR 
ntp_get_time(const char * hostname, int port)
{
    ntp_port = port;
	os_printf("DNS request\n");

	ip_addr_t addr;
	err_t error = espconn_gethostbyname(&udp_client, hostname, &addr, dns_callback);

	if (error == ESPCONN_INPROGRESS) 
    {
		os_printf("DNS pending\n");
	}
	else if (error == ESPCONN_OK) 
    {
		// Already in the local names table (or hostname was an IP address), execute the callback ourselves.
		dns_callback(hostname, &addr, NULL);
	}
	else 
    {
		if (error == ESPCONN_ARG) 
        {
			os_printf("DNS arg error %s\n", hostname);
		}
		else 
        {
			os_printf("DNS error code %d\n", error);
		}
		dns_callback(hostname, NULL, NULL); // Handle all DNS errors the same way.
	}
}