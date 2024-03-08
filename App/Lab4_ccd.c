// https://blog.csdn.net/u014545515/article/details/38363511/
// https://www.renrendoc.com/paper/161650011.html


#include "HSP_TSL1401.h"
#include "Lab4.h"

ccd_t ccd_data_raw, ccd_data_old;

void Lab4_test(void)
{
  	uint8_t index;
	
  	// initialize LCD
	hsp_spi_init();
	hsp_tft18_init();
	hsp_tft18_clear(BLACK);
	
	// initialize ADC/CCD
	hsp_ccd_init();
	hsp_demo_frame_ccd();
	
	for(index=0; index<128; index++)
		ccd_data_raw[index] = (index<<5);
	
	while(1)
	{
		hsp_ccd_snapshot(ccd_data_raw);
		hsp_ccd_show(ccd_data_raw);
		
//		for(index=0; index<128; index++)
//		{
//			adc_software_trigger_enable(ADC2, ADC_INSERTED_CHANNEL);
//			while(!adc_flag_get(ADC2, ADC_FLAG_EOC));
//			ccd_data_raw[index] = adc_inserted_data_read(ADC2, ADC_INSERTED_CHANNEL_0);
//		}
//		hsp_ccd_show(ccd_data_raw);

	}
	
}

void Lab4(void)
{

}

// draw LinearCCD waveform by pixels: [32,128] ~ [160,64]
void hsp_ccd_show(ccd_t data)
{
    uint8_t i=0;

    for(i=0; i<CCDPIXEL; i++)
	{
        hsp_tft18_draw_pixel(32+i, 128-(ccd_data_old[i]>>6), GRAY1);
        hsp_tft18_draw_pixel(32+i, 128-(data[i]>>6), BLUE);
        ccd_data_old[i] = data[i];
	}
}

void hsp_demo_frame_ccd(void)
{
    hsp_tft18_show_str_color(1, 0, "ExpT:       Max:    ", WHITE, BLACK);
    hsp_tft18_show_str_color(1, 1, "Mode:       Min:    ", WHITE, BLACK);
    hsp_tft18_show_str_color(1, 2, "            Avg:    ", WHITE, BLACK);
    
    // window for TSL1401 waveform
    hsp_tft18_draw_frame(31, 64, 128, 64, BLUE);
    hsp_tft18_draw_block(32, 65, 128, 63, GRAY1);
}
