#ifndef _HSP_GD25Q64_H
#define _HSP_GD25Q64_H

#include "gd32f4xx.h"
#include "gd32f4xx_spi.h"

#define  GD25Q64_PAGE_SIZE       0x100
#define  GD25Q64_CS_LOW()        gpio_bit_reset(GPIOG, GPIO_PIN_9)
#define  GD25Q64_CS_HIGH()       gpio_bit_set(GPIOG, GPIO_PIN_9)

/* initialize SPI5 GPIO and parameter */
void hsp_gd25q64_init(void);
/* erase the specified flash sector */
void gd25q64_spi_sector_erase(uint32_t sector_addr);
/* erase the entire flash */
void gd25q64_spi_bulk_erase(void);
/* write more than one byte to the flash */
void gd25q64_spi_page_write(uint8_t* pbuffer, uint32_t write_addr, uint16_t num_byte_to_write);
/* write block of data to the flash */
void gd25q64_spi_buffer_write(uint8_t* pbuffer, uint32_t write_addr, uint16_t num_byte_to_write);
/* read a block of data from the flash */
void gd25q64_spi_buffer_read(uint8_t* pbuffer, uint32_t read_addr, uint16_t num_byte_to_read);
/* read flash identification */
uint32_t gd25q64_spi_read_id(void);
/* start a read data byte (read) sequence from the flash */
void gd25q64_spi_start_read_sequence(uint32_t read_addr);
/* read a byte from the SPI flash */
uint8_t gd25q64_spi_read_byte(void);
/* send a byte through the SPI interface and return the byte received from the SPI bus */
uint8_t gd25q64_spi_send_byte(uint8_t byte);
/* send a half word through the SPI interface and return the half word received from the SPI bus */
uint16_t gd25q64_spi_send_halfword(uint16_t half_word);
/* enable the write access to the flash */
void gd25q64_spi_write_enable(void);
/* poll the status of the write in progress(wip) flag in the flash's status register */
void gd25q64_wait_for_write_end(void);

/* enable the flash quad mode */
void gd25q64_qspi_enable(void);
/* write block of data to the flash using qspi */
void gd25q64_qspi_buffer_write(uint8_t* pbuffer, uint32_t write_addr, uint16_t num_byte_to_write);
/* read a block of data from the flash using qspi */
void gd25q64_qspi_buffer_read(uint8_t* pbuffer, uint32_t read_addr, uint16_t num_byte_to_read);
/* write more than one byte to the flash using qspi */
void gd25q64_qspi_page_write(uint8_t* pbuffer, uint32_t write_addr, uint16_t num_byte_to_write);

#endif