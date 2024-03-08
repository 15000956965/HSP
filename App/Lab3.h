#ifndef _LAB3_H
#define _LAB3_H

#include "hsp_gpio.h"
#include "hsp_spi.h"
#include "hsp_timer.h"
#include "HSP_MOTOR.h"
#include "HSP_TFT18.h"

typedef struct	\
{
	float kp;
	float ki;
	float kd;
	float p_max;
	float i_max;
	float d_max;
	
	float p_out;
	float i_out;
	float d_out;
	
	float error;
	float error_t;
	float error_2t;
} motor_pid_t;

#define hsp_pid_init(_kp, _ki, _kd, max_p, max_i, max_d) \
{						\
	.kp = _kp,			\
	.ki = _ki,			\
	.kd = _kd,			\
	.out_p = 0,			\
	.out_i = 0,			\
	.out_d = 0,			\
	.p_max = max_p,		\
	.i_max = max_i,		\
	.d_max = max_d,		\
}

void Lab3_test(void);
void hsp_demo_frame_motor(void);

#endif