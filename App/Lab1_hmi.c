#include "hsp_liball.h"
#include "Lab1.h"
#include <stdbool.h>

extern uint8_t RES_value; //extern是外部变量声明，表示该变量在别的文件中定义
volatile uint8_t lab1_var;  //volatile是声明变量为易失性变量，表示该变量可能会被别的程序修改
extern volatile uint8_t mode_flag; // 0表示轮询，1表示中断
// collect rotary encoder switch signal by polling

// uint8_t state_pha, state_phb;
// 	uint8_t state_pha_t, state_phb_t;
	
// 	state_pha = PHA2(),			state_phb = PHB2();
// 	state_pha_t = state_pha,	state_phb_t = state_phb;


// void Lab1_res_polling()
// {
	
	
// }

// collect rotary encoder switch signal by interrupt
void Lab1_res_interrupt()
{
	// uint8_t pulse_counter = 0;

    /* PHA2/PB14 interrupt enable */
    rcu_periph_clock_enable(RCU_SYSCFG);
    nvic_irq_enable(EXTI10_15_IRQn, 2U, 0U);
    syscfg_exti_line_config(EXTI_SOURCE_GPIOB, EXTI_SOURCE_PIN14);
    exti_init(EXTI_14, EXTI_INTERRUPT, EXTI_TRIG_RISING);
    exti_interrupt_flag_clear(EXTI_14);

	lab1_var = RES_value;
	// hsp_cat9555_seg7_decimal(lab1_var);

		if(RES_value != lab1_var)
		{
			lab1_var = RES_value;
			// hsp_cat9555_seg7_decimal(lab1_var);
		}

}


void Lab1_mainfunc() {
    uint8_t state_pha, state_phb;
	uint8_t state_pha_t, state_phb_t;
	uint8_t pulse_counter = 0;
	
	state_pha = PHA2(),			state_phb = PHB2();
	state_pha_t = state_pha,	state_phb_t = state_phb;

	//通过SW1拨码选择轮询或者中断方式
	while (1)
	{
	mode_flag = SW1() ? 1 : 0; // 假设SW1()为高时使用中断，低时使用轮询
	if (mode_flag == 0) { //轮询
	while (1)
	{
		// lab1_var = 127;
		state_pha = PHA2();
		state_phb = PHB2();
		if(state_pha != state_pha_t)	// PHA2 changed
		{
			if(SET == state_pha)
			{
				if(RESET == state_phb)	// 实测逻辑关系
				{
					lab1_var++;
				}
				else if(lab1_var>0)
				{
					lab1_var--;
				}
			}
			else
			{
				if(SET == state_phb)
				{
					lab1_var++;
				}
				else if(lab1_var>0)
				{
					lab1_var--;
				}
			}
			state_pha_t = state_pha;
			state_phb_t = state_phb;
		}
		if (state_phb != state_phb_t)	// PHB2 changed
		{
			if(SET == state_phb)
			{
				if(RESET == state_pha)
				{
					lab1_var--;
				}
				else if(lab1_var>0)
				{
					lab1_var++;
				}
			}
			else
			{
				if(SET == state_pha)
				{
					lab1_var--;
				}
				else if(lab1_var>0)
				{
					lab1_var++;
				}
			}
			state_pha_t = state_pha;
			state_phb_t = state_phb;
		}
		if(!PUSH()) {
        lab1_var = 127;
   	 }
		
		hsp_cat9555_seg7_hexadecimal(lab1_var);
	}

	}

	else { //中断
		Lab1_res_interrupt();
	}
	}



	// 蜂鸣器逻辑
    	if ((lab1_var < 10) || (lab1_var > 50 && lab1_var < 80)) {
        // 调用蜂鸣器以1秒周期鸣叫0.02s的函数
		
    	} else if (lab1_var >= 80) {
        // 调用蜂鸣器以0.2秒周期鸣叫0.02s的函数

   		 }

    // 更新显示（这需要您根据实际使用的显示设备进行实现）
		// hsp_cat9555_seg7_hexadecimal(lab1_var);
	


}




