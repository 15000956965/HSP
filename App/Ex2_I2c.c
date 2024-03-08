#include <stdio.h>
#include "Ex2.h"
#include "systick.h"

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

void Ex2_4_mems()
{
	uint8_t accl_data[8], temp1;
	uint16_t wx,wy,wz;
	int16_t acc_x=0, acc_y=0, acc_z=0;
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
			
			hsp_tft18_show_int16_color(8, 0, acc_x, WHITE, BLACK);
			hsp_tft18_show_int16_color(8, 1, acc_y, WHITE, BLACK);
			hsp_tft18_show_int16_color(8, 2, acc_z, WHITE, BLACK);
		}
		
		if(!S3()) break;
	}
}