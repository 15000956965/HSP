#ifndef _HSP_I2C_H
#define _HSP_I2C_H

#include "gd32f4xx.h"
#include "gd32f4xx_i2c.h"

#define HSP_I2C_BASEADDR			I2C1
//#define HSP_I2C_SPEED				400000U	// limited by CAT9555
#define HSP_I2C_SPEED				800000U		// limited by CAT9555
#define HSP_I2C_CAT9555_ADDR7		0x40		// 0x20
#define HSP_I2C_MMA8451_ADDR7		0x38		// 0x1C
#define HSP_I2C_CAT9555_DATASIZE	2
#define HSP_I2C_MMA8451_DATASIZE	64

#define HSP_SCCB_BASEADDR			I2C0
#define HSP_SCCB_CLK				RCU_I2C0
#define HSP_SCCB_SDA_PIN			GPIO_PIN_9
#define HSP_SCCB_SDA_GPIO_PORT		GPIOB
#define HSP_SCCB_SDA_GPIO_CLK		RCU_GPIOB
#define HSP_SCCB_SCL_PIN			GPIO_PIN_8
#define HSP_SCCB_SCL_GPIO_PORT		GPIOB
#define HSP_SCCB_SCL_GPIO_CLK		RCU_GPIOB
#define HSP_SCCB_SPEED				200000U
#define HSP_SCCB_SLAVE_ADDRESS7		0xFE
#define HSP_SCCB_FLAG_TIMEOUT		20000U
#define HSP_SCCB_WRITE_ADDRESS		0xB8
#define HSP_SCCB_READ_ADDRESS		0xB9

void hsp_i2c_init();
void hsp_i2c_write_2byte(uint8_t* p_buffer, uint8_t write_address);
void hsp_sccb_init(void);
uint8_t hsp_sccb_write_byte(uint8_t reg, uint8_t data);
uint8_t hsp_sccb_write_2byte(uint8_t reg, uint16_t data);
uint8_t hsp_sccb_read_byte(uint8_t reg, uint8_t *data);
uint8_t hsp_sccb_read_2byte(uint8_t reg, uint16_t *data);


#endif