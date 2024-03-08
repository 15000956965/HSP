// Exercise #6: timer related peripherals
// https://zhuanlan.zhihu.com/p/572276498?utm_id=0

#include <stdio.h>
#include <string.h>
#include "systick.h"
#include "HSP_MOTOR.h"
#include "Ex3.h"
#include "Ex6.h"

void Ex6_1_servo_sweep(void)
{
	uint16_t pw = 1000;
	
	hsp_pwm_init();
	
	while(1)
	{
		pw += 10;
//		if(pw > 2520)
//			pw = 520;
		if(pw > 1900)
			pw = 1100;
		hsp_servo_angle(SERVO1, pw);
		hsp_servo_angle(SERVO2, pw);
		hsp_servo_angle(SERVO3, pw);
		hsp_servo_angle(SERVO4, pw);
		delay_1ms(20);
	}
}

void Ex6_2_servo_manual(void)
{
	uint16_t pw = 1500;
	uint16_t pwt;
	uint8_t state_pha, state_phb;
	uint8_t state_pha_t, state_phb_t;
	
	hsp_tft18_init();
	hsp_pwm_init();
	pwt = pw;

	state_pha = PHA2();			state_phb = PHB2();
	state_pha_t = state_pha;	state_phb_t = state_phb;
	
	while(1)
	{
        if (!PUSH())			// push button pressed        
        {
            delay_1ms(50);		// de-jitter
            if (!PUSH())
            {
                while(!PUSH());
                pw = 1500;
            }
        }
		
		state_pha = PHA2();			state_phb = PHB2();
		if((state_pha_t != state_pha) || (state_phb_t != state_phb))
		{
			if(state_phb_t == state_phb)
			{
				if(SET == state_phb)
				{
					if(RESET == state_pha) pw++;
					else pw--;
				}
				else
				{
					if(SET == state_pha) pw++;
					else pw--;
				}
			}
			else
			{
				if(SET == state_pha)
				{
					if(SET == state_phb) pw++;
					else pw--;
				}
				else
				{
					if(RESET == state_pha) pw++;
					else pw--;
				}
			}

			state_pha_t = state_pha;
			state_phb_t = state_phb;
            //delay_1ms(10);		// de-jitter
		}
		
		hsp_tft18_show_int16(8, 0, pw);
		
        // PWM output stage, subjected to steering angle limits
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
			//delay_1ms(20);
		}
	}

}

void Ex6_3_motor_manual(void)
{
	uint16_t dc = 0;
	uint8_t state_pha, state_phb;
	uint8_t state_pha_t, state_phb_t;
	
	hsp_tft18_init();
	hsp_pwm_init();
	MEN_HIGH();			// enable H-bridge

	state_pha = PHA2();			state_phb = PHB2();
	state_pha_t = state_pha;	state_phb_t = state_phb;
	
	while(1)
	{
        if (!PUSH())			// push button pressed        
        {
            delay_1ms(50);		// de-jitter
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
		
		hsp_tft18_show_int16(8, 0, dc);
		
        // PWM output stage, subjected to duty cycle limits
		if(50 < dc)
			dc = 50;
		if(SW1())
		{
			hsp_motor_voltage(MOTORF, dc);		// run forward
		}
		else
		{
			hsp_motor_voltage(MOTORB, dc);		// run backward
		}

	}
}