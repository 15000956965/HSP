#include "HSP_MT9V034.h"
#include "systick.h"

void hsp_mt9v034_init(void)
{
    uint16_t data=0;

	// use TFT18 LCD for display
	hsp_spi_init();
	hsp_tft18_init();
    hsp_tft18_clear(BLACK);

	// use SCCB for configuration
	hsp_sccb_init();

	delay_1ms(10);

	if(hsp_sccb_read_2byte(MT9V034_CHIP_VERSION, &data) == 0)
	{
		if(data != MT9V034_CHIP_ID)     // Chip Version Register R0x00: 0x1324
		{
			hsp_tft18_show_str_color(120, 0, "Cam", YELLOW, RED);
			hsp_tft18_show_str_color(144, 0, "NG", RED, YELLOW);
			hsp_tft18_show_str_color(8, 3, "MT0V034 error!", WHITE, RED);
			hsp_tft18_show_str_color(8, 5, "Program halted!!", WHITE, RED);
			while(1);
		}
		else
		{
			hsp_tft18_show_str_color(120, 0, "CamOK", BLUE, GREEN);
		}
	}
	else
	{
		hsp_tft18_show_str_color(120, 0, "Cam", YELLOW, RED);
		hsp_tft18_show_str_color(144, 0, "NC", RED, YELLOW);
		hsp_tft18_show_str_color(8, 3, "No MT0V034 detected!", WHITE, RED);
		hsp_tft18_show_str_color(8, 5, "Program halted!!", WHITE, RED);
		while(1);
	}
	
	// configure MT9V034 via SCCB
	hsp_mt9v034_config();
	
	// use DCI for MT9V034 connection
	hsp_dci_init();
}

void hsp_mt9v034_config(void)
{
	/* Restore default settings */
	hsp_mt9v034_set_default();

	/* resolution: 188 * 120 */
	hsp_mt9v034_set_resolution(MT9V034_IMAGEH, MT9V034_IMAGEW);

	/* frame rate: 50 */
    hsp_mt9v034_set_fps(50);

    hsp_mt9v034_set_exposure(CAMERA_AUTO_EXPOSURE);
	hsp_sccb_write_2byte(0x2C, 0x0004);
    hsp_sccb_write_2byte(MT9V034_ANALOG_CTRL, MT9V034_ANTI_ECLIPSE_ENABLE);
    hsp_sccb_write_2byte(MT9V034_ADC_RES_CTRL_REG, 0x0303);
    hsp_sccb_write_2byte(0x13, 0x2D2E);
    hsp_sccb_write_2byte(0x20, CAMERA_CONTRAST);
    hsp_sccb_write_2byte(0x24, 0x0010);
    hsp_sccb_write_2byte(0x2B, 0x0003);
    hsp_sccb_write_2byte(0x2F, 0x0003);
    hsp_sccb_write_2byte(MT9V034_V2_CTRL_REG_A, 0x001A);	//0x32   0x001A
    hsp_sccb_write_2byte(MT9V034_HDR_ENABLE_REG, 0x0103);	//0x0F High Dynamic Range enable,bit is set (R0x0F[1]=1), the sensor uses black level correction values from one green plane, which are applied to all colors.

    hsp_sccb_write_2byte(MT9V034_AGC_AEC_DESIRED_BIN_REG, CAMERA_AUTO_EXPOSURE_BRIGHTNESS);
    hsp_sccb_write_2byte(MT9V034_RESET, 0x03);

}

