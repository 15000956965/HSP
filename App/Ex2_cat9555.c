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

	hsp_cat9555_init();

	while(1)
	{
		i++;
		delay_1ms(100);		// delay 0.1 second
		if (PUSH())
			hsp_cat9555_ledbar(i);
		else
			hsp_cat9555_ledbar(~i);
	}
}

void Ex2_3_multiplexed()
{
	uint16_t i=0, j;

	hsp_cat9555_init();

	while(1)
	{
		i++;
		j = i>>7;
		hsp_cat9555_seg7_hexadecimal(j);
		delay_1ms(5);		// delay 0.001 second
		hsp_cat9555_ledbar(j);
		delay_1ms(5);		// delay 0.001 second
	}

}