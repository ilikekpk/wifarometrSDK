  
#include "ets_sys.h"
#include "osapi.h"
#include "user_interface.h"
#include "espconn.h"
#include "mem.h"

#include "index_html.h"
#include "net_sets_html.h"
#include "calibr_struct.h"
//char index_html[] = "<form method=\"POST\" action=\"/\"><input name=\"first\"><input name=\"second\"><input type=\"submit\"></form>";
LOCAL struct espconn esp_conn;
LOCAL esp_tcp esptcp;
    
#define SERVER_LOCAL_PORT         8000

LOCAL char* ICACHE_FLASH_ATTR
parse_arg(char* buf, const char* arg_name);

LOCAL display_msg_t ICACHE_FLASH_ATTR
parse_display_msg(char* buf);

LOCAL void ICACHE_FLASH_ATTR
build_index_html(char* index_html_with_args);

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
    

#define POST_ARGUMENT_MAX_SIZE 256

LOCAL void ICACHE_FLASH_ATTR
tcp_server_recv_cb(void *arg, char *pusrdata, unsigned short length)
{
    char *ptr = 0;
    //received some data from tcp connection
    
    char index_html_with_args[strlen(index_html) + 500];
    memset(index_html_with_args, 0, strlen(index_html) + 500);

    struct espconn *pespconn = arg;
   // os_printf("tcp recv : %s \r\n", pusrdata);
    ptr = (char *)os_strstr(pusrdata, "\r\n");
    ptr[0] = '\0';
    if (os_strcmp(pusrdata, "GET / HTTP/1.1") == 0)
    {
        build_index_html(index_html_with_args);
        http_response(pespconn, 200, (char *)index_html_with_args);
    }
    if (os_strcmp(pusrdata, "POST / HTTP/1.1") == 0)
    {
        ptr[0] = '\r';

        char* tmp_buf;
       // memset(&calibr, 0, sizeof(calibr_struct));
////////////////////////////////////////////////////////////
        tmp_buf = parse_arg(pusrdata, "first_msg");
        calibr.first_msg = parse_display_msg(tmp_buf);

        tmp_buf = parse_arg(pusrdata, "second_msg");
        calibr.second_msg = parse_display_msg(tmp_buf);

        tmp_buf = parse_arg(pusrdata, "third_msg");
        calibr.third_msg = parse_display_msg(tmp_buf);

        tmp_buf = parse_arg(pusrdata, "fourth_msg");
        calibr.fourth_msg = parse_display_msg(tmp_buf);
////////////////////////////////////////////////////////////
        tmp_buf = parse_arg(pusrdata, "first_msg_timer");
        if(tmp_buf) calibr.first_msg_timer = atoi(tmp_buf);

        tmp_buf = parse_arg(pusrdata, "second_msg_timer");
        if(tmp_buf) calibr.second_msg_timer = atoi(tmp_buf);

        tmp_buf = parse_arg(pusrdata, "third_msg_timer");
        if(tmp_buf) calibr.third_msg_timer = atoi(tmp_buf);

        tmp_buf = parse_arg(pusrdata, "fourth_msg_timer");
        if(tmp_buf) calibr.fourth_msg_timer = atoi(tmp_buf);
////////////////////////////////////////////////////////////
        tmp_buf = parse_arg(pusrdata, "dark_adc");
        if(tmp_buf) calibr.dark_adc = atoi(tmp_buf);

        tmp_buf = parse_arg(pusrdata, "brigh_adc");
        if(tmp_buf) calibr.brigh_adc = atoi(tmp_buf);
////////////////////////////////////////////////////////////
        tmp_buf = parse_arg(pusrdata, "meteo_update_timer");
        if(tmp_buf) calibr.meteo_update_timer = atoi(tmp_buf);

        tmp_buf = parse_arg(pusrdata, "clock_update_timer");
        if(tmp_buf) calibr.clock_update_timer = atoi(tmp_buf);

        tmp_buf = parse_arg(pusrdata, "GMT"); //temping solution
        memcpy(calibr.GMT, tmp_buf, 10);  
////////////////////////////////////////////////////////////
        tmp_buf = parse_arg(pusrdata, "latitude");
        memcpy(calibr.latitude, tmp_buf, 20);  

        tmp_buf = parse_arg(pusrdata, "longitude");
        memcpy(calibr.longitude, tmp_buf, 20); 
////////////////////////////////////////////////////////////

        spi_flash_erase_sector(0x8c);
        uint32_t ret = spi_flash_write(0x8c000, &calibr, sizeof(calibr_struct));

        print_calibr();

        build_index_html(index_html_with_args);
        http_response(pespconn, 200, (char *)index_html_with_args);
    }
    if (os_strcmp(pusrdata, "GET /net_sets HTTP/1.1") == 0)
    {
        char net_sets_html_with_args[strlen(net_sets_html) + 50];
        os_sprintf(net_sets_html_with_args, net_sets_html, calibr.ssid, calibr.hostname);
        http_response(pespconn, 200, (char *)net_sets_html_with_args);
    }
    if (os_strcmp(pusrdata, "POST /net_sets HTTP/1.1") == 0)
    {
        ptr[0] = '\r';

        memset(calibr.hostname, 0, HOSTNAME_BUF_SIZE);
        memset(calibr.ssid, 0, SSID_BUF_SIZE);

        char* tmp_buf;

        // tmp_buf = parse_arg(pusrdata, "hostname");
        // memcpy(calibr.hostname, tmp_buf, strlen(tmp_buf));

        tmp_buf = parse_arg(pusrdata, "ssid");
        if(strlen(tmp_buf) <= SSID_BUF_SIZE) memcpy(calibr.ssid, tmp_buf, strlen(tmp_buf));

        tmp_buf = parse_arg(pusrdata, "hostname");
        if(strlen(tmp_buf) <= HOSTNAME_BUF_SIZE) memcpy(calibr.hostname, tmp_buf, strlen(tmp_buf));

        tmp_buf = parse_arg(pusrdata, "passwd");
        if(os_strcmp(tmp_buf, "00000") != 0)
        {
            os_printf("passwd: %s\r\n", tmp_buf);
            memset(calibr.passwd, 0, PASSWD_BUF_SIZE);
            if(strlen(tmp_buf) <= PASSWD_BUF_SIZE) memcpy(calibr.passwd, tmp_buf, strlen(tmp_buf));
        } 

        tmp_buf = parse_arg(pusrdata, "yandex_api_key");
        if(os_strcmp(tmp_buf, "0000000000000000") != 0)
        {
            os_printf("yandex_api_key: %s\r\n", tmp_buf);
            memset(calibr.yandex_api_key, 0, YANDEX_API_KEY_BUF_SIZE);
            if(strlen(tmp_buf) <= YANDEX_API_KEY_BUF_SIZE) memcpy(calibr.yandex_api_key, tmp_buf, strlen(tmp_buf));
        } 

        spi_flash_erase_sector(0x8c);
        spi_flash_write(0x8c000, &calibr, sizeof(calibr_struct));

        print_calibr();

        char net_sets_html_with_args[strlen(net_sets_html) + 50];
        os_sprintf(net_sets_html_with_args, net_sets_html, calibr.ssid, calibr.hostname);
        http_response(pespconn, 200, (char *)net_sets_html_with_args);
    }
}

