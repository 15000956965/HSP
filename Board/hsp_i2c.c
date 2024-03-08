#include "hsp_i2c.h"

// https://github.com/Ybalrid/mma8451_pi/blob/master/src/mma8451_pi.c
// https://github.com/coocox/cox/blob/master/CoX/Driver/Sensor_Accelerometer/MMA8451/Driver/lib/MMA8451.c

void hsp_i2c_init()
{
    // enable peripheral clock
    rcu_periph_clock_enable(RCU_GPIOF);
    rcu_periph_clock_enable(RCU_I2C1);

    // PF0/I2C1_SDA, PF1/I2C1_SCL
    gpio_af_set(GPIOF, GPIO_AF_4, GPIO_PIN_0 | GPIO_PIN_1);
    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_0 | GPIO_PIN_1);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1);

    // configure I2C clock
    i2c_clock_config(HSP_I2C_BASEADDR, HSP_I2C_SPEED, I2C_DTCY_2);
    // configure I2C address
    i2c_mode_addr_config(HSP_I2C_BASEADDR, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, HSP_I2C_CAT9555_ADDR7);
	//i2c_mode_addr_config(HSP_I2C_BASEADDR, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, HSP_I2C_MMA8451_ADDR7);
    // enable I2C1
    i2c_enable(HSP_I2C_BASEADDR);
    // enable acknowledge
    i2c_ack_config(HSP_I2C_BASEADDR, I2C_ACK_ENABLE);
    i2c_analog_noise_filter_enable(HSP_I2C_BASEADDR);
}

void hsp_i2c_write_2byte(uint8_t* p_buffer, uint8_t write_address)
{
    // wait until I2C bus is idle
    while(i2c_flag_get(HSP_I2C_BASEADDR, I2C_FLAG_I2CBSY));

    // send a start condition to I2C bus
    i2c_start_on_bus(HSP_I2C_BASEADDR);
    
    // wait until SBSEND bit is set
    while(!i2c_flag_get(HSP_I2C_BASEADDR, I2C_FLAG_SBSEND));
    
    // send slave address to I2C bus
    i2c_master_addressing(HSP_I2C_BASEADDR, HSP_I2C_CAT9555_ADDR7, I2C_TRANSMITTER);
	//i2c_master_addressing(HSP_I2C_BASEADDR, HSP_I2C_MMA8451_ADDR7, I2C_TRANSMITTER);
    
    // wait until ADDSEND bit is set
    while(!i2c_flag_get(HSP_I2C_BASEADDR, I2C_FLAG_ADDSEND));
    
    // clear the ADDSEND bit
    i2c_flag_clear(HSP_I2C_BASEADDR, I2C_FLAG_ADDSEND);
    
    // wait until the transmit data buffer is empty
    while(SET != i2c_flag_get(HSP_I2C_BASEADDR, I2C_FLAG_TBE));
    
    // send the EEPROM's internal address to write to : only one byte address
    i2c_data_transmit(HSP_I2C_BASEADDR, write_address);
    
    // wait until BTC bit is set
    while(!i2c_flag_get(HSP_I2C_BASEADDR, I2C_FLAG_BTC));
    
    // send the byte to be written: data to port 0
    i2c_data_transmit(HSP_I2C_BASEADDR, *p_buffer); 
    
    // wait until BTC bit is set
    while(!i2c_flag_get(HSP_I2C_BASEADDR, I2C_FLAG_BTC));

    p_buffer++;
    // send the byte to be written: data to port 1
    i2c_data_transmit(HSP_I2C_BASEADDR, *p_buffer); 
    
    // wait until BTC bit is set
    while(!i2c_flag_get(HSP_I2C_BASEADDR, I2C_FLAG_BTC));

    // send a stop condition to I2C bus
    i2c_stop_on_bus(HSP_I2C_BASEADDR);
    
    // wait until the stop condition is finished
    while(I2C_CTL0(HSP_I2C_BASEADDR)&I2C_CTL0_STOP);
}

