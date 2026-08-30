#include "spiFunctions.h"
#include "gpioFunctions.h"
#include "generalHelpers.h"
#include "uartFunctions.h"


int main(void){
    //init peripherals 
    initSPI();  
    initUART();
    
    //small delay
    delay(200);

    //main loop
    while(1){
        digitalGpioXWrite('A', 8, 0);//drive CS pin low such that select slave


        //*****SPI EXCHANGES*****
        uint8_t val = SPI_Exchange(0xD0);//dummy value from the write to request CHIP_ID
        
        val = SPI_Exchange(0xFF);//CHIP_ID value from the dummy write to be able to get the value

        digitalGpioXWrite('A', 8, 1);//drive CS to high 
        
        //indicator that determines if recieved the correct value for CHIP_ID
        if (val == 88){
            digitalGpioXWrite('A', 6, 1);
        }
        else{
             digitalGpioXWrite('A', 6, 0);
        }

        //****PRINTING****
        uart_printInt(val);//display val
        uart_sendMsg("\r\n");//new line
        delay(1000);


    }

    return 0; 
}
