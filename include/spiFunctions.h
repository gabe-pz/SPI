#pragma once 

#include <stdint.h> 
#include "gpioFunctions.h"

void initSPI();
uint8_t SPI_ReadVal(uint8_t val);
