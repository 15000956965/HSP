#include <stdio.h>
#include <string.h>
#include "systick.h"
#include "Ex3.h"
#include "Ex4.h"

uint16_t adc_raw_data[12];
uint8_t str_line[20];

float potentiometer;
float battery_main;
float temperature;
float battery_rtc;

// ADC_IDATA0(ADC0): Vsense
// ADC_IDATA1(ADC0): POT1
// ADC_IDATA2(ADC0): Vtemp
// ADC_IDATA3(ADC0): Vbat (1/4 voltate of external battery)
void Ex4_1_adc0(void)
{
	//uint16_t i;

  	// initialize LCD
	hsp_spi_init();
	hsp_tft18_init();
	hsp_tft18_clear(BLACK);
	
	// initialize ADC
	hsp_adc_init();
	hsp_adc0_config();
	
	while(1)
	{
		/* ADC software trigger enable */
        adc_software_trigger_enable(ADC0, ADC_INSERTED_CHANNEL);
		/* wait for EOC */
		while(!adc_flag_get(ADC0, ADC_FLAG_EOC));
		
		adc_raw_data[0] = adc_inserted_data_read(ADC0, ADC_INSERTED_CHANNEL_0);
		adc_raw_data[1] = adc_inserted_data_read(ADC0, ADC_INSERTED_CHANNEL_1);
		adc_raw_data[2] = adc_inserted_data_read(ADC0, ADC_INSERTED_CHANNEL_2);
		adc_raw_data[3] = adc_inserted_data_read(ADC0, ADC_INSERTED_CHANNEL_3);
		
		adc_flag_clear(ADC0, ADC_FLAG_EOC);		/* flag cleared automatically by data reading? */

		hsp_tft18_show_uint16(8, 0, adc_raw_data[0]);	// Vsense
		hsp_tft18_show_uint16(8, 1, adc_raw_data[1]);	// POT
		hsp_tft18_show_uint16(8, 2, adc_raw_data[2]);	// Vtemp
		hsp_tft18_show_uint16(8, 3, adc_raw_data[3]);	// Vrtc
		
		battery_main	= (adc_raw_data[0]*5.016*3.3/4096);		// in unit of Volt
		potentiometer	= (adc_raw_data[1]*3.3/4096);			// in unit of Volt
		temperature		= (1.42-adc_raw_data[2]*3.3/4096)*1000/4.35+25;		// in unit of degree
        battery_rtc		= (adc_raw_data[3]*4*3.3/4096);			// in unit of Volt
		
		sprintf(str_line, "-> %0.2fV", battery_main);
		hsp_tft18_show_str(64, 0, str_line);
		sprintf(str_line, "-> %0.2fV", potentiometer);
		hsp_tft18_show_str(64, 1, str_line);
		sprintf(str_line, "-> %0.2fC", temperature);
		hsp_tft18_show_str(64, 2, str_line);
		sprintf(str_line, "-> %0.2fV", battery_rtc);
		hsp_tft18_show_str(64, 3, str_line);

		if(!S3()) break;
		
	}
}

// ADC_IDATA0(ADC1): CCD1 - PC2
// ADC_IDATA1(ADC1): CCD2 - PC3
// ADC_IDATA2(ADC1): MIS1 - PC4
// ADC_IDATA3(ADC1): MIS2 - PC5
void Ex4_2_adc1(void)
{
	//uint16_t i;

  	// initialize LCD
	hsp_spi_init();
	hsp_tft18_init();
	hsp_tft18_clear(BLACK);
	
	// initialize ADC
	hsp_adc_init();
	hsp_adc1_config();

	while(1)
	{
		/* ADC software trigger enable */
        adc_software_trigger_enable(ADC1, ADC_INSERTED_CHANNEL);
		/* wait for EOC */
		while(!adc_flag_get(ADC1, ADC_FLAG_EOC));
		
		adc_raw_data[0] = adc_inserted_data_read(ADC1, ADC_INSERTED_CHANNEL_0);
		adc_raw_data[1] = adc_inserted_data_read(ADC1, ADC_INSERTED_CHANNEL_1);
		adc_raw_data[2] = adc_inserted_data_read(ADC1, ADC_INSERTED_CHANNEL_2);
		adc_raw_data[3] = adc_inserted_data_read(ADC1, ADC_INSERTED_CHANNEL_3);
		
		adc_flag_clear(ADC1, ADC_FLAG_EOC);		/* flag cleared automatically by data reading? */

		hsp_tft18_show_uint16(8, 0, adc_raw_data[0]);	// CCD1
		hsp_tft18_show_uint16(8, 1, adc_raw_data[1]);	// CCD2
		hsp_tft18_show_uint16(8, 2, adc_raw_data[2]);	// MIS1
		hsp_tft18_show_uint16(8, 3, adc_raw_data[3]);	// MIS2

	}
}

