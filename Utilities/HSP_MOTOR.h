#ifndef _HSP_MOTOR_H
#define _HSP_MOTOR_H

#include "gd32f4xx.h"
#include "hsp_timer.h"
#include "hsp_gpio.h"

#define SERVO1	0
#define SERVO2	1
#define SERVO3	2
#define SERVO4	3
#define MOTORF	0
#define MOTORB	1

void hsp_servo_angle(uint8_t channel, uint16_t pulse_width);
void hsp_motor_voltage(uint8_t dir, uint16_t duty_cycle);

#endif