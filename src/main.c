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

    uint16_t T1 = Udig_Cn(0x89, 0x88);
    int16_t T2 = dig_Cn(0x8B, 0x8A);
    int16_t T3 = dig_Cn(0x8D, 0x8C);

    //main loop
    while(1){      
        uint8_t CHIP_ID = SPI_ReadVal(0xD0); 
        int32_t temperature = temp(T1, T2, T3);  


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
