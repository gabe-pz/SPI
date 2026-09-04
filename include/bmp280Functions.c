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

//t_fine constant used in algos for both temperature and pressure
int32_t t_fine(uint16_t dig_T1, int16_t dig_T2, int16_t dig_T3){ 
    SPI_WriteVal(0xF4, 0x21);//put bmp-280 into sampling x1 for temperature, off for pressure, and forced mode 
    delay(7); 
    //get the ADC for temperature 
    uint8_t temp_MSB_Val = SPI_ReadVal(0xFA);//read value for MSB register for temp     
    uint8_t temp_LSB_Val = SPI_ReadVal(0xFB);//read value for LSB register for temp 
    uint8_t temp_XLSB_Val = SPI_ReadVal(0xFC);//read value for XLSB register for temp 
    int32_t U = (temp_MSB_Val << 12) | (temp_LSB_Val << 4) | (temp_XLSB_Val >> 4); 

    int32_t var_1 = ((double)U / 16384.0 - (double)dig_T1 / 1024.0)*(double)dig_T2; 
    int32_t var_2 = ((double)U / 131072.0 - (double)dig_T1 / 8192.0)*((double)U / 131072.0 - (double)dig_T1 / 8192.0)*(double)dig_T3;
    return (var_1 + var_2); 
}

//function to compute temperature 
int32_t temp(int32_t tFine){
    
    //algo to compute temperature from bosch
    return (tFine*100) / 5120;
} 

int32_t pres(uint16_t dig_P1, int16_t dig_P2, int16_t dig_P3, int16_t dig_P4, int16_t dig_P5, int16_t dig_P6, int16_t dig_P7, int16_t dig_P8, int16_t dig_P9, int32_t tFine){
    SPI_WriteVal(0xF4, 0x05);//put bmp-280 into sampling x1 for temperature, off for pressure, and forced mode
    
    //get the ADC for pressure
    uint8_t pres_MSB_Val = SPI_ReadVal(0xF7); 
    uint8_t pres_LSB_Val = SPI_ReadVal(0xF8); 
    uint8_t pres_XLSB_Val = SPI_ReadVal(0xF9);

    int32_t U = (pres_MSB_Val << 12) | (pres_LSB_Val << 4) | (pres_XLSB_Val >> 4);


    //algo to compute pressure from bosch
    float var_1 = (tFine / 2.0f)*64000.0f;
    float var_2 = var_1 * var_1 * ((float)dig_P6/32768.0f); 

    var_2 = var_2 + var_1 * (float)dig_P5 *2.0f; 
    var_2 = (var_2/4.0f)*(float)dig_P4*65536.0f;  
    
    var_1 = (float)dig_P3 * var_1 * (var_1 / 524288.0f) + (((float)dig_P2 * var_1) / 524288.0f);
    var_1 = (1 + var_1/32768.0f)*(float)dig_P1; 

    float p = 1048576.0f - (float)U;
    p = (p - (var_2 / 4096.0f)) * (6250.0f / var_1);
    
    var_1 = (float)dig_P9 * p * (p/2147483648.0f); 
    var_2 = p * ((float)dig_P8 / 32768.0f); 

    p = p + (var_1 + var_2 + (float)dig_P7) / 16.0f;

    return p; 
}



