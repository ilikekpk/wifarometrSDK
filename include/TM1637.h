#ifndef TM1637_H
#define TM1637_H

#include "osapi.h"

#define A_SYMBOL 10
#define B_SYMBOL 11
#define C_SYMBOL 12
#define E_SYMBOL 13
#define F_SYMBOL 14
#define G_SYMBOL 15
#define H_SYMBOL 16
#define I_SYMBOL 17
#define J_SYMBOL 18
#define L_SYMBOL 19
#define O_SYMBOL 20
#define P_SYMBOL 21
#define S_SYMBOL 22
#define U_SYMBOL 23
#define Y_SYMBOL 24

#define b_SYMBOL 25
#define c_SYMBOL 26
#define d_SYMBOL 27
#define h_SYMBOL 28
#define i_SYMBOL 29
#define j_SYMBOL 30
#define l_SYMBOL 31
#define n_SYMBOL 32
#define o_SYMBOL 33
#define q_SYMBOL 34
#define r_SYMBOL 35
#define u_SYMBOL 36


#define MINUS_SYMBOL 37
#define GRADUS_SYMBOL 38
#define EMPTY_SYMBOL 39
#define WIND_SYMBOL 40

void tm1637_init();
void tm1637_display(uint8_t* disp_data);
void tm1637_display_raw(uint8_t* disp_data);
void tm1637_dots_state(bool state);

#endif
