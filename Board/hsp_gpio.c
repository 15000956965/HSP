#include "hsp_gpio.h"
//#inlcude "gd32f4xx_gpio.h"

// HSP V1.0
void hsp_gpio_init()
{
    /* Enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOE);
    rcu_periph_clock_enable(RCU_GPIOF);
    rcu_periph_clock_enable(RCU_GPIOG);
      
    /* PortA inputs on system board: none */
    /* PortA inputs on base board: none */

    /* PortB inputs on system board: none */
    /* PortB inputs on base board: PHB2/PB13, PHA2/PB14. KEY1/PB15 */
    gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);

    /* PortC inputs on system board: none */
    /* PortC inputs on base board: none */

    // ***jumper wiring***
    /* PortD inputs on system board: none */
    /* PortD inputs on base board: SW4/PD9, SW3/PD10, SW2/PD11, SW1/PD12, KEY2/PD13=>PD15 */
    //gpio_mode_set(GPIOD, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13);
	// PD13 conflict with TIMER3_CH1(to be used for velocity feedback), jumper wiring to PD15 for remedy
    gpio_mode_set(GPIOD, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_15);
    
    /* PortE inputs on system board: none */
    /* PortE inputs on base board: none */

    /* PortF inputs on system board: none */
    /* PortF inputs on base board: none */
   
    /* PortG inputs on system board: S2/PG6, S3/PG7, S1/PG8 */
    /* PortG inputs on base board: PUSH/PG2 */
    gpio_mode_set(GPIOG, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_2 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8);

    /* PortA outputs on system board: led_r/PA7, BUZZ/PA12 */
    /* PortA outputs on base board: LED2/PA8 */
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_12);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_12);
    gpio_bit_set(GPIOA, GPIO_PIN_7);       	// set to turn off led_r
    gpio_bit_reset(GPIOA, GPIO_PIN_8);     	// clear to turn off LED2
    gpio_bit_reset(GPIOA, GPIO_PIN_12);    	// clear to turn off BUZZ

    /* PortB outputs on system board: led_g/PB0, led_b/PB1 */
    /* PortB outputs on base board: CCD2_CLK/PB5 */
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_5);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_5);
    gpio_bit_set(GPIOB, GPIO_PIN_0);       	// set to turn off led_g
    gpio_bit_set(GPIOB, GPIO_PIN_1);       	// set to turn off led_b
    gpio_bit_reset(GPIOB, GPIO_PIN_5);      // clear to initialize TSL1401#2 CLK logic

    /* PortC outputs on system board: none */
    /* PortC outputs on base board: CCD1_SI/PC13 */
    gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_13);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
    gpio_bit_reset(GPIOC, GPIO_PIN_13);     // clear to initialize TSL1401#1 SI logic

    /* PortD outputs on system board: LCD_BL/PD0, LCD_CS/PD1, LCD_DC/PD3, LCD_RST/PD4 */
    /* PortD outputs on base board: CCD1_CLK/PD7 */
    gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_7);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_7);
    gpio_bit_reset(GPIOD, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_4);		// clear to turn off LCD backlight
    gpio_bit_reset(GPIOD, GPIO_PIN_7);      // clear to initialize TSL1401#1 CLK logic

    /* PortE outputs on system board: none */
    /* PortE outputs on base board: none */

    /* PortF outputs on system board: none */
    /* PortF outputs on base board: MEN/PF11 */
    gpio_mode_set(GPIOF, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_11);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
    gpio_bit_reset(GPIOF, GPIO_PIN_11);     // clear to disable motor drive bridge

    /* PortG outputs on system board: SEG_BAR/PG4 */
    /* PortG outputs on base board: LED1/PG3, CCD2_SI/PG15 */
    gpio_mode_set(GPIOG, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_15);
    gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_15);
    gpio_bit_reset(GPIOG, GPIO_PIN_3);      // clear to turn off LED1
    gpio_bit_reset(GPIOG, GPIO_PIN_4);      // clear to use SEG7 display, set to use LED bar graph
    gpio_bit_reset(GPIOG, GPIO_PIN_15);     // clear to initialize TSL1401#2 SI logic
}

uint8_t hsp_get_taskid(void)
{
    return (~((SW4()<<3) + (SW3()<<2) + (SW2()<<1) + SW1())) & 0x0F;
}