  
#include "ets_sys.h"
#include "osapi.h"
#include "user_interface.h"
#include "espconn.h"
#include "mem.h"

#include "web_page.h"
#include "calibr_struct.h"
//char index_html[] = "<form method=\"POST\" action=\"/\"><input name=\"first\"><input name=\"second\"><input type=\"submit\"></form>";
LOCAL struct espconn esp_conn;
LOCAL esp_tcp esptcp;
    
#define SERVER_LOCAL_PORT         8000

LOCAL void ICACHE_FLASH_ATTR
parse_arg(char* buf, const char* arg_name, char* arg);

LOCAL void ICACHE_FLASH_ATTR
http_response(struct espconn *pespconn, int error, char *html_txt)
{
    char *buffer = NULL;
    int html_length = 0;
    buffer = (char *)os_malloc(256 * sizeof(char));
    if(buffer != NULL)
    {
        if(html_txt != NULL)
        {
            html_length = os_strlen(html_txt);
        }
        else
        {
            html_length = 0;
        }

        os_sprintf(buffer, "HTTP/1.1 %d OK\r\n"
                            "Content-Length: %d\r\n"
                            "Content-Type: text/html\r\n"
                            "Connection: Closed\r\n"
                            "\r\n"
        ,error, html_length);

        if(html_length > 0)
        {
            buffer = (char *)os_realloc(buffer, (256 + html_length) *sizeof(char));
            os_strcat(buffer, html_txt);
        }

        espconn_sent(pespconn, buffer, strlen(buffer));
        os_free(buffer);
    }
}

LOCAL void ICACHE_FLASH_ATTR
tcp_server_sent_cb(void *arg)
{
    //data sent successfully
    
    os_printf("tcp sent cb \r\n");
}
    

#define POST_ARGUMENT_MAX_SIZE 20

LOCAL void ICACHE_FLASH_ATTR
tcp_server_recv_cb(void *arg, char *pusrdata, unsigned short length)
{
    char *ptr = 0;
    //received some data from tcp connection
    
    struct espconn *pespconn = arg;
   // os_printf("tcp recv : %s \r\n", pusrdata);
    ptr = (char *)os_strstr(pusrdata, "\r\n");
    ptr[0] = '\0';
    if (os_strcmp(pusrdata, "GET / HTTP/1.1") == 0)
    {
        http_response(pespconn, 200, (char *)index_html);
    }
    if (os_strcmp(pusrdata, "POST / HTTP/1.1") == 0)
    {
        ptr[0] = '\r';

        char argument[POST_ARGUMENT_MAX_SIZE] = "";

        parse_arg(pusrdata, "tmpTime", argument);
        calibr.tmp_time = atoi(argument);
        memset(argument, 0, POST_ARGUMENT_MAX_SIZE);
        parse_arg(pusrdata, "prsTime", argument);
        calibr.prs_time = atoi(argument);
        memset(argument, 0, POST_ARGUMENT_MAX_SIZE);
        parse_arg(pusrdata, "clkTime", argument);
        calibr.clk_time = atoi(argument);
        memset(argument, 0, POST_ARGUMENT_MAX_SIZE);
        parse_arg(pusrdata, "Darkest", argument);
        calibr.dark_adc = atoi(argument);
        memset(argument, 0, POST_ARGUMENT_MAX_SIZE);
        parse_arg(pusrdata, "Brightest", argument);
        calibr.brigh_adc = atoi(argument);
        memset(argument, 0, POST_ARGUMENT_MAX_SIZE);
        parse_arg(pusrdata, "updateTimer", argument);
        calibr.meteo_update_timer = atoi(argument);
        memset(argument, 0, POST_ARGUMENT_MAX_SIZE);
        parse_arg(pusrdata, "updateClkTimer", argument);
        calibr.clock_update_timer = atoi(argument);
        memset(argument, 0, POST_ARGUMENT_MAX_SIZE);
        parse_arg(pusrdata, "GMT", calibr.GMT);

        spi_flash_write(0x90000, &calibr, sizeof(calibr_struct));

        http_response(pespconn, 200, (char *)index_html);
    }
}

LOCAL void ICACHE_FLASH_ATTR
parse_arg(char* buf, const char* arg_name, char* arg)
{
    char* data = (char*)os_strstr(buf, arg_name);
    if(!data) return;
    data = os_strstr(data, "=");
    data++; //skip '=' symbol
    for(uint8_t i = 0; i < POST_ARGUMENT_MAX_SIZE; i++)
    {
        if(data[i] == '\0' || data[i] == '&') break;
        arg[i] = data[i];
    }
    os_printf("%s: %s\r\n", arg_name, arg);
}
    
LOCAL void ICACHE_FLASH_ATTR
tcp_server_discon_cb(void *arg)
{
    //tcp disconnect successfully
    
    os_printf("tcp disconnect succeed !!! \r\n");
}
    
LOCAL void ICACHE_FLASH_ATTR
tcp_server_recon_cb(void *arg, sint8 err)
{
    //error occured , tcp connection broke. 
    
    os_printf("reconnect callback, error code %d !!! \r\n",err);
}

LOCAL void ICACHE_FLASH_ATTR
tcp_server_listen(void *arg)
{
    struct espconn *pesp_conn = arg;
    os_printf("tcp_server_listen !!! \r\n");
    
    espconn_regist_recvcb(pesp_conn, tcp_server_recv_cb);
    espconn_regist_reconcb(pesp_conn, tcp_server_recon_cb);
    espconn_regist_disconcb(pesp_conn, tcp_server_discon_cb);
        
    espconn_regist_sentcb(pesp_conn, tcp_server_sent_cb);
}
    
void ICACHE_FLASH_ATTR
user_tcpserver_init(uint32_t port)
{
    esp_conn.type = ESPCONN_TCP;
    esp_conn.state = ESPCONN_NONE;
    esp_conn.proto.tcp = &esptcp;
    esp_conn.proto.tcp->local_port = port;
    espconn_regist_connectcb(&esp_conn, tcp_server_listen);
    
    sint8 ret = espconn_accept(&esp_conn);
        
    os_printf("espconn_accept [%d] !!! \r\n", ret);
    
}

