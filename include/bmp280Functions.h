#pragma once 

#include <stdint.h>
#include "spiFunctions.h" 
#include "generalHelpers.h"



//constants
uint16_t Udig_Cn(uint8_t MSB_RWA, uint8_t LSB_RWA);
int16_t dig_Cn(uint8_t MSB_RWA, uint8_t LSB_RWA);

//temp and pres
int32_t temp(uint16_t dig_T1, int16_t dig_T2, int16_t dig_T3);
int32_t pres(uint16_t dig_P1, int16_t dig_P2, int16_t dig_P3, int16_t dig_P4, int16_t dig_P5, int16_t dig_P6, int16_t dig_P7, int16_t dig_P8, int16_t dig_P9, uint16_t dig_T1, int16_t dig_T2, int16_t dig_T3);
