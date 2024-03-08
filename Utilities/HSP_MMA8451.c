#include "HSP_MMA8451.h"
#include <stdio.h>

// https://community.nxp.com/t5/Sensors-Knowledge-Base/MMA8451Q-Bare-metal-example-project/ta-p/1127268

void hsp_mma8451_config()
{
    uint8_t write_address;
    uint8_t buffer_write[2];
	uint8_t temp1, reg_val;

	hsp_i2c_mma8451_init();
	
	do
	{
		hsp_mma8451_read_byte(WHO_AM_I_REG, &temp1);
		
	} while(temp1 != MMA8451Q_ID);
	printf("\nMMA8451Q detected\n\r");

//	hsp_mma8451_read_byte(CTRL_REG1, &temp1);
//	hsp_mma8451_write_byte(CTRL_REG2, temp1|0x01);
	hsp_mma8451_write_byte(CTRL_REG1, 0x05);
	hsp_mma8451_write_byte(CTRL_REG2, 0x02);
	hsp_mma8451_write_byte(XYZ_DATA_CFG_REG, 0x10);
	

//	hsp_mma8451_write_byte(CTRL_REG2, 0x40);
//	do
//	{
//		hsp_mma8451_read_byte(CTRL_REG2, &temp1);
//		reg_val = (temp1 & 0x40);
//	} while (reg_val);


//	hsp_mma8451_write_byte(CTRL_REG1, ASLP_RATE_20MS+DATA_RATE_2500US);
//	hsp_mma8451_write_byte(XYZ_DATA_CFG_REG, FULL_SCALE_2G);
//	hsp_mma8451_write_byte(CTRL_REG1, (ACTIVE_MASK+ASLP_RATE_20MS+DATA_RATE_5MS)&(~FREAD_MASK));
	//hsp_mma8451_write_byte(XYZ_DATA_CFG_REG, 0x00);
	//hsp_mma8451_write_byte(CTRL_REG2, 0x02);	// high resolution mode
	//hsp_mma8451_write_byte(CTRL_REG1, 0x3D);	// ODR = 1.56Hz, reduced noise, active mode
}

void hsp_i2c_mma8451_init()
{
    // enable peripheral clock
    rcu_periph_clock_enable(RCU_GPIOF);
    rcu_periph_clock_enable(RCU_I2C1);

    // PF0/I2C1_SDA, PF1/I2C1_SCL
    gpio_af_set(GPIOF, GPIO_AF_4, GPIO_PIN_0 | GPIO_PIN_1);
    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_0 | GPIO_PIN_1);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1);

    // configure I2C clock
    //i2c_clock_config(MMA8451_I2C_BASEADDR, MMA8451_I2C_SPEED, I2C_DTCY_2);
	i2c_clock_config(MMA8451_I2C_BASEADDR, MMA8451_I2C_SPEED, I2C_DTCY_16_9);
    // configure I2C address
    i2c_mode_addr_config(MMA8451_I2C_BASEADDR, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, MMA8451_I2C_ADDR7);
    // enable I2C1
    i2c_enable(MMA8451_I2C_BASEADDR);
    // enable acknowledge
    i2c_ack_config(MMA8451_I2C_BASEADDR, I2C_ACK_ENABLE);
	//i2c_ack_config(MMA8451_I2C_BASEADDR, I2C_ACK_DISABLE);
    i2c_analog_noise_filter_enable(MMA8451_I2C_BASEADDR);
}

