#ifndef _HSP_CAT9555_H
#define _HSP_CAT9555_H

#include "gd32f4xx.h"
#include "hsp_gpio.h"
#include "hsp_i2c.h"

//#define CAT9555_I2C_BASEADDR    I2C1
//#define CAT9555_I2C_SPEED       400000
//#define CAT9555_I2C_ADDRESS7    0x40
//#define CAT9555_I2C_DATASIZE    2

void hsp_cat9555_init();
void hsp_cat9555_seg7_decimal(uint16_t value);
void hsp_cat9555_seg7_hexadecimal(uint16_t value);
void hsp_cat9555_ledbar(uint16_t value);

#endif