void hsp_sccb_init(void)
{
    /* enable GPIOB and I2C clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_I2C0);

    /* configure I2C GPIO pins AF: PB8/I2C0_SCL, PB9/I2C0_SDA */
    // gpio_af_set(GPIOB, GPIO_AF_4, GPIO_PIN_8);
    // gpio_af_set(GPIOB, GPIO_AF_4, GPIO_PIN_9);
    gpio_af_set(GPIOB, GPIO_AF_4, GPIO_PIN_8 | GPIO_PIN_9);
    
    /* configure I2C0_SCL(PB8), I2C0_SDA(PB9) */ 
    // gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8);
    // gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_2MHZ, GPIO_PIN_8);
    // gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);
    // gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_2MHZ, GPIO_PIN_9);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8 | GPIO_PIN_9);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_8 | GPIO_PIN_9);
    
    /* configure I2C parameter */
    i2c_deinit(I2C0);
    i2c_clock_config(I2C0, HSP_SCCB_SPEED, I2C_DTCY_2);
    i2c_mode_addr_config(I2C0, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, HSP_SCCB_SLAVE_ADDRESS7);
    i2c_enable(I2C0);

    i2c_ack_config(I2C0, I2C_ACK_ENABLE);
}

uint8_t hsp_sccb_write_byte(uint8_t reg, uint8_t data)
{
    volatile uint32_t timeout = HSP_SCCB_FLAG_TIMEOUT;

    /* the software must wait until i2c bus is idle */
    while(i2c_flag_get(HSP_SCCB_BASEADDR, I2C_FLAG_I2CBSY));  
    /* generate the start condition */
    i2c_start_on_bus(HSP_SCCB_BASEADDR);
    
    /* test on i2c I2C_SBSEND and clear it */
    timeout = HSP_SCCB_FLAG_TIMEOUT;
    while(!i2c_flag_get(HSP_SCCB_BASEADDR, I2C_FLAG_SBSEND)){
        if ((timeout--) == 0)
            return 0xFF;
    }
    
    /* send DCI selected device slave address for write */
    i2c_master_addressing(HSP_SCCB_BASEADDR, HSP_SCCB_WRITE_ADDRESS, I2C_TRANSMITTER);
    
    /* test on I2C I2C_ADDSEND and clear it */
    timeout = HSP_SCCB_FLAG_TIMEOUT; 
    while(!i2c_flag_get(HSP_SCCB_BASEADDR, I2C_FLAG_ADDSEND)){
        if ((timeout--) == 0) 
            return 0xFF;
    }
    i2c_flag_clear(HSP_SCCB_BASEADDR, I2C_FLAG_ADDSEND);
    
    timeout = HSP_SCCB_FLAG_TIMEOUT; 
    while(!i2c_flag_get(HSP_SCCB_BASEADDR, I2C_FLAG_TBE)){
         if ((timeout--) == 0) 
             return 0xFF;
    }
    
    /* send register address */
    i2c_data_transmit(HSP_SCCB_BASEADDR, (uint8_t)(reg));

    /* test on I2C I2C_BTC and clear it */
    timeout = HSP_SCCB_FLAG_TIMEOUT;
    while(!i2c_flag_get(HSP_SCCB_BASEADDR, I2C_FLAG_BTC)){
        if ((timeout--) == 0) 
            return 0xFF;
    }
    /* send data */
    i2c_data_transmit(HSP_SCCB_BASEADDR, data);    

    timeout = HSP_SCCB_FLAG_TIMEOUT; 
    while(!i2c_flag_get(HSP_SCCB_BASEADDR, I2C_FLAG_TBE)){
        if ((timeout--) == 0)
            return 0xFF;
    }  
    /* send I2C stop condition */
    i2c_stop_on_bus(HSP_SCCB_BASEADDR);

    /* if operation is ok, return 0 */
    return 0;
}

uint8_t hsp_sccb_write_2byte(uint8_t reg, uint16_t data)
{
	uint8_t result;
	result = hsp_sccb_write_byte(reg, (uint8_t)(data>>8));	// write upper byte
	result |= hsp_sccb_write_byte(0xF0, (uint8_t)data);		// write lower byte
	
	return result;
}

