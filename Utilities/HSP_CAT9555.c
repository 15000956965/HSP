#include "HSP_CAT9555.h"

static uint8_t seg7_table[] = \
		{ 0x81, 0xE7, 0x49, 0x43, 0x27, 0x13, 0x11, 0xC7, 0x01, 0x03, 0x05, 0x31, 0x79, 0x61, 0x19, 0x1D };

void hsp_cat9555_init()
{
    uint8_t write_address;
    uint8_t buffer_write[2];

	hsp_i2c_init();
	
    // CAT9555 output polarity
    buffer_write[0] = 0xFFU;
    buffer_write[1] = 0xFFU;
    write_address = 0x04U;
    hsp_i2c_write_2byte(buffer_write, write_address);
    // config IO as outputs
    buffer_write[0] = 0x0U;
    buffer_write[1] = 0x0U;
    write_address = 0x06U;
    hsp_i2c_write_2byte(buffer_write, write_address);
}

void hsp_cat9555_seg7_decimal(uint16_t value)
{
    uint8_t write_address;
    uint8_t buffer_write[2];
	uint16_t delay = 20;
    
	//SEG7_ON();
    write_address = 0x02U;
    buffer_write[0] = 0xFF;
    buffer_write[1] = 0xFF;
    hsp_i2c_write_2byte(buffer_write, write_address);
	while(delay--) {}
	SEG7_ON();
    buffer_write[0] = (seg7_table[(value%100)/10] & 0xFFU);
    buffer_write[1] = (seg7_table[value%10]&0xFFU);
    hsp_i2c_write_2byte(buffer_write, write_address);
}

void hsp_cat9555_seg7_hexadecimal(uint16_t value)
{
    uint8_t write_address;
    uint8_t buffer_write[2];
	uint16_t delay = 20;
    
	//SEG7_ON();
    write_address = 0x02U;
    buffer_write[0] = 0xFF;
    buffer_write[1] = 0xFF;
    hsp_i2c_write_2byte(buffer_write, write_address);
	while(delay--) {}
	SEG7_ON();
    buffer_write[0] = (seg7_table[(value%0x100)/0x10] & 0xFFU);
    buffer_write[1] = (seg7_table[value%0x10]&0xFFU);
    hsp_i2c_write_2byte(buffer_write, write_address);
}

void hsp_cat9555_ledbar(uint16_t value) //用于控制LED灯条
{
    uint8_t write_address;
    uint8_t buffer_write[2];
	uint16_t delay = 50;
    
	//LED_BAR_ON();
    write_address = 0x02U;
    buffer_write[0] = 0xFF;
    buffer_write[1] = 0xFF;
    hsp_i2c_write_2byte(buffer_write, write_address);
	while(delay--) {}
	LED_BAR_ON();
    buffer_write[0] = ~(value & 0xFFU);
    buffer_write[1] = ~((value>>8) & 0xFFU);
    hsp_i2c_write_2byte(buffer_write, write_address);
}
