#include "HSP_MOTOR.h"

// adjust the limit values according to mechanical tunning
#define CH1_LOWER_LIMIT	520U
#define CH1_UPPER_LIMIT	2520U
#define CH2_LOWER_LIMIT	520U
#define CH2_UPPER_LIMIT	2520U
#define CH3_LOWER_LIMIT	520U
#define CH3_UPPER_LIMIT	2520U
#define CH4_LOWER_LIMIT	520U
#define CH4_UPPER_LIMIT	2520U
// adjust the limit value as needed
#define MOTOR_DC_LIMIT	95

// pulse_width: value in unit of millisecond
// pulse_width maximum range: 520 ~ 2520
void hsp_servo_angle(uint8_t channel, uint16_t pulse_width)
{
	uint16_t duty;
	
	duty = pulse_width;
	switch (channel)
	{
		case SERVO1:
			if (CH1_LOWER_LIMIT > pulse_width)
				duty = CH1_LOWER_LIMIT;
			if (CH1_UPPER_LIMIT < pulse_width)
				duty = CH1_UPPER_LIMIT;
			timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_1, duty);
			break;
		case SERVO2:
			if (CH2_LOWER_LIMIT > pulse_width)
				duty = CH2_LOWER_LIMIT;
			if (CH2_UPPER_LIMIT < pulse_width)
				duty = CH2_UPPER_LIMIT;
			timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_0, duty);
			break;
		case SERVO3:
			if (CH3_LOWER_LIMIT > pulse_width)
				duty = CH3_LOWER_LIMIT;
			if (CH3_UPPER_LIMIT < pulse_width)
				duty = CH3_UPPER_LIMIT;
			timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_2, duty);
			break;
		case SERVO4:
			if (CH4_LOWER_LIMIT > pulse_width)
				duty = CH4_LOWER_LIMIT;
			if (CH4_UPPER_LIMIT < pulse_width)
				duty = CH4_UPPER_LIMIT;
			timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_3, duty);
			break;
		default:
			break;
	}
}

// duty_cycle: value in percent
// duty_cycle range: 0 ~ 100
void hsp_motor_voltage(uint8_t dir, uint16_t duty_cycle)
{
	uint16_t dc;
	
	dc = duty_cycle;
	if (MOTOR_DC_LIMIT < duty_cycle)
		dc = MOTOR_DC_LIMIT;
	
	switch (dir)
	{
		case MOTORF:
			timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_2, dc);
			timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_3, 0);
			break;
		case MOTORB:
			timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_2, 0);
			timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_3, dc);
			break;
		default:
			break;
	}
}