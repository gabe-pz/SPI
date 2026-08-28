#include "spiFunctions.h"
#include "gpioFunctions.h"
#include "generalHelpers.h"


int main(void){
    initSPI();  

    delay(200);

    //Address for the SPI2 hardware status register
    volatile uint32_t *SPI2_SPI_SR_Addr = (volatile uint32_t *)(0x40003800+0x08);

    //Address for the SPI2 hardware data register
    volatile uint8_t *SPI2_SPI_DR_Addr = (volatile uint8_t *)(0x40003800+0x0C);
    

    while(1){
        digitalGpioXWrite('A', 8, 0);//drive CS pin low such that select slave


        //*****SPI EXCHANGES*****
        uint8_t val = SPI_Exchange(SPI2_SPI_SR_Addr, SPI2_SPI_DR_Addr, 0xD0);//dummy value from the write to request CHIP_ID
        
        val = SPI_Exchange(SPI2_SPI_SR_Addr, SPI2_SPI_DR_Addr, 0xFF);//CHIP_ID value from the dummy write to be able to get the value


        digitalGpioXWrite('A', 8, 1);//drive CS to high 
        
        //indicator that determines if recieved the correct value for CHIP_ID
        if (val == 0x58){
            digitalGpioXWrite('A', 6, 1);
        }
        else{
             digitalGpioXWrite('A', 6, 0);
        }

    }

    return 0;
}