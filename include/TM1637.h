#ifndef TM1637_H
#define TM1637_H

#include <stdint.h>

void tm1637_init();
void tm1637_display(uint8_t* disp_data);
void tm1637_dots_state(uint8_t state);

#endif
