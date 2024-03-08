#include "gd32f4xx_it.h"
#include "systick.h"
#include "Lab3.h"

extern uint32_t sys_tick_counter;

void Lab3_test(void)
{
	uint16_t motor_speed;
	  
  	// initialize LCD
	hsp_spi_init();
	hsp_tft18_init();
	hsp_tft18_clear(BLACK);
	
	// initialize ADC/CCD
	hsp_demo_frame_motor();

	while(1)
	{
		if(sys_tick_counter > 4)	// 5ms interval
		{
			motor_speed = timer_counter_read(TIMER3);
			timer_counter_value_config(TIMER3, 0);
			hsp_tft18_show_uint16_color(41, 1, motor_speed, WHITE, BLACK);
			sys_tick_counter = 0;
			
		}

	}

}

void hsp_demo_frame_motor(void)
{
    hsp_tft18_show_str_color(1, 0, "Vcmd:       Vin:    ", WHITE, BLACK);
    hsp_tft18_show_str_color(1, 1, "Fdbk:       MI1:    ", WHITE, BLACK);
    hsp_tft18_show_str_color(1, 2, "            MI2:    ", WHITE, BLACK);
    
    // window for TSL1401 waveform
    hsp_tft18_draw_frame(31, 64, 128, 64, BLUE);
    hsp_tft18_draw_block(32, 65, 128, 63, GRAY1);
}

// https://blog.csdn.net/miku222/article/details/128356748
float hsp_pid_control(motor_pid_t *pid, float error)
{
	//比例
    pid->p_out = pid->kp * pid->error;
	//积分 
    pid->i_out = pid->ki * pid->error;
	//微分
    pid->d_out = pid->kd * (pid->error - 2*pid->error_t + pid->error_2t);
	//增量 = 比例 + 积分 + 微分

	//上上次偏差赋值
    pid->error_2t = pid->error_t;                                              
	//上次偏差赋值
	pid->error_t = pid->error;                                                   
			
}