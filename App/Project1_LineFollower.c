// https://blog.csdn.net/m0_53966219/article/details/126711218
// https://blog.csdn.net/zhuoqingjoking97298/article/details/120093315
// PID: https://zhuanlan.zhihu.com/p/586532545?utm_id=0
// https://blog.csdn.net/weixin_42208428/article/details/122173575
// https://blog.csdn.net/weixin_43964993/article/details/112383192

#include "Project1.h"

extern image2_t image2_use;			// use 1/3 of the original image (40 continuous lines in the middle)
extern image2_t image2_show;		// show 1/3 of the full size screen
extern image2_t image2_temp;		// show 1/3 of the full size screen
extern uint8_t image_ready;			// MT9V034 new data frame ready in buffer

// Project#1: Line Following Robot (LFR)
void Project_LFR(void)
{
	uint16_t pw = 1500, pwt = 0;
	uint16_t dc = 0;
	uint16_t tloss = 0;				// target lost loop counter
	uint8_t state_pha, state_phb;
	uint8_t state_pha_t, state_phb_t;
	
	hsp_tft18_init();
	hsp_mt9v034_init();
	hsp_dci_dma_config();
	hsp_pwm_init();
	MEN_HIGH();					// enable H-bridge

	state_pha = PHA2();			state_phb = PHB2();
	state_pha_t = state_pha;	state_phb_t = state_phb;
	image_ready = RESET;
	
	while(1)
	{
        if (!PUSH())			// push button pressed        
        {
            //delay_1ms(50);		// de-jitter
            if (!PUSH())
            {
                while(!PUSH());
                dc = 0;
            }
        }
		
		state_pha = PHA2();			state_phb = PHB2();
		if((state_pha_t != state_pha) || (state_phb_t != state_phb))
		{
			if(state_phb_t == state_phb)
			{
				if(SET == state_phb)
				{
					if(RESET == state_pha) dc++;
					else if(0 < dc) dc--;
				}
				else
				{
					if(SET == state_pha) dc++;
					else if(0 < dc) dc--;
				}
			}
			else
			{
				if(SET == state_pha)
				{
					if(SET == state_phb) dc++;
					else if(0 < dc) dc--;
				}
				else
				{
					if(RESET == state_pha) dc++;
					else if(0 < dc) dc--;
				}
			}
			state_pha_t = state_pha;
			state_phb_t = state_phb;
            //delay_1ms(10);		// de-jitter
		}
		
		// hsp_tft18_show_int16(8, 0, dc);
        // PWM output stage, subjected to duty cycle limits
		if(35 < dc)
			dc = 35;
		if(SW1())
		{
			hsp_motor_voltage(MOTORF, dc);		// run forward
		}
		else
		{
			hsp_motor_voltage(MOTORB, dc);		// run backward
		}
		
		// camera image processing
		if(image_ready == SET)
		{
			//threshold = hsp_image2_threshold_otsu(image2_use);
			//threshold = hsp_image2_threshold_mean(image2_use);
			//threshold = hsp_image2_threshold_minmax(image2_use);
			//hsp_image2_show_dma(image2_use);
			//hsp_image2_show_dma(image2_show);

			hsp_image2_binary_minmax(image2_use, image2_temp);
			pw = hsp_image_judge(image2_temp);
			if(pw == 0)
			{
				tloss++;
				pw = pwt;		// use previous result
				if(tloss > 10)	// off-road protection
				{
					dc = 0;
					pw = 1500;
					tloss = 0;
				}
			}
			else
			{
				tloss = 0;
			}
			
			// apply steering angle limits
			if(2050 < pw)
				pw = 2050;
			if(1100 > pw)
				pw = 1100;
			if(pwt != pw)
			{
				hsp_servo_angle(SERVO1, pw);
				hsp_servo_angle(SERVO2, pw);
				hsp_servo_angle(SERVO3, pw);
				hsp_servo_angle(SERVO4, pw);
				pwt = pw;
			}
			hsp_tft18_set_region(0, 0, TFT_X_MAX-1, TFT_Y_MAX-1);
			hsp_tft18_show_int16(8, 0, pw);
			
			if(!SW1())
			{
				hsp_image2_show_dma(image2_use);
			}
			else
			{
				hsp_image2_show_dma(image2_temp);
			}
			image_ready = RESET;
		}
	}
}

uint16_t hsp_image_judge(image2_t image)
{
	uint16_t pw;	// pulse-width control steering angle
	uint8_t i, j;
	uint8_t gte_l, gte_r, gte_ok;				// guide tape edge flag
	uint8_t gte_l_idx, gte_r_idx, gte_c_idx;		// guide tape index
	
	gte_l = RESET;
	gte_r = RESET;
	gte_ok = RESET;
	for(i=2; i<(IMAGEW2-2); i++)
	{
		if(RESET == gte_l)
		{
			if((255 == image[20][i]) && (0 == image[20][i+1]))	// left edge found
			{
				gte_l = SET;
				gte_l_idx = i;									// left edge index
			}
		}
		if((SET == gte_l) && (RESET == gte_r))
		{
			if((0 == image[20][i]) && (255 == image[20][i+1]))	// right edge found
			{
				gte_r = SET;
				gte_r_idx = i;									// right edge index
			}
		}
		if((SET == gte_l) && (SET == gte_r) && (RESET == gte_ok))		// both edges found
		{
			if(((gte_r_idx - gte_l_idx) > 6) && ((gte_r_idx - gte_l_idx) < 30))		// proper tape width
			{
				gte_ok = SET;
				gte_c_idx = (gte_r_idx + gte_l_idx) >> 1;	// tape center index
			}
			else
			{
				gte_l = RESET;
				gte_r = RESET;
				gte_ok = RESET;
			}
		}
	}
	
	if(SET == gte_ok)
		pw = 1500 + 10 * (94 - gte_c_idx);
	else
		pw = 0;
	
	return pw;
}