void hsp_mt9v034_set_default(void)
{
    hsp_sccb_write_2byte(0x01, 0x0001);   //COL_WINDOW_START_CONTEXTA_REG
    hsp_sccb_write_2byte(0x02, 0x0004);   //ROW_WINDOW_START_CONTEXTA_REG
    hsp_sccb_write_2byte(0x03, 0x01E0);   //ROW_WINDOW_SIZE_CONTEXTA_REG
    hsp_sccb_write_2byte(0x04, 0x02F0);   //COL_WINDOW_SIZE_CONTEXTA_REG
    //hsp_sccb_write_2byte(0x05, 0x005E);   //HORZ_BLANK_CONTEXTA_REG
    //hsp_sccb_write_2byte(0x06, 0x002D);   //VERT_BLANK_CONTEXTA_REG
    hsp_sccb_write_2byte(0x05, 0x005B);   //HORZ_BLANK_CONTEXTA_REG
    hsp_sccb_write_2byte(0x06, 0x000A);   //VERT_BLANK_CONTEXTA_REG
    hsp_sccb_write_2byte(0x07, 0x0188);   //CONTROL_MODE_REG
    hsp_sccb_write_2byte(0x08, 0x01BB);   //COARSE_SHUTTER_WIDTH_1_CONTEXTA
    hsp_sccb_write_2byte(0x09, 0x01D9);   //COARSE_SHUTTER_WIDTH_2_CONTEXTA
    hsp_sccb_write_2byte(0x0A, 0x0164);   //SHUTTER_WIDTH_CONTROL_CONTEXTA
    hsp_sccb_write_2byte(0x0B, 0x0000);   //COARSE_SHUTTER_WIDTH_TOTAL_CONTEXTA
    hsp_sccb_write_2byte(0x0C, 0x0000);   //RESET_REG
    hsp_sccb_write_2byte(0x0D, 0x0300);   //READ_MODE_REG
    hsp_sccb_write_2byte(0x0E, 0x0000);   //READ_MODE2_REG
    hsp_sccb_write_2byte(0x0F, 0x0000);   //PIXEL_OPERATION_MODE
    hsp_sccb_write_2byte(0x10, 0x0040);   //RAMP_START_DELAY
    hsp_sccb_write_2byte(0x11, 0x8042);   //OFFSET_CONTROL
    hsp_sccb_write_2byte(0x12, 0x0022);   //AMP_RESET_BAR_CONTROL
    hsp_sccb_write_2byte(0x13, 0x2D2E);   //5T_PIXEL_RESET_CONTROL
    hsp_sccb_write_2byte(0x14, 0x0E02);   //4T_PIXEL_RESET_CONTROL
    hsp_sccb_write_2byte(0x15, 0x0E32);   //TX_CONTROL
    hsp_sccb_write_2byte(0x16, 0x2802);   //5T_PIXEL_SHS_CONTROL
    hsp_sccb_write_2byte(0x17, 0x3E38);   //4T_PIXEL_SHS_CONTROL
    hsp_sccb_write_2byte(0x18, 0x3E38);   //5T_PIXEL_SHR_CONTROL
    hsp_sccb_write_2byte(0x19, 0x2802);   //4T_PIXEL_SHR_CONTROL
    hsp_sccb_write_2byte(0x1A, 0x0428);   //COMPARATOR_RESET_CONTROL
    hsp_sccb_write_2byte(0x1B, 0x0000);   //LED_OUT_CONTROL
    hsp_sccb_write_2byte(0x1C, 0x0302);   //DATA_COMPRESSION
    hsp_sccb_write_2byte(0x1D, 0x0040);   //ANALOG_TEST_CONTROL
    hsp_sccb_write_2byte(0x1E, 0x0000);   //SRAM_TEST_DATA_ODD
    hsp_sccb_write_2byte(0x1F, 0x0000);   //SRAM_TEST_DATA_EVEN
    hsp_sccb_write_2byte(0x20, 0x03C7);   //BOOST_ROW_EN
    hsp_sccb_write_2byte(0x21, 0x0020);   //I_VLN_CONTROL
    hsp_sccb_write_2byte(0x22, 0x0020);   //I_VLN_AMP_CONTROL
    hsp_sccb_write_2byte(0x23, 0x0010);   //I_VLN_CMP_CONTROL
    hsp_sccb_write_2byte(0x24, 0x001B);   //I_OFFSET_CONTROL
//    MTV_IICWriteReg1G=0x25, 0x0000); // I_BANDGAP_CONTROL - TRIMMED PER DIE
    hsp_sccb_write_2byte(0x26, 0x0004);   //I_VLN_VREF_ADC_CONTROL
    hsp_sccb_write_2byte(0x27, 0x000C);   //I_VLN_STEP_CONTROL
    hsp_sccb_write_2byte(0x28, 0x0010);   //I_VLN_BUF_CONTROL
    hsp_sccb_write_2byte(0x29, 0x0010);   //I_MASTER_CONTROL
    hsp_sccb_write_2byte(0x2A, 0x0020);   //I_VLN_AMP_60MHZ_CONTROL
    hsp_sccb_write_2byte(0x2B, 0x0004);   //VREF_AMP_CONTROL
    hsp_sccb_write_2byte(0x2C, 0x0004);   //VREF_ADC_CONTROL
    hsp_sccb_write_2byte(0x2D, 0x0004);   //VBOOST_CONTROL
    hsp_sccb_write_2byte(0x2E, 0x0007);   //V_HI_CONTROL
    hsp_sccb_write_2byte(0x2F, 0x0003);   //V_LO_CONTROL
    hsp_sccb_write_2byte(0x30, 0x0003);   //V_AMP_CAS_CONTROL
    hsp_sccb_write_2byte(0x31, 0x0027);   //V1_CONTROL_CONTEXTA
    hsp_sccb_write_2byte(0x32, 0x001A);   //V2_CONTROL_CONTEXTA
    hsp_sccb_write_2byte(0x33, 0x0005);   //V3_CONTROL_CONTEXTA
    hsp_sccb_write_2byte(0x34, 0x0003);   //V4_CONTROL_CONTEXTA
    hsp_sccb_write_2byte(0x35, 0x0010);   //GLOBAL_GAIN_CONTEXTA_REG
    hsp_sccb_write_2byte(0x36, 0x8010);   //GLOBAL_GAIN_CONTEXTB_REG
    hsp_sccb_write_2byte(0x37, 0x0000);   //VOLTAGE_CONTROL
    hsp_sccb_write_2byte(0x38, 0x0000);   //IDAC_VOLTAGE_MONITOR
    hsp_sccb_write_2byte(0x39, 0x0027);   //V1_CONTROL_CONTEXTB
    hsp_sccb_write_2byte(0x3A, 0x0026);   //V2_CONTROL_CONTEXTB
    hsp_sccb_write_2byte(0x3B, 0x0005);   //V3_CONTROL_CONTEXTB
    hsp_sccb_write_2byte(0x3C, 0x0003);   //V4_CONTROL_CONTEXTB
    hsp_sccb_write_2byte(0x40, 0x0080);   //DARK_AVG_THRESHOLDS
    hsp_sccb_write_2byte(0x46, 0x231D);   //CALIB_CONTROL_REG (AUTO)
    hsp_sccb_write_2byte(0x47, 0x0080);   //STEP_SIZE_AVG_MODE
    hsp_sccb_write_2byte(0x48, 0x0020);   //ROW_NOISE_CONTROL
    hsp_sccb_write_2byte(0x4C, 0x0002);   //NOISE_CONSTANT
    hsp_sccb_write_2byte(0x60, 0x0000);   //PIXCLK_CONTROL
    hsp_sccb_write_2byte(0x67, 0x0000);   //TEST_DATA
    hsp_sccb_write_2byte(0x6C, 0x0000);   //TILE_X0_Y0
    hsp_sccb_write_2byte(0x70, 0x0000);   //TILE_X1_Y0
    hsp_sccb_write_2byte(0x71, 0x002A);   //TILE_X2_Y0
    hsp_sccb_write_2byte(0x72, 0x0000);   //TILE_X3_Y0
    hsp_sccb_write_2byte(0x7F, 0x0000);   //TILE_X4_Y0
    hsp_sccb_write_2byte(0x99, 0x0000);   //TILE_X0_Y1
    hsp_sccb_write_2byte(0x9A, 0x0096);   //TILE_X1_Y1
    hsp_sccb_write_2byte(0x9B, 0x012C);   //TILE_X2_Y1
    hsp_sccb_write_2byte(0x9C, 0x01C2);   //TILE_X3_Y1
    hsp_sccb_write_2byte(0x9D, 0x0258);   //TILE_X4_Y1
    hsp_sccb_write_2byte(0x9E, 0x02F0);   //TILE_X0_Y2
    hsp_sccb_write_2byte(0x9F, 0x0000);   //TILE_X1_Y2
    hsp_sccb_write_2byte(0xA0, 0x0060);   //TILE_X2_Y2
    hsp_sccb_write_2byte(0xA1, 0x00C0);   //TILE_X3_Y2
    hsp_sccb_write_2byte(0xA2, 0x0120);   //TILE_X4_Y2
    hsp_sccb_write_2byte(0xA3, 0x0180);   //TILE_X0_Y3
    hsp_sccb_write_2byte(0xA4, 0x01E0);   //TILE_X1_Y3
    hsp_sccb_write_2byte(0xA5, 0x003A);   //TILE_X2_Y3
    hsp_sccb_write_2byte(0xA6, 0x0002);   //TILE_X3_Y3
    hsp_sccb_write_2byte(0xA8, 0x0000);   //TILE_X4_Y3
    hsp_sccb_write_2byte(0xA9, 0x0002);   //TILE_X0_Y4
    hsp_sccb_write_2byte(0xAA, 0x0002);   //TILE_X1_Y4
    hsp_sccb_write_2byte(0xAB, 0x0040);   //TILE_X2_Y4
    hsp_sccb_write_2byte(0xAC, 0x0001);   //TILE_X3_Y4
    hsp_sccb_write_2byte(0xAD, 0x01E0);   //TILE_X4_Y4
    hsp_sccb_write_2byte(0xAE, 0x0014);   //X0_SLASH5
    hsp_sccb_write_2byte(0xAF, 0x0000);   //X1_SLASH5
    hsp_sccb_write_2byte(0xB0, 0xABE0);   //X2_SLASH5
    hsp_sccb_write_2byte(0xB1, 0x0002);   //X3_SLASH5
    hsp_sccb_write_2byte(0xB2, 0x0010);   //X4_SLASH5
    hsp_sccb_write_2byte(0xB3, 0x0010);   //X5_SLASH5
    hsp_sccb_write_2byte(0xB4, 0x0000);   //Y0_SLASH5
    hsp_sccb_write_2byte(0xB5, 0x0000);   //Y1_SLASH5
    hsp_sccb_write_2byte(0xB6, 0x0000);   //Y2_SLASH5
    hsp_sccb_write_2byte(0xB7, 0x0000);   //Y3_SLASH5
    hsp_sccb_write_2byte(0xBF, 0x0016);   //Y4_SLASH5
    hsp_sccb_write_2byte(0xC0, 0x000A);   //Y5_SLASH5
    hsp_sccb_write_2byte(0xC2, 0x18D0);   //DESIRED_BIN
    hsp_sccb_write_2byte(0xC3, 0x007F);   //EXP_SKIP_FRM_H
    hsp_sccb_write_2byte(0xC4, 0x007F);   //EXP_LPF
    hsp_sccb_write_2byte(0xC5, 0x007F);   //GAIN_SKIP_FRM
    hsp_sccb_write_2byte(0xC6, 0x0000);   //GAIN_LPF_H
    hsp_sccb_write_2byte(0xC7, 0x4416);   //MAX_GAIN
    hsp_sccb_write_2byte(0xC8, 0x4421);   //MIN_COARSE_EXPOSURE
    hsp_sccb_write_2byte(0xC9, 0x0001);   //MAX_COARSE_EXPOSURE
    hsp_sccb_write_2byte(0xCA, 0x0004);   //BIN_DIFF_THRESHOLD
    hsp_sccb_write_2byte(0xCB, 0x01E0);   //AUTO_BLOCK_CONTROL
    hsp_sccb_write_2byte(0xCC, 0x02F0);   //PIXEL_COUNT
    hsp_sccb_write_2byte(0xCD, 0x005E);   //LVDS_MASTER_CONTROL
    hsp_sccb_write_2byte(0xCE, 0x002D);   //LVDS_SHFT_CLK_CONTROL
    hsp_sccb_write_2byte(0xCF, 0x01DE);   //LVDS_DATA_CONTROL
    hsp_sccb_write_2byte(0xD0, 0x01DF);   //LVDS_DATA_STREAM_LATENCY
    hsp_sccb_write_2byte(0xD1, 0x0164);   //LVDS_INTERNAL_SYNC
    hsp_sccb_write_2byte(0xD2, 0x0001);   //LVDS_USE_10BIT_PIXELS
    hsp_sccb_write_2byte(0xD3, 0x0000);   //STEREO_ERROR_CONTROL
    hsp_sccb_write_2byte(0xD4, 0x0000);   //INTERLACE_FIELD_VBLANK
    hsp_sccb_write_2byte(0xD5, 0x0104);   //IMAGE_CAPTURE_NUM
    hsp_sccb_write_2byte(0xD6, 0x0000);   //ANALOG_CONTROLS
    hsp_sccb_write_2byte(0xD7, 0x0000);   //AB_PULSE_WIDTH_REG
    hsp_sccb_write_2byte(0xD8, 0x0000);   //TX_PULLUP_PULSE_WIDTH_REG
    hsp_sccb_write_2byte(0xD9, 0x0000);   //RST_PULLUP_PULSE_WIDTH_REG
    hsp_sccb_write_2byte(0xF0, 0x0000);   //NTSC_FV_CONTROL
    hsp_sccb_write_2byte(0xFE, 0xBEEF);   //NTSC_HBLANK
}

