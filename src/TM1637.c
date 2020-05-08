#include "TM1637.h"
#include "osapi.h"
#include "gpio.h"

#define CLK_LOW GPIO_OUTPUT_SET(4, 0)
#define CLK_HIGH GPIO_OUTPUT_SET(4, 1)

#define DATA_LOW GPIO_OUTPUT_SET(5, 0)
#define DATA_HIGH GPIO_OUTPUT_SET(5, 1)


//************definitions for TM1637*********************
#define ADDR_AUTO  0x40
#define ADDR_FIXED 0x44

#define STARTADDR  0xC0
/**** definitions for the clock point of the digit tube *******/
#define POINT_ON   1
#define POINT_OFF  0
/**************definitions for brightness***********************/
#define  BRIGHT_DARKEST 0
#define  BRIGHT_TYPICAL 2
#define  BRIGHTEST      7



static void bit_delay();
static void coding(uint8_t* disp_data);
static uint8_t write_byte(uint8_t wr_data);
static void send_array(uint8_t* array);

uint8_t Cmd_SetData = 0x40;
uint8_t Cmd_SetAddr = 0xc0;

uint8_t dots_state = 0; // 0 - off, 0x80 - on

static uint8_t TubeTab[] = {
                            0x3f, 0x06, 0x5b, 0x4f, 
                            0x66, 0x6d, 0x7d, 0x07,
                            0x7f, 0x6f, 
                            
                            0x77, 0x7f, 0x39, 0x79,
                            0x71, 0x3d, 0x76, 0x30, 
                            0x1f, 0x38, 0x3f, 0x73, 
                            0x6d, 0x3e, 0x6e,
                            
                            0x7c, 0x58, 0x5e, 0x74, 
                            0x10, 0x1e, 0x30, 0x54, 
                            0x5c, 0x67, 0x50, 0x1c,

                            0x40, 0x63, 0x00, 0x49
                            };//0~9, A~Z, a~z, -, °, \0, wind, 


/*--------------------------------
0 b 0 0 0 0 0 0 0 0
    : 7 6 5 4 3 2 1
               1
          -----------
          |         |
      6   |         |  2
          |    7    |
          -----------
          |         |
      5   |         |  3
          |         |
          -----------
               4

-----------------------------------*/

void tm1637_init()
{
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4);
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5);
}

static uint8_t write_byte(uint8_t wr_data)
{
    for(uint8_t i = 0; i < 8; i++)
    {
        CLK_LOW;
        if(wr_data >> i & 0x01) DATA_HIGH;
        else DATA_LOW;
        CLK_HIGH;
    }

    CLK_LOW;
    DATA_HIGH;
    CLK_HIGH;
    bit_delay();
}

static void start()
{
    CLK_HIGH;
    DATA_HIGH;
    DATA_LOW;
    CLK_LOW;
}

static void stop()
{
    CLK_LOW;
    DATA_LOW;
    CLK_HIGH;
    DATA_HIGH;
}


uint8_t seg_data[4] = {0}; // segment data, global for points

void tm1637_display(uint8_t* disp_data)
{
  for(uint8_t i = 0; i < 4; i++) seg_data[i] = disp_data[i];

  coding(seg_data);
  send_array(seg_data);
}

void tm1637_dots_state(bool state)
{
  dots_state = state ? 0x80 : 0;
  send_array(seg_data);
}

static void send_array(uint8_t* array)
{
  start();
  write_byte(ADDR_AUTO);
  stop();
  start();
  write_byte(Cmd_SetAddr);
  for(uint8_t i = 0; i < 4; i++) write_byte(array[i] | dots_state);
  stop(); 
  start();
  write_byte(0x88 + 0x07); //bright
  stop();
}

static void coding(uint8_t* disp_data)
{
  for(uint8_t i = 0; i < 4; i++)
  {
    disp_data[i] = TubeTab[disp_data[i]];
  }
}

static void bit_delay()
{
    os_delay_us(50);
}
