#include "hsp_timer.h"
#include "gd32f4xx_gpio.h"
#include "hsp_gpio.h"

static uint16_t pit_counter1=0;


// Motor: Timer0_CH2(PE13/MIN0) / Timer0_CH3(PA11/MIN1)
// Servo: Timer1_CH0(PA0/SERVO2) / Timer1_CH1(PA1/SERVO1)
// Servo: Timer1_CH2(PA2/SERVO3) / Timer1_CH3(PA3/SERVO4)
void hsp_pwm_init(void)
{
	timer_oc_parameter_struct timer_ocintpara;
	timer_parameter_struct timer_initpara;

	// Servo #1~#4: PA0~PA3/AF1 - TIMER1_CH0~CH4
	rcu_periph_clock_enable(RCU_GPIOA);
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, \
				GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, \
				GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	gpio_af_set(GPIOA, GPIO_AF_1, \
				GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	
	// Motor PWM #1: PE13/AF1 - TIMER0_CH2
	rcu_periph_clock_enable(RCU_GPIOE);
	gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_13);
	gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
	gpio_af_set(GPIOE, GPIO_AF_1, GPIO_PIN_13);
	
	// Motor PWM #2: PA11/AF1 - TIMER0_CH3
	rcu_periph_clock_enable(RCU_GPIOA);
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_11);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
	gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_11);

	// ** TIMER0 PWM setting for motor control **
	rcu_periph_clock_enable(RCU_TIMER0);
	rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL2);
	timer_struct_para_init(&timer_initpara);
	timer_deinit(TIMER0);

	/* TIMER0 configuration */
	timer_initpara.prescaler		= 119;
	timer_initpara.alignedmode		= TIMER_COUNTER_EDGE;
	timer_initpara.counterdirection	= TIMER_COUNTER_UP;
	timer_initpara.period			= 100;
	timer_initpara.clockdivision	= TIMER_CKDIV_DIV4;
	timer_initpara.repetitioncounter = 0;
	timer_init(TIMER0, &timer_initpara);

	/* TIMER0 CH2 & CH3 configure in PWM mode 0 */
	timer_channel_output_struct_para_init(&timer_ocintpara);
	timer_ocintpara.ocpolarity		= TIMER_OC_POLARITY_HIGH;
	timer_ocintpara.outputstate		= TIMER_CCX_ENABLE;
	timer_ocintpara.ocnpolarity		= TIMER_OCN_POLARITY_HIGH;
	timer_ocintpara.outputnstate	= TIMER_CCXN_DISABLE;
	timer_ocintpara.ocidlestate		= TIMER_OC_IDLE_STATE_LOW;
	timer_ocintpara.ocnidlestate	= TIMER_OCN_IDLE_STATE_LOW;

	timer_channel_output_config(TIMER0, TIMER_CH_2, &timer_ocintpara);
	timer_channel_output_config(TIMER0, TIMER_CH_3, &timer_ocintpara);

	/* TIMER0 CH2 configure in PWM mode 0, duty cycle 0 */
	timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_2, 0);
	timer_channel_output_mode_config(TIMER0, TIMER_CH_2, TIMER_OC_MODE_PWM0);
	timer_channel_output_shadow_config(TIMER0, TIMER_CH_3, TIMER_OC_SHADOW_DISABLE);
	/* TIMER0 CH3 configure in PWM mode 0, duty cycle 0 */
	timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_3, 0);
	timer_channel_output_mode_config(TIMER0, TIMER_CH_3, TIMER_OC_MODE_PWM0);
	timer_channel_output_shadow_config(TIMER0, TIMER_CH_3, TIMER_OC_SHADOW_DISABLE);

	/* auto-reload preload enable */
	timer_auto_reload_shadow_enable(TIMER0);
	/* important: must included! */
	timer_primary_output_config(TIMER0, ENABLE);
	/* TIMER0 enable */
	timer_enable(TIMER0);

	// ** TIMER1 PWM setting for servo control **
	rcu_periph_clock_enable(RCU_TIMER1);
	//rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL2);
	timer_struct_para_init(&timer_initpara);
	timer_deinit(TIMER1);

	/* TIMER1 configuration */
	//timer_initpara.prescaler		= 31;
    timer_initpara.prescaler		= 119;
	timer_initpara.alignedmode		= TIMER_COUNTER_EDGE;
	timer_initpara.counterdirection	= TIMER_COUNTER_UP;
	timer_initpara.period			= 20000;
	timer_initpara.clockdivision	= TIMER_CKDIV_DIV1;
	timer_initpara.repetitioncounter = 0;
	timer_init(TIMER1, &timer_initpara);

	/* TIMER1 CH0 ~ CH3 configure in PWM mode 0 */
	timer_channel_output_struct_para_init(&timer_ocintpara);
	timer_ocintpara.ocpolarity		= TIMER_OC_POLARITY_HIGH;
	timer_ocintpara.outputstate		= TIMER_CCX_ENABLE;
	timer_ocintpara.ocnpolarity		= TIMER_OCN_POLARITY_HIGH;
	timer_ocintpara.outputnstate	= TIMER_CCXN_DISABLE;
	timer_ocintpara.ocidlestate		= TIMER_OC_IDLE_STATE_LOW;
	timer_ocintpara.ocnidlestate	= TIMER_OCN_IDLE_STATE_LOW;

	timer_channel_output_config(TIMER1, TIMER_CH_0, &timer_ocintpara);
	timer_channel_output_config(TIMER1, TIMER_CH_1, &timer_ocintpara);
	timer_channel_output_config(TIMER1, TIMER_CH_2, &timer_ocintpara);
	timer_channel_output_config(TIMER1, TIMER_CH_3, &timer_ocintpara);

	/* TIMER1 CH0 configure in PWM mode 0, duty cycle 0% */
	timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_0, 1520);
	timer_channel_output_mode_config(TIMER1, TIMER_CH_0, TIMER_OC_MODE_PWM0);
	timer_channel_output_shadow_config(TIMER1, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);
	/* TIMER1 CH1 configure in PWM mode 0, duty cycle 0% */
	timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_1, 1520);
	timer_channel_output_mode_config(TIMER1, TIMER_CH_1, TIMER_OC_MODE_PWM0);
	timer_channel_output_shadow_config(TIMER1, TIMER_CH_1, TIMER_OC_SHADOW_DISABLE);
	/* TIMER1 CH2 configure in PWM mode 0, duty cycle 0% */
	timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_2, 1520);
	timer_channel_output_mode_config(TIMER1, TIMER_CH_2, TIMER_OC_MODE_PWM0);
	timer_channel_output_shadow_config(TIMER1, TIMER_CH_2, TIMER_OC_SHADOW_DISABLE);
	/* TIMER1 CH3 configure in PWM mode 0, duty cycle 0% */
	timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_3, 1520);
	timer_channel_output_mode_config(TIMER1, TIMER_CH_3, TIMER_OC_MODE_PWM0);
	timer_channel_output_shadow_config(TIMER1, TIMER_CH_3, TIMER_OC_SHADOW_DISABLE);

	/* auto-reload preload enable */
	timer_auto_reload_shadow_enable(TIMER1);
	//timer_primary_output_config(TIMER1, ENABLE);
	/* TIMER1 enable */
	timer_enable(TIMER1);
}