/*!
    \brief      read a byte from a specific camera register 
    \param[in]  reg:  camera register address
    \param[out] data: read from the specific register
    \retval     0x00 if write operation is ok. 
                0xFF if timeout condition occured (device not connected or bus error).
*/
uint8_t hsp_sccb_read_byte(uint8_t reg, uint8_t *data)
{
    volatile uint32_t timeout = HSP_SCCB_FLAG_TIMEOUT;

    I2C_STAT0(HSP_SCCB_BASEADDR) |= (uint16_t)0x0400;

    /* generate the start condition */
    i2c_start_on_bus(HSP_SCCB_BASEADDR);

    /* test on I2C I2C_SBSEND and clear it */
    timeout = HSP_SCCB_FLAG_TIMEOUT;
    while(!i2c_flag_get(HSP_SCCB_BASEADDR, I2C_FLAG_SBSEND)){
        if ((timeout--) == 0) 
            return 0xFF;
    } 
    /* send DCI selected device slave address for read */
    i2c_master_addressing(HSP_SCCB_BASEADDR, HSP_SCCB_WRITE_ADDRESS, I2C_TRANSMITTER);

    /* test on I2C I2C_ADDSEND and clear it */
    timeout = HSP_SCCB_FLAG_TIMEOUT; 
    while(!i2c_flag_get(HSP_SCCB_BASEADDR, I2C_FLAG_ADDSEND)){
        if ((timeout--) == 0) 
            return 0xFF;
    }
    i2c_flag_clear(HSP_SCCB_BASEADDR, I2C_FLAG_ADDSEND);
    
    timeout = HSP_SCCB_FLAG_TIMEOUT; 
    while(!i2c_flag_get(HSP_SCCB_BASEADDR, I2C_FLAG_TBE)){
        if ((timeout--) == 0) 
            return 0xFF;
    }
    
    /* send register address */
    i2c_data_transmit(HSP_SCCB_BASEADDR, (uint8_t)(reg));
          
    /* test on I2C I2C_BTC and clear it */
    timeout = HSP_SCCB_FLAG_TIMEOUT; 
    while(!i2c_flag_get(HSP_SCCB_BASEADDR, I2C_FLAG_BTC)){
        if ((timeout--) == 0) 
            return 0xFF;
    } 

    /* clear AF flag if arised */
    I2C_STAT0(HSP_SCCB_BASEADDR) |= (uint16_t)0x0400;
        
    /* prepare stop after receiving data */
    i2c_stop_on_bus(HSP_SCCB_BASEADDR);
    
    /* generate the start condition */
    i2c_start_on_bus(HSP_SCCB_BASEADDR);

    /* test on I2C I2C_SBSEND and clear it */
    timeout = HSP_SCCB_FLAG_TIMEOUT; 
    while(!i2c_flag_get(HSP_SCCB_BASEADDR, I2C_FLAG_SBSEND)){
        if ((timeout--) == 0) 
            return 0xFF;
    } 
    
    /* send DCI selcted device slave address for write */
    i2c_master_addressing(HSP_SCCB_BASEADDR, HSP_SCCB_READ_ADDRESS, I2C_RECEIVER);
    
    /* test on I2C I2C_ADDSEND and clear it */
    timeout = HSP_SCCB_FLAG_TIMEOUT;
    while(!i2c_flag_get(HSP_SCCB_BASEADDR, I2C_FLAG_ADDSEND)){
        if ((timeout--) == 0) 
            return 0xFF;
    }
    i2c_flag_clear(HSP_SCCB_BASEADDR, I2C_FLAG_ADDSEND); 
    
    /* prepare an nack for the next data received */
    i2c_ack_config(HSP_SCCB_BASEADDR, I2C_ACK_DISABLE);

    /* test on I2C_RBNE and clear it */
    timeout = HSP_SCCB_FLAG_TIMEOUT;
    while(!i2c_flag_get(HSP_SCCB_BASEADDR, I2C_FLAG_RBNE)){
        if ((timeout--) == 0) 
            return 0xFF;
    } 
    
    /* prepare stop after receiving data */
    i2c_stop_on_bus(HSP_SCCB_BASEADDR); 

    /* receive the data */
    *data = i2c_data_receive(HSP_SCCB_BASEADDR);
	
    /* clear AF flag if arised */
    I2C_STAT0(HSP_SCCB_BASEADDR) |= (uint16_t)0x0400;

    return 0;
}

uint8_t hsp_sccb_read_2byte(uint8_t reg, uint16_t *data)
{
	uint8_t temp1, temp2, result;
	
	result = hsp_sccb_read_byte(reg, &temp1);
	result |= hsp_sccb_read_byte(0xF0, &temp2);
	*data = (temp1<<8) + temp2;
	
	return result;
}