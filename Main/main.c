/*!
    \file    main.c
    \brief   running led

    \version 2023-06-25, V3.1.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2023, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <LowLevelIOInterface.h>
#include "gd32f4xx.h"
#include "hsp_liball.h"
#include "systick.h"
#include "00_MENU.h"
#include "Ex1.h"
#include "Ex2.h"
#include "Ex3.h"
#include "Ex4.h"
#include "Ex5.h"
#include "Ex6.h"
#include "Ex7.h"
#include "Ex8.h"
#include "Lab3.h"
#include "Lab4.h"
#include "Lab1.h"
#include "Project1.h"
#include "HSP_MMA8451.h"
#include "GFX_FUNCTIONS.h"
//#include "HSP_WRITE.c"

extern volatile uint8_t lab1_var; 
volatile uint8_t mode_flag=0;
volatile uint32_t sys_tick_counter=0;
volatile uint8_t RES_value;
uint8_t task_id=0;       // TaskID set by 4-bit DIP switch
uint8_t menu_id=0;       // MenuItem ID returned by Menu_Loop()

#pragma location = 0X20030000
uint8_t image_raw[22560];
//__no_init uint8_t image_raw[22560][22560];
//__attribute__((aligned(32))) uint8_t image_raw[22560][22560];

/* HSP board initialize */
void board_init()
{
    systick_config();
	hsp_gpio_init();
	hsp_spi_init();
	hsp_cat9555_init();
	//hsp_uart_init();
	//hsp_pit_config();
	hsp_counter_init();
}

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void check_and_beep();



int main(void)
{
  	uint16_t i;
	
	board_init();
	// LCD_BL_ON();
	hsp_spi_init();
	hsp_tft18_init();

	
	// printf("\nHello SJTU HSP!\n\r");
	
    while(1) {
//		menu_id = hsp_menu_loop();
//		
//		if(0 == menu_id)		// GPIO keys and leds
//		{
//			hsp_demo_hmi();
//		}
//		if(1 == menu_id)		// ADC sensor
//		{
//			Ex4_1_adc0();
//		}
//		if(2 == menu_id)		// led bar and seg7 display
//		{
//			Ex2_2_ledbar();
//		}
//		if(3 == menu_id)		// MMA8451 gradienter
//		{
//			Ex2_4_mems();
//		}
//		if(4 == menu_id)		// bitmaps
//		{
//			Ex3_4_bitmap2();
//			delay_1ms(2000);
//		}
//		if(5 == menu_id)		// GIF demo
//		{
//			testAll();
//			delay_1ms(600);
//		}
//		if(6 == menu_id)		// real-time-clock
//		{
//			hsp_rtc_demo();
//			//Ex8_1_rtc();
//		}
//		if(7 == menu_id)		// system information
//		{
//			hsp_demo_hmi();
//		}
		
		//delay_1ms(50);
		//if(!S1())
		//	Ex0_gif_poweron();
		//else
		// Ex1_1_led();
		// Ex1_2_buzz();
		// Ex1_3_led_switch();
		// Ex2_1_seg7();
		// Ex2_2_ledbar();
		//Ex2_3_segbarmux();
		//Ex2_4_mems();
		// Ex3_1_tft18_text();
		// Ex3_2_tft18_menu();
		// Ex3_3_bitmap();

		
		// lab1_lcdshow(8);
		
		





		// Ex4_1_adc0();
		//Ex4_2_adc1();
		//Ex4_3_adc2();
		//Ex4_4_adc01();
		//Ex4_5_adc012();
		//Ex5_2_uart2_opensda();
		//Ex5_3_uart5_wireless();
		//Ex6_1_servo_sweep();
		//Ex6_2_servo_manual();
		//Ex6_3_motor_manual();
		//Ex7_1_dcam();
		//Ex7_2_edge();
		//Ex8_1_rtc();
		// Lab1_res_polling();
		// Lab1_res_interrupt();
	  	//Lab3_test();
	  	//Lab4_test();
		Lab1_mainfunc();
		// 在main.c的主循环中
		// if(mode_flag == 0) {
		// 	Lab1_res_polling();
		// } else {
		// 	// 确保中断已经被配置，这通常只需要做一次
		// 	Lab1_res_interrupt(); // 这个不是在这里调用，而是你确保中断配置好，并让ISR处理
		// }

		// if(mode_flag == 0) {

		// Lab1_res_polling();
		// Ex1_1_led();
			


		// } else {
		// 	// 确保中断已经被配置，这通常只需要做一次
		// 	Lab1_res_interrupt(); // 这个不是在这里调用，而是你确保中断配置好，并让ISR处理
		// }
		// hsp_cat9555_seg7_hexadecimal(lab1_var);




		// Project_LFR(); 
		
		if (!(sys_tick_counter % 20))
		{
			sys_tick_counter = 0;
			LED_R_TOGGLE();
			i = timer_counter_read(TIMER3);
			//hsp_tft18_show_uint16_color(0, 1, i, WHITE, BLACK);
			printf("counter value: %d\n\r", i);
		}
    }
}