// Encoder: Timer3_CH2(PD14/PHB1) / Timer3_CH3(PD15/PHA1)
// https://blog.csdn.net/qq_40696002/article/details/103759101
void hsp_qdec_init(void)
{
	timer_ic_parameter_struct timer_icintpara;
	timer_parameter_struct timer_initpara;

	// Encoder PHB1: PD14/AF2 - Timer3_CH2
	// Encoder PHA1: PD15/AF2 - Timer3_CH3
	rcu_periph_clock_enable(RCU_GPIOD);
	gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, \
	  			GPIO_PIN_14 | GPIO_PIN_15);
	gpio_af_set(GPIOD, GPIO_AF_2, GPIO_PIN_14 | GPIO_PIN_15);
	
	rcu_periph_clock_enable(RCU_TIMER3);
	//rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL2);
	timer_struct_para_init(&timer_initpara);
	timer_deinit(TIMER3);

	/* TIMER3 configuration */
	timer_initpara.prescaler         = 1;
	timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
	timer_initpara.counterdirection  = TIMER_COUNTER_UP;
	timer_initpara.period            = 10000;
	timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
	timer_initpara.repetitioncounter = 0;
	timer_init(TIMER3, &timer_initpara);

	/* TIMER3 CH2 & CH3 configure in encoder mode 0 */
	timer_channel_input_struct_para_init(&timer_icintpara);
	timer_icintpara.icfilter  		= 0x05;
	timer_icintpara.icpolarity 		= TIMER_IC_POLARITY_RISING;
	timer_icintpara.icselection  	= TIMER_IC_SELECTION_DIRECTTI;
	timer_icintpara.icprescaler 	= TIMER_IC_PSC_DIV1;
	timer_input_capture_config(TIMER3, TIMER_CH_0, &timer_icintpara);
	timer_input_capture_config(TIMER3, TIMER_CH_1, &timer_icintpara);

	timer_quadrature_decoder_mode_config(TIMER3, TIMER_QUAD_DECODER_MODE0, TIMER_IC_POLARITY_RISING, TIMER_IC_POLARITY_RISING);

	/* TIMER3 enable */
	timer_enable(TIMER3);
	timer_counter_value_config(TIMER3, 5000);
}

