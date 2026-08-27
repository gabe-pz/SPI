#include "spiFunctions.h"
#include "gpioFunctions.h"
#include "generalHelpers.h"


int main(void){
    initSPI();  

    delay(200);


    //Address for the SPI2 hardware status register
    volatile uint32_t *SPI2_SPI_SR_Addr = (volatile uint32_t *)(0x40003800+0x08);

    //Address for the SPI2 hardware data register
    volatile uint8_t *SPI2_SPI_DR_Addr = (volatile uint8_t *)(0x40003800+0x0C);//8-bits at the 32 bit address thus use uint8
    

    while(1){
        digitalGpioXWrite('A', 8, 0);//drive CS pin low such that select slave


        //*****SPI EXCHANGES*****
        //dummy value from first exchange
        uint8_t val = SPI_Exchange(SPI2_SPI_SR_Addr, SPI2_SPI_DR_Addr, 0xD0);//read value in data register
        //actual value want 
        val = SPI_Exchange(SPI2_SPI_SR_Addr, SPI2_SPI_DR_Addr, 0xFF);


        digitalGpioXWrite('A', 8, 1);//drive CS to high so slave knows done with it
        
        //indicator recieved correct data(CHIP ID) 
        if (val == 0x58){
            digitalGpioXWrite('A', 6, 1);
        }
        else{
             digitalGpioXWrite('A', 6, 0);
        }

    }

    return 0;
}