// ADC_IDATA0(ADC2): EM-PF3
// ADC_IDATA1(ADC2): EM-PF8
// ADC_IDATA2(ADC2): EM-PF9
// ADC_IDATA3(ADC2): EM-PF10
void Ex4_3_adc2(void)
{
	//uint16_t i=0, j;

  	// initialize LCD
	hsp_spi_init();
	hsp_tft18_init();
	hsp_tft18_clear(BLACK);

	// initialize ADC
	hsp_adc_init();
	hsp_adc2_config();

	while(1)
	{
		/* ADC software trigger enable */
        adc_software_trigger_enable(ADC2, ADC_INSERTED_CHANNEL);
		/* wait for EOC */
		while(!adc_flag_get(ADC2, ADC_FLAG_EOC));
		
		adc_raw_data[0] = adc_inserted_data_read(ADC2, ADC_INSERTED_CHANNEL_0);
		adc_raw_data[1] = adc_inserted_data_read(ADC2, ADC_INSERTED_CHANNEL_1);
		adc_raw_data[2] = adc_inserted_data_read(ADC2, ADC_INSERTED_CHANNEL_2);
		adc_raw_data[3] = adc_inserted_data_read(ADC2, ADC_INSERTED_CHANNEL_3);
		
		adc_flag_clear(ADC2, ADC_FLAG_EOC);		/* flag cleared automatically by data reading? */

		hsp_tft18_show_uint16(8, 0, adc_raw_data[0]);	// CCD1
		hsp_tft18_show_uint16(8, 1, adc_raw_data[1]);	// CCD2
		hsp_tft18_show_uint16(8, 2, adc_raw_data[2]);	// MIS1
		hsp_tft18_show_uint16(8, 3, adc_raw_data[3]);	// MIS2

	}
}

void Ex4_4_adc01(void)
{
	//uint16_t i=0, j;

  	// initialize LCD
	hsp_spi_init();
	hsp_tft18_init();
	hsp_tft18_clear(BLACK);

	// initialize ADC
	hsp_adc_init();
	hsp_adc0_config();
	hsp_adc1_config();
	adc_sync_mode_config(ADC_DAUL_INSERTED_PARALLEL);

	while(1)
	{
		/* ADC software trigger enable */
        adc_software_trigger_enable(ADC0, ADC_INSERTED_CHANNEL);
		/* wait for EOC */
		while(!adc_flag_get(ADC0, ADC_FLAG_EOC));
		//while(!adc_flag_get(ADC1, ADC_FLAG_EOC));

		adc_raw_data[0] = ADC_IDATA0(ADC0);
		adc_raw_data[1] = ADC_IDATA1(ADC0);
		adc_raw_data[2] = ADC_IDATA2(ADC0);
		adc_raw_data[3] = ADC_IDATA3(ADC0);
		adc_raw_data[4] = ADC_IDATA0(ADC1);
		adc_raw_data[5] = ADC_IDATA1(ADC1);
		adc_raw_data[6] = ADC_IDATA2(ADC1);
		adc_raw_data[7] = ADC_IDATA3(ADC1);

		adc_flag_clear(ADC0, ADC_FLAG_EOC);		/* flag cleared automatically by data reading? */
		adc_flag_clear(ADC1, ADC_FLAG_EOC);		/* flag cleared automatically by data reading? */

		hsp_tft18_show_uint16(8, 0, adc_raw_data[0]);	// Vsense
		hsp_tft18_show_uint16(8, 1, adc_raw_data[1]);	// POT
		hsp_tft18_show_uint16(8, 2, adc_raw_data[2]);	// Vtemp
		hsp_tft18_show_uint16(8, 3, adc_raw_data[3]);	// Vrtc
		hsp_tft18_show_uint16(8, 4, adc_raw_data[4]);	// CCD1
		hsp_tft18_show_uint16(8, 5, adc_raw_data[5]);	// CCD2
		hsp_tft18_show_uint16(8, 6, adc_raw_data[6]);	// MIS1
		hsp_tft18_show_uint16(8, 7, adc_raw_data[7]);	// MIS2
	}
}