// Use TIMER6 for pit
void hsp_pit_config(void)
{
	/* TIMER configuration:TIMERICLK= 72MHz, prescaler = 36000
	TIMER counter clock = systemcoreclock/36000 = 2KHz.
	TIME= ((1+Prescaler)/72M) * (1+period)*/

	timer_parameter_struct timer_initpara;
	rcu_periph_clock_enable(RCU_TIMER6);
	timer_deinit(TIMER6);
	
	/* TIMER6 configuration */
	//timer_initpara.prescaler		= 35999;
	timer_initpara.prescaler		= 35;
	timer_initpara.alignedmode		= TIMER_COUNTER_EDGE;
	timer_initpara.counterdirection	= TIMER_COUNTER_UP;
	timer_initpara.period			= 999;
	timer_initpara.clockdivision	= TIMER_CKDIV_DIV2;
	timer_initpara.repetitioncounter= 0;
	timer_init(TIMER6, &timer_initpara);
	timer_auto_reload_shadow_enable(TIMER6);
	/* clear the update interrupt bit */
	timer_interrupt_flag_clear(TIMER6, TIMER_INT_UP); 
	/* enable the update interrupt */
	timer_interrupt_enable(TIMER6, TIMER_INT_UP); 
	/* TIMERI counter enable */
	timer_enable(TIMER6);
}

void TIMER6_IRQHandler(void)
{
	if(SET == timer_interrupt_flag_get(TIMER6, TIMER_INT_UP))
	{
		pit_counter1++;
		if(10 < pit_counter1)
		{
			pit_counter1 = 0;
			LED_B_TOGGLE();
		}
	  	timer_interrupt_flag_clear(TIMER6, TIMER_INT_UP);
	}
}

// https://community.st.com/t5/stm32-mcus-products/stm32f4-timer-external-pulse-counting/td-p/527289
// PD12/TIMER3_CH0 conflict with SW2 configure, use only one at a time
void hsp_counter_init(void)
{
	timer_oc_parameter_struct timer_ocintpara;
	timer_parameter_struct timer_initpara;

	// TIMER3_CH1/PD13
	rcu_periph_clock_enable(RCU_GPIOD);
	rcu_periph_clock_enable(RCU_TIMER3);
	gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_13);
	gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
	gpio_af_set(GPIOD, GPIO_AF_2, GPIO_PIN_13);

	timer_initpara.period			= 65535;
	timer_initpara.prescaler		= 0;
	timer_initpara.clockdivision	= TIMER_CKDIV_DIV1;
	timer_initpara.counterdirection	= TIMER_COUNTER_UP;
	timer_init(TIMER3, &timer_initpara);

	timer_external_clock_mode0_config(TIMER3, TIMER_EXT_TRI_PSC_OFF, TIMER_ETP_RISING, 5);
	//timer_input_trigger_source_select(TIMER3, TIMER_SMCFG_TRGSEL_CI1FE1);
	timer_external_trigger_as_external_clock_config(TIMER3, TIMER_SMCFG_TRGSEL_CI1FE1, TIMER_IC_POLARITY_RISING, 5);

	timer_enable(TIMER3);
}