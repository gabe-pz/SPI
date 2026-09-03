#include "spiFunctions.h"
#include "gpioFunctions.h"
#include "generalHelpers.h"
#include "uartFunctions.h"
#include "bmp280Functions.h" 

int main(void){
    //init peripherals 
    initSPI();  
    initUART();
    
    delay(500);
    
    //calibration constants for temperature 
    uint16_t T1 = Udig_Cn(0x89, 0x88);
    int16_t T2 = dig_Cn(0x8B, 0x8A);
    int16_t T3 = dig_Cn(0x8D, 0x8C); 
    
    //calibration constants for pressure
    uint16_t P1 = Udig_Cn(0x8F, 0x8E);
    int16_t P2 = dig_Cn(0x91, 0x90);
    int16_t P3 = dig_Cn(0x93, 0x92); 
    int16_t P4 = dig_Cn(0x95, 0x94);
    int16_t P5 = dig_Cn(0x97, 0x96); 
    int16_t P6 = dig_Cn(0x99, 0x98);
    int16_t P7 = dig_Cn(0x9B, 0x9A); 
    int16_t P8 = dig_Cn(0x9D, 0x9C);
    int16_t P9 = dig_Cn(0x9F, 0x9E); 

    //main loop
    while(1){      
        uint8_t CHIP_ID = SPI_ReadVal(0xD0); 
        int32_t tFine = t_fine(T1, T2, T3);
        int32_t temperature = temp(tFine);  


        if(CHIP_ID == 0x58){
            digitalGpioXWrite('A', 6, 1);
        }

	    //****PRINTING****
	    uart_printMsg("Value = ");
	    uart_printInt(temperature);//display value
	    uart_printMsg("\r\n");//new line to display
        delay(500); 

    }

    return 0; 
}