uint8_t hsp_mma8451_read_byte(uint8_t reg, uint8_t *data)
{
    volatile uint32_t timeout = MMA8451_I2C_FLAG_TIMEOUT;

    I2C_STAT0(MMA8451_I2C_BASEADDR) |= (uint16_t)0x0400;

    /* generate the start condition */
    i2c_start_on_bus(MMA8451_I2C_BASEADDR);

    /* test on I2C I2C_SBSEND and clear it */
    timeout = MMA8451_I2C_FLAG_TIMEOUT;
    while(!i2c_flag_get(MMA8451_I2C_BASEADDR, I2C_FLAG_SBSEND)){
        if ((timeout--) == 0) 
            return 0xFF;
    } 
    /* send DCI selected device slave address for read */
    i2c_master_addressing(MMA8451_I2C_BASEADDR, MMA8451_I2C_WRITE_ADDRESS, I2C_TRANSMITTER);
	
    /* test on I2C I2C_ADDSEND and clear it */
    timeout = MMA8451_I2C_FLAG_TIMEOUT; 
    while(!i2c_flag_get(MMA8451_I2C_BASEADDR, I2C_FLAG_ADDSEND)){
        if ((timeout--) == 0) 
            return 0xFF;
    }
    i2c_flag_clear(MMA8451_I2C_BASEADDR, I2C_FLAG_ADDSEND);
    
    timeout = MMA8451_I2C_FLAG_TIMEOUT; 
    while(!i2c_flag_get(MMA8451_I2C_BASEADDR, I2C_FLAG_TBE)){
        if ((timeout--) == 0) 
            return 0xFF;
    }

    /* send register address */
    i2c_data_transmit(MMA8451_I2C_BASEADDR, (uint8_t)(reg));

    /* test on I2C I2C_BTC and clear it */
    timeout = MMA8451_I2C_FLAG_TIMEOUT; 
    while(!i2c_flag_get(MMA8451_I2C_BASEADDR, I2C_FLAG_BTC)){
        if ((timeout--) == 0) 
            return 0xFF;
    } 
	
    /* clear AF flag if arised */
    I2C_STAT0(MMA8451_I2C_BASEADDR) |= (uint16_t)0x0400;
        
    /* prepare stop after receiving data */
    //i2c_stop_on_bus(MMA8451_I2C_BASEADDR);
    
    /* generate the start condition */
    i2c_start_on_bus(MMA8451_I2C_BASEADDR);

    /* test on I2C I2C_SBSEND and clear it */
    timeout = MMA8451_I2C_FLAG_TIMEOUT; 
    while(!i2c_flag_get(MMA8451_I2C_BASEADDR, I2C_FLAG_SBSEND)){
        if ((timeout--) == 0) 
            return 0xFF;
    } 
    
    /* send DCI selcted device slave address for write */
    i2c_master_addressing(MMA8451_I2C_BASEADDR, MMA8451_I2C_READ_ADDRESS, I2C_RECEIVER);
    
    /* test on I2C I2C_ADDSEND and clear it */
    timeout = MMA8451_I2C_FLAG_TIMEOUT;
    while(!i2c_flag_get(MMA8451_I2C_BASEADDR, I2C_FLAG_ADDSEND)){
        if ((timeout--) == 0) 
            return 0xFF;
    }
    i2c_flag_clear(MMA8451_I2C_BASEADDR, I2C_FLAG_ADDSEND); 
    
    /* prepare an nack for the next data received */
    i2c_ack_config(MMA8451_I2C_BASEADDR, I2C_ACK_DISABLE);

    /* test on I2C_RBNE and clear it */
    timeout = HSP_SCCB_FLAG_TIMEOUT;
    while(!i2c_flag_get(MMA8451_I2C_BASEADDR, I2C_FLAG_RBNE)){
        if ((timeout--) == 0) 
            return 0xFF;
    } 
    
    /* receive the data */
    *data = i2c_data_receive(MMA8451_I2C_BASEADDR);

	/* prepare stop after receiving data */
    i2c_stop_on_bus(MMA8451_I2C_BASEADDR); 
	
    /* clear AF flag if arised */
    I2C_STAT0(MMA8451_I2C_BASEADDR) |= (uint16_t)0x0400;
	
    return 0;
}

