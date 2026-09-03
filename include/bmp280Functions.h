#pragma once 

#include <stdint.h>
#include "spiFunctions.h" 
#include "generalHelpers.h"



//constants
uint16_t Udig_Cn(uint8_t MSB_RWA, uint8_t LSB_RWA);
int16_t dig_Cn(uint8_t MSB_RWA, uint8_t LSB_RWA);

//temp and pres
int32_t temp(uint16_t dig_T1, int16_t dig_T2, int16_t dig_T3);
