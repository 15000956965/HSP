// Exercise #8: Real Time Clock application

#include <stdio.h>
#include <string.h>
#include "systick.h"
#include "hsp_gpio.h"
#include "HSP_TFT18.h"
#include "Ex8.h"

// RTC

// Menu Cursor bitmap
//const unsigned char Arrow16x16[4][32]={
//   {0x00,0x00,0x80,0x00,0xC0,0x01,0xE0,0x03,    // UP
//    0xF0,0x07,0xF8,0x0F,0xFC,0x1F,0xFE,0x3F,
//    0xE0,0x03,0xE0,0x03,0xE0,0x03,0xE0,0x03,
//    0xE0,0x03,0xE0,0x03,0xE0,0x03,0x00,0x00},
//   {0x00,0x00,0xE0,0x03,0xE0,0x03,0xE0,0x03,    // DOWN
//    0xE0,0x03,0xE0,0x03,0xE0,0x03,0xE0,0x03,
//    0xFE,0x3F,0xFC,0x1F,0xF8,0x0F,0xF0,0x07,
//    0xE0,0x03,0xC0,0x01,0x80,0x00,0x00,0x00},
//   {0x00,0x00,0x00,0x01,0x00,0x03,0x00,0x07,    // RIGHT
//    0x00,0x0F,0xFE,0x1F,0xFE,0x3F,0xFE,0x7F,
//    0xFE,0x3F,0xFE,0x1F,0x00,0x0F,0x00,0x07,
//    0x00,0x03,0x00,0x01,0x00,0x00,0x00,0x00},
//   {0x00,0x00,0x80,0x00,0xC0,0x00,0xE0,0x00,    // LEFT
//    0xF0,0x00,0xF8,0x7F,0xFC,0x7F,0xFE,0x7F,
//    0xFC,0x7F,0xF8,0x7F,0xF0,0x00,0xE0,0x00,
//    0xC0,0x00,0x80,0x00,0x00,0x00,0x00,0x00},
//};

void Ex8_1_rtc(void)
{
    char line1[20], line2[20];
	rtc_parameter_struct   rtc_initpara;
	rtc_parameter_struct   rtc_now, rtc_justnow;

	uint8_t year, month, day, hour, minute, second;
	uint8_t tyear, tmonth, tday, thour, tminute, tsecond;
	uint8_t datetime[6];

  	// initialize LCD
	hsp_spi_init();
	hsp_tft18_init();
	hsp_tft18_clear(BLACK);

	// initialize RTC (for once)
	hsp_rtc_setup(24U, 2U, 26U, 11U, 06U, 0U);

    rtc_current_time_get(&rtc_now);
    year = bcd2bin(rtc_now.year);   month = bcd2bin(rtc_now.month);     day = bcd2bin(rtc_now.date);
    hour = bcd2bin(rtc_now.hour);   minute = bcd2bin(rtc_now.minute);   second = bcd2bin(rtc_now.second);
    datetime[0] = year;     datetime[1] = month;        datetime[2] = day;
    datetime[3] = hour;     datetime[4] = minute;       datetime[5] = second;
	
	while(1)
	{
		rtc_current_time_get(&rtc_now);
		year = bcd2bin(rtc_now.year);   month = bcd2bin(rtc_now.month);     day = bcd2bin(rtc_now.date);
		hour = bcd2bin(rtc_now.hour);   minute = bcd2bin(rtc_now.minute);   second = bcd2bin(rtc_now.second);

		if (tsecond != second)
        {
            tyear = year;   tmonth = month;     tday = day;
            thour = hour;   tminute = minute;   tsecond = second;
            LED1_TOGGLE();
    		sprintf(line1, "%0.2d-%0.2d-%0.2d",year, month, day);
    		sprintf(line2, "%0.2d:%0.2d:%0.2d",hour, minute, second);
			hsp_tft18_show_str(0, 1, line1);
			hsp_tft18_show_str(0, 2, line2);
		}
	}
}

void Ex8_2_rtc_set_local(void)
{

}

void Ex8_3_rtc_set_uart(void)
{

}