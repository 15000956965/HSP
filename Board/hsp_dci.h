#ifndef _HSP_DCI_H
#define _HSP_DCI_H

#include "gd32f4xx.h"
#include "gd32f4xx_dci.h"

#define DCI_DR_ADDRESS     (0x50050028)
#define DCI_TIMEOUT        20000

void hsp_dci_init(void);
//void hsp_dci_dma_config(uint8_t *buff);
void hsp_dci_dma_config(void);
void DMA1_Channel7_IRQHandler(void);

#endif