#include "hsp_liball.h"
#include "Lab1.h"
#include <stdbool.h>
#include "HSP_TFT18.h"

#define DEBOUNCE_TIME 20 // 防抖时间设置为20ms
#define DEFAULT_DEBOUNCE_TIME 20 // 默认的防抖时间，单位为毫秒
#define FAST_ROTATION_THRESHOLD 100 // 快速旋转的时间阈值，单位为毫秒
#define SHORT_DEBOUNCE_TIME 10 // 快速旋转时的短防抖时间，单位为毫秒
#define LONG_DEBOUNCE_TIME 30 // 慢速旋转或静止时的长防抖时间，单位为毫秒

extern uint32_t sys_tick_counter;
extern uint8_t RES_value; //extern是外部变量声明，表示该变量在别的文件中定义
volatile uint8_t lab1_var;  //volatile是声明变量为易失性变量，表示该变量可能会被别的程序修改
extern volatile uint8_t mode_flag; // 0表示轮询，1表示中断


uint32_t getCurrentTime() {
    return sys_tick_counter;
}
volatile uint32_t last_rotation_time = 0; // 上次旋转事件的时间
uint32_t debounce_time = DEFAULT_DEBOUNCE_TIME; // 动态防抖时间，初始值为默认值

void updateDebounceTime() {
    uint32_t current_time = getCurrentTime();
    uint32_t time_interval = current_time - last_rotation_time;

    // 根据旋转速度调整防抖时间
    if (time_interval < FAST_ROTATION_THRESHOLD) {
        debounce_time = SHORT_DEBOUNCE_TIME; // 快速旋转时使用更短的防抖时间
    } else {
        debounce_time = LONG_DEBOUNCE_TIME; // 慢速旋转或静止时使用更长的防抖时间
    }

    last_rotation_time = current_time; // 更新上次旋转事件的时间
}

void Lab1_res_polling()
{
	uint8_t state_pha, state_phb;
	uint8_t state_pha_t, state_phb_t;
	uint8_t pulse_counter = 0;
	
	state_pha = PHA2(),			state_phb = PHB2();
	state_pha_t = state_pha,	state_phb_t = state_phb;
	
	while(1)
	{
		state_pha = PHA2();
		state_phb = PHB2();
		if(state_pha != state_pha_t)	// PHA2 changed
		{
			if(SET == state_pha)
			{
				if(RESET == state_phb)	// 实测逻辑关系
				{
					pulse_counter++;
				}
				else if(pulse_counter>0)
				{
					pulse_counter--;
				}
			}
			state_pha_t = state_pha;
			state_phb_t = state_phb;
		}
		hsp_cat9555_seg7_decimal(pulse_counter);
		
	}
}

void Lab1_res_interrupt()
{
	uint8_t pulse_counter = 0;

    /* PHA2/PB14 interrupt enable */
    rcu_periph_clock_enable(RCU_SYSCFG);
    nvic_irq_enable(EXTI10_15_IRQn, 2U, 0U);
    syscfg_exti_line_config(EXTI_SOURCE_GPIOB, EXTI_SOURCE_PIN14);
    exti_init(EXTI_14, EXTI_INTERRUPT, EXTI_TRIG_RISING);
    exti_interrupt_flag_clear(EXTI_14);

	pulse_counter = RES_value;
	hsp_cat9555_seg7_decimal(pulse_counter);
	while(1)
	{
		if(RES_value != pulse_counter)
		{
			pulse_counter = RES_value;
			hsp_cat9555_seg7_decimal(pulse_counter);
		}
	}
}


void lab1_lcdshow(uint8_t lab1_var) { 

	if ((lab1_var < 10) || (lab1_var > 50 && lab1_var < 80)){
		hsp_tft18_clear(BLUE);
		hsp_tft18_show_uint16_color(36, 4, lab1_var, YELLOW, BLUE);
		// delay_1ms(20); //<10 or 50~80 蓝底黄字
	}
	else if (lab1_var >= 80){
		hsp_tft18_clear(YELLOW);
		hsp_tft18_show_uint16_color(36, 4, lab1_var, RED, YELLOW);
		// delay_1ms(20); //>=80 黄底红字
	}
	else {
		hsp_tft18_clear(BLACK);
		hsp_tft18_show_uint16_color(36, 4, lab1_var, WHITE, BLACK);
		// delay_1ms(10000); //10~50 黑底白字
	}
	
}


