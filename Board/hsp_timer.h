#ifndef _HSP_TIMER_H
#define _HSP_TIMER_H

#include "gd32f4xx.h"
#include "gd32f4xx_timer.h"

void hsp_pwm_init(void);
void hsp_counter_init(void);
void hsp_qdec_init(void);
void hsp_pit_config(void);
void TIMER6_IRQHandler(void);


#endif