#pragma once 

#include <stdint.h> 
#include "gpioFunctions.h"

void initSPI();
uint8_t SPI_Exchange(volatile uint32_t *SPI_SPI_SR_Addr, volatile uint8_t *SPI_SPI_DR_Addr, uint8_t val);
