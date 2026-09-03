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

    uint8_t temp_XLSB_Val = SPI_ReadVal(0xFC);//read value for XLSB register for temp 
    uint8_t temp_LSB_Val = SPI_ReadVal(0xFB);//read value for LSB register for temp 
    uint8_t temp_MSB_Val = SPI_ReadVal(0xFA);//read value for MSB register for temp 
    
    int32_t U = (temp_MSB_Val << 12) | (temp_LSB_Val << 4) | (temp_XLSB_Val >> 4); 

    double var_1 = ((double)U / 16384.0 - (double)dig_T1 / 1024.0)*(double)dig_T2; 
    double var_2 = ((double)U / 131072.0 - (double)dig_T1 / 8192.0)*((double)U / 131072.0 - (double)dig_T1 / 8192.0)*(double)dig_T3;
 
    return (((var_1 + var_2) / 5120.0) * 100);
} 

int32_t pres(uint16_t dig_P1, int16_t dig_P2, int16_t dig_P3, int16_t dig_P4, int16_t dig_P5, int16_t dig_P6, int16_t dig_P7, int16_t dig_P8, int16_t dig_P9, uint16_t dig_T1, int16_t dig_T2, int16_t dig_T3){
    SPI_WriteVal(0xF4, 0x05); 

    uint8_t pres_XLSB_Val = SPI_ReadVal(0xF9);
    uint8_t pres_LSB_Val = SPI_ReadVal(0xF8);
    uint8_t pres_MSB_Val = SPI_ReadVal(0xF7);

    int32_t U = (pres_MSB_Val << 12) | (pres_LSB_Val << 4) | (pres_XLSB_Val >> 4);

    double var_1T = ((double)U / 16384.0 - (double)dig_T1 / 1024.0)*(double)dig_T2; 
    double var_2T = ((double)U / 131072.0 - (double)dig_T1 / 8192.0)*((double)U / 131072.0 - (double)dig_T1 / 8192.0)*(double)dig_T3;


    double var_1 = (var_1T+var_2T / 2.0)*64000.0;
    double var_2 = var_1*var_1 * ((double)dig_P6/32768.0); 

    var_2 = var_2 + var_1 * (double)dig_P5 *2.0; 
    var_2 = (var_2/4.0)*(double)dig_P4*65536.0;  
    
    var_1 = (double)dig_P3 * var_1 * (var_1 / 524288.0) + (((double)dig_P2 * var_1) / 524288.0);
    var_1 = (1 + var_1/32768.0)*(double)dig_P1; 

    double p = 1048576.0 - (double)U;
    p = (p - (var_2 / 4096.0)) * (6250.0 / var_1);
    
    var_1 = (double)dig_P9 * p * (p/2147483648.0); 
    var_2 = p * ((double)dig_P8 / 32768.0); 

    p = p + (var_1 + var_2 + (double)dig_P7) / 16.0;

    return p; 
}