uint8_t hsp_mma8451_read_nbyte(uint8_t reg, uint8_t *data_addr, uint8_t n)
{
    volatile uint32_t timeout = MMA8451_I2C_FLAG_TIMEOUT;
	uint8_t num;
	
	num = n;

    I2C_STAT0(MMA8451_I2C_BASEADDR) |= (uint16_t)0x0400;

    /* generate the start condition */
    i2c_start_on_bus(MMA8451_I2C_BASEADDR);

    /* test on I2C I2C_SBSEND and clear it */
    timeout = MMA8451_I2C_FLAG_TIMEOUT;
    while(!i2c_flag_get(MMA8451_I2C_BASEADDR, I2C_FLAG_SBSEND)){
        if ((timeout--) == 0) 
            return 0xFF;
    } 
    /* send selected device slave address for read */
    i2c_master_addressing(MMA8451_I2C_BASEADDR, MMA8451_I2C_WRITE_ADDRESS, I2C_TRANSMITTER);
	
    /* test on I2C I2C_ADDSEND and clear it */
    timeout = MMA8451_I2C_FLAG_TIMEOUT; 
    while(!i2c_flag_get(MMA8451_I2C_BASEADDR, I2C_FLAG_ADDSEND)){
        if ((timeout--) == 0) 
            return 0xFF;
    }
    i2c_flag_clear(MMA8451_I2C_BASEADDR, I2C_FLAG_ADDSEND);
    
    timeout = MMA8451_I2C_FLAG_TIMEOUT; 
    while(!i2c_flag_get(MMA8451_I2C_BASEADDR, I2C_FLAG_TBE)){
        if ((timeout--) == 0) 
            return 0xFF;
    }

    /* send register address */
    i2c_data_transmit(MMA8451_I2C_BASEADDR, (uint8_t)(reg));

    /* test on I2C I2C_BTC and clear it */
    timeout = MMA8451_I2C_FLAG_TIMEOUT; 
    while(!i2c_flag_get(MMA8451_I2C_BASEADDR, I2C_FLAG_BTC)){
        if ((timeout--) == 0) 
            return 0xFF;
    } 
	
    /* clear AF flag if arised */
    I2C_STAT0(MMA8451_I2C_BASEADDR) |= (uint16_t)0x0400;
        
    /* prepare stop after receiving data */
    //i2c_stop_on_bus(MMA8451_I2C_BASEADDR);
    
    /* generate the start condition */
    i2c_start_on_bus(MMA8451_I2C_BASEADDR);

    /* test on I2C I2C_SBSEND and clear it */
    timeout = MMA8451_I2C_FLAG_TIMEOUT; 
    while(!i2c_flag_get(MMA8451_I2C_BASEADDR, I2C_FLAG_SBSEND)){
        if ((timeout--) == 0) 
            return 0xFF;
    } 
    
    /* send selcted device slave address for write */
    i2c_master_addressing(MMA8451_I2C_BASEADDR, MMA8451_I2C_READ_ADDRESS, I2C_RECEIVER);
    
    /* test on I2C I2C_ADDSEND and clear it */
    timeout = MMA8451_I2C_FLAG_TIMEOUT;
    while(!i2c_flag_get(MMA8451_I2C_BASEADDR, I2C_FLAG_ADDSEND)){
        if ((timeout--) == 0) 
            return 0xFF;
    }
    i2c_flag_clear(MMA8451_I2C_BASEADDR, I2C_FLAG_ADDSEND); 

	//i2c_ack_config(MMA8451_I2C_BASEADDR, I2C_ACK_ENABLE);
	while (--num)
	{
		/* prepare an ack for the next data received */
		i2c_ack_config(MMA8451_I2C_BASEADDR, I2C_ACK_ENABLE);

		/* test on I2C_RBNE and clear it */
		timeout = HSP_SCCB_FLAG_TIMEOUT;
		while(!i2c_flag_get(MMA8451_I2C_BASEADDR, I2C_FLAG_RBNE)){
			if ((timeout--) == 0) 
				return 0xFF;
		} 

		/* receive the data */
		*data_addr = i2c_data_receive(MMA8451_I2C_BASEADDR);

		/* clear AF flag if arised */
		I2C_STAT0(MMA8451_I2C_BASEADDR) |= (uint16_t)0x0400;
		
		data_addr++;
	}

	/* prepare an nack for the next data received */
    i2c_ack_config(MMA8451_I2C_BASEADDR, I2C_ACK_DISABLE);

	/* test on I2C_RBNE and clear it */
	timeout = HSP_SCCB_FLAG_TIMEOUT;
	while(!i2c_flag_get(MMA8451_I2C_BASEADDR, I2C_FLAG_RBNE)){
		if ((timeout--) == 0) 
			return 0xFF;
	} 

    /* receive the data */
    *data_addr = i2c_data_receive(MMA8451_I2C_BASEADDR);

	/* prepare stop after receiving data */
    i2c_stop_on_bus(MMA8451_I2C_BASEADDR); 
	
    /* clear AF flag if arised */
    I2C_STAT0(MMA8451_I2C_BASEADDR) |= (uint16_t)0x0400;
	
    return 0;
}

