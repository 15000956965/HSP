#include <stdio.h>
#include "Ex2.h"
#include "systick.h"
#include "stdbool.h"

int16_t acc_x=0, acc_y=0, acc_z=0;
int16_t prev_acc_x = 0, prev_acc_y = 0, prev_acc_z = 0;
volatile int step_count = 0;
int16_t THRESHOLD = 750;		

int16_t delta_accx = 0;
int16_t delta_accy = 0;
int16_t delta_accz = 0;
int16_t max_delta = 0;
extern volatile uint32_t sys_tick_counter;
//添加一个是否增加了步数的标志，是则为5000，否则为0
int16_t step_flag = 0;

void Ex2_1_seg7()
{
	uint16_t i = 0;
	
	hsp_cat9555_init();
	
	while(1)
	{
		i++;
		delay_1ms(100);		// delay 0.1 second
		if (PUSH())
			hsp_cat9555_seg7_decimal(i);
		else
			hsp_cat9555_seg7_hexadecimal(i);
	}
}

void Ex2_2_ledbar()
{
	uint16_t i = 0;

	// initialize LCD
	hsp_spi_init();
	hsp_tft18_init();
	hsp_tft18_clear(BLACK);
	
	hsp_cat9555_init();

	while(1)
	{
		i++;
		delay_1ms(100);		// delay 0.1 second
		
		if(!SW1())
		{
			if (PUSH())
				hsp_cat9555_ledbar(i);
			else
				hsp_cat9555_ledbar(~i);
		}
		else
		{
			if (PUSH())
				hsp_cat9555_seg7_decimal(i);
			else
				hsp_cat9555_seg7_hexadecimal(i);
		}

		if(!S3()) break;
	}
}

void Ex2_3_segbarmux()
{
	uint16_t i=0, j;

	hsp_cat9555_init();

	while(1)
	{
		i++;
		j = i>>6;
		hsp_cat9555_seg7_hexadecimal(j);
		delay_1ms(3);		// delay 0.003 second
		hsp_cat9555_ledbar(j);
		delay_1ms(3);		// delay 0.003 second
	}

}


//低通滤波实现
// 定义缓冲区大小
#define FILTER_WINDOW_SIZE 8
int16_t filter_buffer[FILTER_WINDOW_SIZE] = {0};
int filter_index = 0;

// 低通滤波函数，通过对一定数量的连续样本取平均来实现
void low_pass_filter(int16_t *input, int16_t *output) {
    filter_buffer[filter_index] = *input; //buffer 的意思是缓冲区
    filter_index = (filter_index + 1) % FILTER_WINDOW_SIZE;

    int32_t sum = 0;
    for (int i = 0; i < FILTER_WINDOW_SIZE; ++i) {
        sum += filter_buffer[i];
    }

    *output = sum / FILTER_WINDOW_SIZE;
}

#define WINDOW_SIZE 8
int16_t window_buffer[WINDOW_SIZE][3]; // 存储X, Y, Z三轴数据
int window_index = 0;

// 将新的加速度数据加入窗口
void add_to_window(int16_t x, int16_t y, int16_t z) {
    window_buffer[window_index][0] = x;
    window_buffer[window_index][1] = y;
    window_buffer[window_index][2] = z;
    window_index = (window_index + 1) % WINDOW_SIZE;
}

#define MIN_STEP_INTERVAL 200 // 假设最短步伐时间为200ms

bool step_detected = false; // 步伐检测标志
bool step_upward = false;   // 步伐上升标志
uint32_t last_step_time = 0; // 上一步的时间戳

bool detect_step(uint32_t current_time) {
    int16_t max_accx = 0, min_accx = 32767;
    int16_t max_accy = 0, min_accy = 32767;
    int16_t max_accz = 0, min_accz = 32767;

    // 遍历窗口中的数据，更新最大和最小加速度
    for (int i = 0; i < WINDOW_SIZE; ++i) {
        if (window_buffer[i][0] > max_accx) max_accx = window_buffer[i][0];
        if (window_buffer[i][0] < min_accx) min_accx = window_buffer[i][0];
        if (window_buffer[i][1] > max_accy) max_accy = window_buffer[i][1];
        if (window_buffer[i][1] < min_accy) min_accy = window_buffer[i][1];
        if (window_buffer[i][2] > max_accz) max_accz = window_buffer[i][2];
        if (window_buffer[i][2] < min_accz) min_accz = window_buffer[i][2];
    }

    // 计算三轴加速度变化的范围
	delta_accx = max_accx - min_accx;
	delta_accy = max_accy - min_accy;
	delta_accz = max_accz - min_accz;

    // 找出最大的加速度变化
	max_delta = delta_accx;
    if (delta_accy > max_delta) max_delta = delta_accy;
    if (delta_accz > max_delta) max_delta = delta_accz;

 	if (max_delta > THRESHOLD) {
        if (!step_upward) {
            step_upward = true; // 标记为上升阶段
        }
    } else if (step_upward) {
        step_upward = false; // 下降阶段
        if (!step_detected && (current_time - last_step_time > MIN_STEP_INTERVAL)) {
            step_detected = true; // 检测到步伐
            last_step_time = current_time; // 更新时间戳
            return true; // 步数增加
        }
    }
    // 如果没有检测到步伐或时间间隔过短，重置标志
    if (current_time - last_step_time > MIN_STEP_INTERVAL) {
        step_detected = false;
    }
    return false; // 保持步数不变


    // // 检测最大的加速度变化是否超过阈值
    // if (max_delta > THRESHOLD) {
    //     return true; // 步数增加
    // }
    
    // return false; // 没有检测到步伐
}