void Lab1_mainfunc() {
    uint8_t state_pha, state_phb;
	uint8_t state_pha_t, state_phb_t;
	uint32_t last_debounce_time = 0;
	bool flag;
	
	state_pha = PHA2(),			state_phb = PHB2();
	state_pha_t = state_pha,	state_phb_t = state_phb;

 	/* PHA2/PB14 interrupt enable */
	rcu_periph_clock_enable(RCU_SYSCFG);
    nvic_irq_enable(EXTI10_15_IRQn, 2U, 0U);
	//PHA2/PB14中断使能
    syscfg_exti_line_config(EXTI_SOURCE_GPIOB, EXTI_SOURCE_PIN14);
    exti_init(EXTI_14, EXTI_INTERRUPT, EXTI_TRIG_RISING);
	// exti_init(EXTI_14, EXTI_INTERRUPT, EXTI_TRIG_BOTH); //BOTH有一定问题
	// exti_init(EXTI_14, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(EXTI_14);
	//PHB2/PB13中断使能
	syscfg_exti_line_config(EXTI_SOURCE_GPIOB, EXTI_SOURCE_PIN13);
	exti_init(EXTI_13, EXTI_INTERRUPT, EXTI_TRIG_RISING);
	// exti_init(EXTI_13, EXTI_INTERRUPT, EXTI_TRIG_BOTH); //BOTH有一定问题
	// exti_init(EXTI_13, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
	exti_interrupt_flag_clear(EXTI_13);
	
	//通过SW1拨码选择轮询或者中断方式
	while (1)
	{
	mode_flag = SW1() ? 1 : 0; // 假设SW1()为高时使用中断，低时使用轮询
	if (mode_flag == 0) { //轮询
		
		state_pha = PHA2();
		state_phb = PHB2();
		if (state_pha != state_pha_t || state_phb != state_phb_t)
		{
			if ((getCurrentTime() - last_debounce_time) > debounce_time){
				// 更新最后的防抖时间
                    last_debounce_time = getCurrentTime();

                    if (state_pha != state_pha_t) {
                        if (SET == state_pha) {
                            if (RESET == state_phb) {
                                lab1_var++;
                            } else if (lab1_var > 0) {
                                lab1_var--;
                            }
                        } else {
                            if (SET == state_phb) {
                                lab1_var++;
                            } else if (lab1_var > 0) {
                                lab1_var--;
                            }
                        }
                        state_pha_t = state_pha;
                    }

                    if (state_phb != state_phb_t) {
                        if (SET == state_phb) {
                            if (RESET == state_pha) {
                                lab1_var--;
                            } else if (lab1_var > 0) {
                                lab1_var++;
                            }
                        } else {
                            if (SET == state_pha) {
                                lab1_var--;
                            } else if (lab1_var > 0) {
                                lab1_var++;
                            }
                        }
                        state_phb_t = state_phb;
                    }


                    // last_debounce_time = sys_tick_counter;
					updateDebounceTime();
					// state_pha_t = state_pha;
					// state_phb_t = state_phb;
			}
		}
		
		lab1_lcdshow(lab1_var);
		hsp_cat9555_seg7_hexadecimal(lab1_var);

		if ((lab1_var < 10) || (lab1_var > 50 && lab1_var < 80)) {
        // 调用蜂鸣器以1秒周期鸣叫0.02s的函数
			if (sys_tick_counter >= 1000)
			{
				BUZZ_ON();
				if(flag) hsp_cat9555_seg7_hexadecimal_1(lab1_var);
				else hsp_cat9555_seg7_hexadecimal_2(lab1_var);
			if (sys_tick_counter >= 1020)
			{
				BUZZ_OFF();
				sys_tick_counter = 20;
				if(flag) {hsp_cat9555_seg7_hexadecimal_1(lab1_var);flag = false;}
				else {hsp_cat9555_seg7_hexadecimal_2(lab1_var);flag = true;}
			}
			}	
    	} else if (lab1_var >= 80) {
        // 调用蜂鸣器以0.2秒周期鸣叫0.02s的函数
			if (sys_tick_counter >= 200)
			{
			BUZZ_ON();
			if(flag) hsp_cat9555_seg7_hexadecimal_1(lab1_var);
				else hsp_cat9555_seg7_hexadecimal_2(lab1_var);
			if (sys_tick_counter >= 220)
			{
			BUZZ_OFF();
			sys_tick_counter = 20;
			if(flag) {hsp_cat9555_seg7_hexadecimal_1(lab1_var);flag = false;}
				else {hsp_cat9555_seg7_hexadecimal_2(lab1_var);flag = true;}
			}
			}
   		 }
	
		RES_value = lab1_var;
	}

	else { //中断
	lab1_var = RES_value;
	hsp_cat9555_seg7_hexadecimal(lab1_var);
	
		if(RES_value != lab1_var)
		{
			lab1_var = RES_value;
			hsp_cat9555_seg7_hexadecimal(lab1_var);
		}
		

		//中断的鸣叫部分
		if ((lab1_var < 10) || (lab1_var > 50 && lab1_var < 80)) {
        // 调用蜂鸣器以1秒周期鸣叫0.02s的函数
			if (sys_tick_counter >= 1000)
			{
				BUZZ_ON();
				if(flag) hsp_cat9555_seg7_hexadecimal_1(lab1_var);
				else hsp_cat9555_seg7_hexadecimal_2(lab1_var);
			if (sys_tick_counter >= 1020)
			{
				BUZZ_OFF();
				sys_tick_counter = 20;
				if(flag) {hsp_cat9555_seg7_hexadecimal_1(lab1_var);flag = false;}
				else {hsp_cat9555_seg7_hexadecimal_2(lab1_var);flag = true;}
			}
			}	
    	} else if (lab1_var >= 80) {
        // 调用蜂鸣器以0.2秒周期鸣叫0.02s的函数
			if (sys_tick_counter >= 2000)
			{
				BUZZ_ON();
				if(flag) hsp_cat9555_seg7_hexadecimal_1(lab1_var);
				else hsp_cat9555_seg7_hexadecimal_2(lab1_var);

			if (sys_tick_counter >= 2020)
			{
			BUZZ_OFF();
			sys_tick_counter = 20;
			if(flag) {hsp_cat9555_seg7_hexadecimal_1(lab1_var);flag = false;}
				else {hsp_cat9555_seg7_hexadecimal_2(lab1_var);flag = true;}

			}
			}
   		 }
		 lab1_lcdshow(lab1_var);
	}
	
	if(!PUSH()) {
        lab1_var = 127;
		RES_value = 127;
   	 	}
	}



    // 更新显示（这需要您根据实际使用的显示设备进行实现）
		// hsp_cat9555_seg7_hexadecimal(lab1_var);


}








