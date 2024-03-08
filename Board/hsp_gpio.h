#ifndef _HSP_GPIO_H
#define _HSP_GPIO_H

#include "gd32f4xx.h"

// GPIO inputs on system board
#define S1()	gpio_input_bit_get(GPIOG, GPIO_PIN_8)
#define S2()	gpio_input_bit_get(GPIOG, GPIO_PIN_6)
#define S3()	gpio_input_bit_get(GPIOG, GPIO_PIN_7)
// GPIO inputs on base board
#define SW1()	gpio_input_bit_get(GPIOD, GPIO_PIN_12)
#define SW2()	gpio_input_bit_get(GPIOD, GPIO_PIN_11)
#define SW3()	gpio_input_bit_get(GPIOD, GPIO_PIN_10)
#define SW4()	gpio_input_bit_get(GPIOD, GPIO_PIN_9)
#define KEY1()	gpio_input_bit_get(GPIOB, GPIO_PIN_15)
#define KEY2()	gpio_input_bit_get(GPIOD, GPIO_PIN_15)
#define PHA2()	gpio_input_bit_get(GPIOB, GPIO_PIN_14)
#define PHB2()	gpio_input_bit_get(GPIOB, GPIO_PIN_13)
#define PUSH()	gpio_input_bit_get(GPIOG, GPIO_PIN_2)

// GPIO outputs on system board
#define LED_R_ON()		gpio_bit_reset(GPIOA, GPIO_PIN_7)
#define LED_R_OFF()		gpio_bit_set(GPIOA, GPIO_PIN_7)
#define LED_R_TOGGLE()	gpio_bit_toggle(GPIOA, GPIO_PIN_7)
#define LED_G_ON()		gpio_bit_reset(GPIOB, GPIO_PIN_0)
#define LED_G_OFF()		gpio_bit_set(GPIOB, GPIO_PIN_0)
#define LED_G_TOGGLE()	gpio_bit_toggle(GPIOB, GPIO_PIN_0)
#define LED_B_ON()		gpio_bit_reset(GPIOB, GPIO_PIN_1)
#define LED_B_OFF()		gpio_bit_set(GPIOB, GPIO_PIN_1)
#define LED_B_TOGGLE()	gpio_bit_toggle(GPIOB, GPIO_PIN_1)
#define BUZZ_ON()		gpio_bit_set(GPIOA, GPIO_PIN_12)
#define BUZZ_OFF()		gpio_bit_reset(GPIOA, GPIO_PIN_12)
#define BUZZ_TOGGLE()	gpio_bit_toggle(GPIOA, GPIO_PIN_12)
#define SEG7_ON()		gpio_bit_reset(GPIOG, GPIO_PIN_4)
#define SEG7_OFF()		gpio_bit_set(GPIOG, GPIO_PIN_4)
#define LED_BAR_ON()	gpio_bit_set(GPIOG, GPIO_PIN_4)
#define LED_BAR_OFF()	gpio_bit_reset(GPIOG, GPIO_PIN_4)
#define LCD_BL_ON()		gpio_bit_set(GPIOD, GPIO_PIN_0)
#define LCD_BL_OFF()	gpio_bit_reset(GPIOD, GPIO_PIN_0);
#define LCD_BL_TOGGLE()	gpio_bit_toggle(GPIOD, GPIO_PIN_0);
#define LCD_CS_LOW()	gpio_bit_reset(GPIOD, GPIO_PIN_1)
#define LCD_CS_HIGH()	gpio_bit_set(GPIOD, GPIO_PIN_1)
#define LCD_DC_LOW()	gpio_bit_reset(GPIOD, GPIO_PIN_3)
#define LCD_DC_HIGH()	gpio_bit_set(GPIOD, GPIO_PIN_3)
#define LCD_RST_LOW()	gpio_bit_reset(GPIOD, GPIO_PIN_4)
#define LCD_RST_HIGH()	gpio_bit_set(GPIOD, GPIO_PIN_4)
// GPIO outputs on base board
#define LED1_ON()       gpio_bit_set(GPIOG, GPIO_PIN_3)
#define LED1_OFF()      gpio_bit_reset(GPIOG, GPIO_PIN_3)
#define LED1_TOGGLE()   gpio_bit_toggle(GPIOG, GPIO_PIN_3)
#define LED2_ON()       gpio_bit_set(GPIOA, GPIO_PIN_8)
#define LED2_OFF()      gpio_bit_reset(GPIOA, GPIO_PIN_8)
#define LED2_TOGGLE()   gpio_bit_toggle(GPIOA, GPIO_PIN_8)
#define MEN_HIGH()      gpio_bit_set(GPIOF, GPIO_PIN_11)
#define MEN_LOW()       gpio_bit_reset(GPIOF, GPIO_PIN_11)
#define CCD1_SI_HIGH	gpio_bit_set(GPIOC, GPIO_PIN_13)
#define CCD1_SI_LOW		gpio_bit_reset(GPIOC, GPIO_PIN_13)
#define CCD1_CLK_HIGH	gpio_bit_set(GPIOD, GPIO_PIN_7)
#define CCD1_CLK_LOW	gpio_bit_reset(GPIOD, GPIO_PIN_7)
#define CCD2_SI_HIGH	gpio_bit_set(GPIOG, GPIO_PIN_15)
#define CCD2_SI_LOW		gpio_bit_reset(GPIOG, GPIO_PIN_15)
#define CCD2_CLK_HIGH	gpio_bit_set(GPIOB, GPIO_PIN_5)
#define CCD2_CLK_LOW	gpio_bit_reset(GPIOB, GPIO_PIN_5)

void hsp_gpio_init();
uint8_t hsp_get_taskid(void);

#endif