LOCAL void ICACHE_FLASH_ATTR
build_index_html(char* index_html_with_args)
{
    os_sprintf(index_html_with_args, index_html, 
                   calibr.first_msg, 
                   calibr.second_msg,
                   calibr.third_msg,
                   calibr.fourth_msg,
                   calibr.first_msg_timer, 
                   calibr.second_msg_timer,
                   calibr.third_msg_timer,
                   calibr.fourth_msg_timer,
                   calibr.dark_adc,
                   calibr.brigh_adc,
                   999,
                   calibr.meteo_update_timer,
                   calibr.clock_update_timer,
                   calibr.GMT,
                   calibr.latitude,
                   calibr.longitude
    );
}

LOCAL display_msg_t ICACHE_FLASH_ATTR
parse_display_msg(char* buf)
{
    os_printf("buf: %s\r\n", buf);
    if(!os_strcmp(buf, "disabled")) return DISABLED;
    else if(!os_strcmp(buf, "temp")) return TEMP;
    else if(!os_strcmp(buf, "feels_like")) return FEELS_LIKE;
    else if(!os_strcmp(buf, "temp_water")) return TEMP_WATER;
    else if(!os_strcmp(buf, "wind_speed")) return WIND_SPEED;
    else if(!os_strcmp(buf, "wind_gust")) return WIND_GUST;
    else if(!os_strcmp(buf, "pressure_mm")) return PRESSURE_MM;
    else if(!os_strcmp(buf, "pressure_pa")) return PRESSURE_PA;
    else if(!os_strcmp(buf, "humidity")) return HUMIDITY;
    else return DISABLED;
}


LOCAL char* ICACHE_FLASH_ATTR
parse_arg(char* buf, const char* arg_name)
{
    static char res_buf[POST_ARGUMENT_MAX_SIZE] = {0};
    memset(res_buf, 0, POST_ARGUMENT_MAX_SIZE);

    char* data = (char*)os_strstr(buf, arg_name);
    if(!data) return NULL;
    data = os_strstr(data, "=");
    data++; //skip '=' symbol
    for(uint8_t i = 0; i < POST_ARGUMENT_MAX_SIZE; i++)
    {
        if(data[i] == '\0' || data[i] == '&') break;
        res_buf[i] = data[i];
    }
    return res_buf;
    os_printf("%s: %s\r\n", arg_name, res_buf);
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

