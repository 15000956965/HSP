#ifndef _HSP_TSL1401_H
#define _HSP_TSL1401_H

#include "gd32f4xx.h"
#include "systick.h"
#include "hsp_gpio.h"
#include "hsp_adc.h"

#define CCDPIXEL	128U  	// TSL1401ÏñËØµã

typedef uint16_t ccd_t[CCDPIXEL];

void hsp_ccd_init();
void hsp_ccd_flush(void);
void hsp_ccd_snapshot(ccd_t linear_array);
void hsp_ccd_delay(void);

#endif