void hsp_mt9v034_set_resolution(uint16_t height, uint16_t width)
{
    uint16_t data = 0;

    if((height*4) <= MAX_IMAGE_HEIGHT)
    {
        height *= 4;
        data |= MT9V034_READ_MODE_ROW_BIN_4;
    }
    else if((height*2) <= MAX_IMAGE_HEIGHT )
    {
        height *= 2;
        data |= MT9V034_READ_MODE_ROW_BIN_2;
    }

    if((width*4) <= MAX_IMAGE_WIDTH )
    {
        width *= 4;
        data |= MT9V034_READ_MODE_COL_BIN_4;
    }
    else if((width*2) <= MAX_IMAGE_WIDTH )
    {
        width *= 2;
        data |= MT9V034_READ_MODE_COL_BIN_2;
    }

    data |= (MT9V034_READ_MODE_ROW_FLIP|MT9V034_READ_MODE_COLUMN_FLIP);
    hsp_sccb_write_2byte(MT9V034_READ_MODE, data);

    hsp_sccb_write_2byte(MT9V034_WINDOW_WIDTH,  width);
    hsp_sccb_write_2byte(MT9V034_WINDOW_HEIGHT, height);

    hsp_sccb_write_2byte(MT9V034_COLUMN_START, MT9V034_COLUMN_START_MIN);
    hsp_sccb_write_2byte(MT9V034_ROW_START, MT9V034_ROW_START_MIN);
}

