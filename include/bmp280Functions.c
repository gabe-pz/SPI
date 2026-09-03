#include "bmp280Functions.h" 

//function to return the unsigned 16-bit calibration constants(e.g dig_T1 and dig_P1)
uint16_t Udig_Cn(uint8_t MSB_RWA, uint8_t LSB_RWA){
    uint16_t MSB_VAL = SPI_ReadVal(MSB_RWA);
    uint16_t LSB_VAL = SPI_ReadVal(LSB_RWA);

    return ((MSB_VAL << 8) | LSB_VAL);
}

//function to return the signed 16-bit calibration constants(e.g dig_Tn and dig_Pn, n != 1)
int16_t dig_Cn(uint8_t MSB_RWA, uint8_t LSB_RWA){
    int16_t MSB_VAL = SPI_ReadVal(MSB_RWA);
    int16_t LSB_VAL = SPI_ReadVal(LSB_RWA);

    return ((MSB_VAL << 8) | LSB_VAL);
}

//algo to read the temp via data sheet by bosch
int32_t temp(uint16_t dig_T1, int16_t dig_T2, int16_t dig_T3){ 
    SPI_WriteVal(0xF4, 0x21);//put bmp-280 into forced mode
    delay(10);

    uint8_t temp_XLSB_Val = SPI_ReadVal(0xFC);//read value for XLSB register for temp 
    uint8_t temp_LSB_Val = SPI_ReadVal(0xFB);//read value for LSB register for temp 
    uint8_t temp_MSB_Val = SPI_ReadVal(0xFA);//read value for MSB register for temp 
    
    int32_t U = (temp_MSB_Val << 12) | (temp_LSB_Val << 4) | (temp_XLSB_Val >> 4); 

    int32_t x = U / 8 - (int32_t)dig_T1 * 2;//scaling such that can still get ints          
    int32_t var_1 = x * (int32_t)dig_T2 / 2048;
    int32_t var_2 = ((x/2) * ((x/2) / 4096)) * (int32_t)dig_T3 / 16384;

    return ((var_1 + var_2) * 5 + 128) >> 8;
}