uint8_t hsp_mma8451_write_byte(uint8_t reg, uint8_t data)
{
    volatile uint32_t timeout = MMA8451_I2C_FLAG_TIMEOUT;

    I2C_STAT0(MMA8451_I2C_BASEADDR) |= (uint16_t)0x0400;

    /* generate the start condition */
    i2c_start_on_bus(MMA8451_I2C_BASEADDR);

    /* test on I2C I2C_SBSEND and clear it */
    timeout = MMA8451_I2C_FLAG_TIMEOUT;
    while(!i2c_flag_get(MMA8451_I2C_BASEADDR, I2C_FLAG_SBSEND)){
        if ((timeout--) == 0) 
            return 0xFF;
    } 
    /* send selected device slave address for write */
    i2c_master_addressing(MMA8451_I2C_BASEADDR, MMA8451_I2C_WRITE_ADDRESS, I2C_TRANSMITTER);
	
    /* test on I2C I2C_ADDSEND and clear it */
    timeout = MMA8451_I2C_FLAG_TIMEOUT; 
    while(!i2c_flag_get(MMA8451_I2C_BASEADDR, I2C_FLAG_ADDSEND)){
        if ((timeout--) == 0) 
            return 0xFF;
    }
    i2c_flag_clear(MMA8451_I2C_BASEADDR, I2C_FLAG_ADDSEND);
    
    timeout = MMA8451_I2C_FLAG_TIMEOUT; 
    while(!i2c_flag_get(MMA8451_I2C_BASEADDR, I2C_FLAG_TBE)){
        if ((timeout--) == 0) 
            return 0xFF;
    }

    /* send register address */
    i2c_data_transmit(MMA8451_I2C_BASEADDR, (uint8_t)(reg));

    /* test on I2C I2C_BTC and clear it */
    timeout = MMA8451_I2C_FLAG_TIMEOUT; 
    while(!i2c_flag_get(MMA8451_I2C_BASEADDR, I2C_FLAG_BTC)){
        if ((timeout--) == 0) 
            return 0xFF;
    } 
	
    /* clear AF flag if arised */
    I2C_STAT0(MMA8451_I2C_BASEADDR) |= (uint16_t)0x0400;
        
    /* send register address */
    i2c_data_transmit(MMA8451_I2C_BASEADDR, data);
    
    /* test on I2C I2C_BTC and clear it */
    timeout = MMA8451_I2C_FLAG_TIMEOUT; 
    while(!i2c_flag_get(MMA8451_I2C_BASEADDR, I2C_FLAG_BTC)){
        if ((timeout--) == 0) 
            return 0xFF;
    } 
    
	/* prepare stop after receiving data */
    i2c_stop_on_bus(MMA8451_I2C_BASEADDR); 
	
    /* clear AF flag if arised */
    I2C_STAT0(MMA8451_I2C_BASEADDR) |= (uint16_t)0x0400;
	
    return 0;
}