void hsp_mt9v034_set_fps(uint8_t fps)
{
    float vertical = 0;
	
    if(fps > 200)
    {
        vertical = 2;
    }
    else if(fps >= 150)
    {
        vertical = -1.06 * fps + 251;
    }
    else if(fps > 100)
    {
        vertical = -2.14 * fps + 413;
    }
    else if(fps >= 71)
    {
        vertical = -4.5017 * fps + 649.17;
    }
    else if(fps > 50)
    {
        vertical = -8.8517 * fps + 957.58;
    }
    else if(fps > 20)
    {
        vertical = -31.167 * fps + 2073.3;
    }
    else
    {
        vertical = -155 * fps + 4550;
    }

	hsp_sccb_write_2byte(MT9V034_VERTICAL_BLANKING, (uint16_t)vertical);
}

void hsp_mt9v034_set_exposure(uint8_t enable)
{
    uint16_t reg = 0;

    hsp_sccb_read_2byte(MT9V034_AEC_AGC_ENABLE, &reg);
    
    if(enable)
    {
        /* enable auto exposure gain control */
        hsp_sccb_write_2byte(MT9V034_AEC_AGC_ENABLE, reg | MT9V034_AEC_ENABLE | MT9V034_AGC_ENABLE);
        /* Max exposure time */
        hsp_sccb_write_2byte(MT9V034_MAX_EXPOSURE_REG, CAMERA_MAX_EXPOSURE_TIME);
        /* Min exposure time */
        hsp_sccb_write_2byte(MT9V034_MIN_EXPOSURE_REG, CAMERA_MIN_EXPOSURE_TIME);
        /* Gain setting: 0~60 */
        hsp_sccb_write_2byte(MT9V034_MAX_GAIN_REG, 20);
        /* AEC/AGC pixel count, max to 44000 */
        //hsp_sccb_write_2byte(MT9V034_AGC_AEC_PIXEL_COUNT_REG, 22560);
		hsp_sccb_write_2byte(MT9V034_AGC_AEC_PIXEL_COUNT_REG, MT9V034_IMAGEH * MT9V034_IMAGEW);
    }
    else
    {
        /* disable auto exposure gain control */
        hsp_sccb_write_2byte(MT9V034_AEC_AGC_ENABLE, reg & ~(MT9V034_AEC_ENABLE|MT9V034_AGC_ENABLE));
        /* Max analog gain 64 */
        hsp_sccb_write_2byte(MT9V034_MAX_GAIN_REG, 30);
        /* Exposure time: 0 ~ 32765 */
        hsp_sccb_write_2byte(MT9V034_TOTAL_SHUTTER_WIDTH, CAMERA_EXPOSURE_TIME);
    }
}