void Ex4_5_adc012(void)
{
	//uint16_t i=0, j;

  	// initialize LCD
	hsp_spi_init();
	hsp_tft18_init();
	hsp_tft18_clear(BLACK);

	// initialize ADC
	hsp_adc_init();
	hsp_adc0_config();
	hsp_adc1_config();
	hsp_adc2_config();
	adc_sync_mode_config(ADC_ALL_INSERTED_PARALLEL);

	while(1)
	{
		/* ADC software trigger enable */
        adc_software_trigger_enable(ADC0, ADC_INSERTED_CHANNEL);
		/* wait for EOC */
		while(!adc_flag_get(ADC0, ADC_FLAG_EOC));
		//while(!adc_flag_get(ADC1, ADC_FLAG_EOC));
		//while(!adc_flag_get(ADC2, ADC_FLAG_EOC));

		adc_raw_data[0] = ADC_IDATA0(ADC0);
		adc_raw_data[1] = ADC_IDATA1(ADC0);
		adc_raw_data[2] = ADC_IDATA2(ADC0);
		adc_raw_data[3] = ADC_IDATA3(ADC0);
		adc_raw_data[4] = ADC_IDATA0(ADC1);
		adc_raw_data[5] = ADC_IDATA1(ADC1);
		adc_raw_data[6] = ADC_IDATA2(ADC1);
		adc_raw_data[7] = ADC_IDATA3(ADC1);
		adc_raw_data[8] = ADC_IDATA0(ADC2);
		adc_raw_data[9] = ADC_IDATA1(ADC2);
		adc_raw_data[10] = ADC_IDATA2(ADC2);
		adc_raw_data[11] = ADC_IDATA3(ADC2);

		adc_flag_clear(ADC0, ADC_FLAG_EOC);		/* flag cleared automatically by data reading? */
		adc_flag_clear(ADC1, ADC_FLAG_EOC);		/* flag cleared automatically by data reading? */
		adc_flag_clear(ADC2, ADC_FLAG_EOC);		/* flag cleared automatically by data reading? */

		hsp_tft18_show_uint16(8, 0, adc_raw_data[0]);	// Vsense
		hsp_tft18_show_uint16(8, 1, adc_raw_data[1]);	// POT
		hsp_tft18_show_uint16(8, 2, adc_raw_data[2]);	// Vtemp
		hsp_tft18_show_uint16(8, 3, adc_raw_data[3]);	// Vrtc
		hsp_tft18_show_uint16(8, 4, adc_raw_data[4]);	// CCD1
		hsp_tft18_show_uint16(8, 5, adc_raw_data[5]);	// CCD2
		hsp_tft18_show_uint16(8, 6, adc_raw_data[6]);	// MIS1
		hsp_tft18_show_uint16(8, 7, adc_raw_data[7]);	// MIS2
		hsp_tft18_show_uint16(80, 0, adc_raw_data[8]);	// EM-PF3
		hsp_tft18_show_uint16(80, 1, adc_raw_data[9]);	// EM-PF8
		hsp_tft18_show_uint16(80, 2, adc_raw_data[10]);	// EM-PF9
		hsp_tft18_show_uint16(80, 3, adc_raw_data[11]);	// EM-PF10
	}
}


