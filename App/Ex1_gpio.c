#include "Ex1.h"
#include "systick.h"

extern volatile uint32_t sys_tick_counter;

void Ex1_1_led()
{
	// initialize LEDs pattern
	LED_R_ON();
	LED_G_OFF();
	LED_B_ON();
	LED1_OFF();
	LED2_ON();
	
	// sequentially toggle LED state
	while(1)
	{
		delay_1ms(50);	// delay 0.05 second
		LED_R_TOGGLE();
		delay_1ms(50);	// delay 0.05 second
		LED_G_TOGGLE();
		delay_1ms(50);	// delay 0.05 second
		LED_B_TOGGLE();
		delay_1ms(50);	// delay 0.05 second
		LED1_TOGGLE();
		delay_1ms(50);	// delay 0.05 second
		LED2_TOGGLE();
	}
}


void Ex1_2_buzz()
{
	while(1)
	{
		if (PUSH() && S3())
			BUZZ_OFF();
		else
			BUZZ_ON();

		if (KEY1() && S1())
			LED1_OFF();
		else
			LED1_ON();
		if (KEY2() && S2())
			LED2_OFF();
		else
			LED2_ON();
	}
}

void Ex1_3_led_switch()
{
	while(1)
	{
		if(S1() && PUSH())
		  LED_R_OFF();
		else
		  LED_R_ON();
		if(S2() && PUSH())
		  LED_G_OFF();
		else
		  LED_G_ON();
		if(S3() && PUSH())
		  LED_B_OFF();
		else
		  LED_B_ON();
		
		if(SW1() && SW3())
			LED1_OFF();
		else
			LED1_ON();
		if(SW2() && SW4())
			LED2_OFF();
		else
			LED2_ON();
	}
}