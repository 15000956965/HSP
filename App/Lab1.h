#ifndef _LAB1_H
#define _LAB1_H

#include "hsp_gpio.h"
#include "HSP_CAT9555.h"

void Lab1_res_polling();
void Lab1_res_interrupt();
extern volatile uint8_t mode_flag; // 0表示轮询，1表示中断

#endif