//UI界面显示
void display_results() {

	
	hsp_tft18_show_int16_color(8, 0, acc_x, WHITE, BLACK);
	hsp_tft18_show_int16_color(8, 1, acc_y, WHITE, BLACK);
	hsp_tft18_show_int16_color(8, 2, acc_z, WHITE, BLACK);
	hsp_tft18_show_int16_color(8, 3, step_count, WHITE, BLACK);
}


void Ex2_4_mems()
{
	uint8_t accl_data[8], temp1;
	uint16_t wx,wy,wz;
	int16_t filtered_acc_x = 0, filtered_acc_y = 0, filtered_acc_z = 0;

	uint8_t x1,x2,y1,y2,z1,z2;

	// initialize LCD
	hsp_spi_init();
	hsp_tft18_init();
	hsp_tft18_clear(BLACK);

	hsp_mma8451_config();
	
	while(1)
	{
		hsp_mma8451_read_byte(F_STATUS_REG, &temp1);
		if (temp1 & 0x0F)
		{
//			hsp_mma8451_read_byte(OUT_X_MSB_REG, &x1);
//			hsp_mma8451_read_byte(OUT_X_LSB_REG, &x2);
//			hsp_mma8451_read_byte(OUT_Y_MSB_REG, &y1);
//			hsp_mma8451_read_byte(OUT_Y_LSB_REG, &y2);
//			hsp_mma8451_read_byte(OUT_Z_MSB_REG, &z1);
//			hsp_mma8451_read_byte(OUT_Z_LSB_REG, &z2);
//			
//			acc_x = x1<<8;
//			acc_x |= x2;
//			acc_x >>= 2;
//
//			acc_y = y1<<8;
//			acc_y |= y2;
//			acc_y >>= 2;
//
//			acc_z = z1<<8;
//			acc_z |= z2;
//			acc_z >>= 2;
//			printf("X: %d; Y: %d; Z: %d\n\r", acc_x, acc_y, acc_z);

			hsp_mma8451_read_nbyte(OUT_X_MSB_REG, accl_data, 6);
			wx = (uint16_t)accl_data[0]<<8 | accl_data[1]; 
			if(accl_data[0]>0x7f)
				acc_x = -(int16_t)((~(wx>>2) + 1)&0X3FFF);
			else
				acc_x = (wx>>2) & 0X3FFF;
			wy = (uint16_t)accl_data[2]<<8 | accl_data[3];
			if(accl_data[2]>0x7f)
				acc_y = -(int16_t)((~(wy>>2) + 1)&0X3FFF);
			else
				acc_y = (wy>>2)&0X3FFF;
			wz = (uint16_t)accl_data[4]<<8 | accl_data[5];
			if(accl_data[4]>0x7f)
				acc_z = -(int16_t)((~(wz>>2) + 1)&0X3FFF);
			else
				acc_z = (wz>>2)&0X3FFF;
			//printf("X: %d; Y: %d; Z: %d\n\r", acc_x, acc_y, acc_z);
                   
        //    if(!S1()){
		//    if (abs(acc_x - prev_acc_x) > THRESHOLD || 
        //    abs(acc_y - prev_acc_y) > THRESHOLD || 
        //    abs(acc_z - prev_acc_z) > THRESHOLD)
		//     {
        //    step_count++;
		    // }
			// 低通滤波
			low_pass_filter(&acc_x, &filtered_acc_x);
            low_pass_filter(&acc_y, &filtered_acc_y);
            low_pass_filter(&acc_z, &filtered_acc_z);

			// 将滤波后的数据加入窗口
			add_to_window(filtered_acc_x, filtered_acc_y, filtered_acc_z);

			// 检测是否有步伐
			if (window_index == WINDOW_SIZE - 1) { // 窗口填满，可以进行步伐检测
                if (detect_step(sys_tick_counter)) {
                    step_count++; // 增加步数
					step_flag=5000;
                }
            }


			if(!PUSH()){
				if(step_count!=0)
				{step_count=0;
				delay_1ms(100);
				}

				else 
				  break; 
			}
			
			// 保存前一次加速度值
			prev_acc_x = acc_x;
			prev_acc_y = acc_y;
			prev_acc_z = acc_z;

			display_results();
		   }
		// }
	
		// if(!S3())   break;
	}
}