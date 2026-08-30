#include "spiFunctions.h"
#include "gpioFunctions.h"
#include "generalHelpers.h"
#include "uartFunctions.h"


int main(void){
    //init peripherals 
    initSPI();  
    initUART();
    
    delay(200);

    //main loop
    while(1){
        //*****SPI EXCHANGES*****
        uint8_t val = SPI_ReadVal(0xD0);//value for chip_id
        
        //indicator that determines if recieved the correct value for CHIP_ID
        if (val == 88){
            digitalGpioXWrite('A', 6, 1);
	          delay(1000);
	          digitalGpioXWrite('A', 6, 0);
	          delay(1000);
        }

        else{
             digitalGpioXWrite('A', 6, 0);
        }

        //****PRINTING****
        uart_printInt(val);//display value
        uart_sendMsg("\r\n");//new line
        


    }

    return 0; 
}
