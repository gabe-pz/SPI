#include "spiFunctions.h"

//configured for SPI2 hardware with the stm32f446
void initSPI(){  
  gpioXPinMode('A', 8, 01);//PA8 to general output and enable GPIOA RCC
  gpioXPinMode('A', 6, 01);//PA6 to general output and enable GPIOA RCC
  gpioXPinMode('B', 10, 10);//PB10 to alternate function and enable GPIOB RCC
  gpioXPinMode('C', 2, 10);//PC2 to alternate function and enable GPIOC RCC
  gpioXPinMode('C', 3, 10);//PC3 to alternate function and enable GPIOC RCC
  

  //Address for the RCC APB1 register
  volatile uint32_t *RCC_APB1_Addr = (volatile uint32_t *)(0x40023800+0x40); 
  //Enable the RCC for SPI2 hardware
  *RCC_APB1_Addr = *RCC_APB1_Addr | (1 << 14);//enable SPI2 RCC

  //Addresses for GPIO B & C, AF registers
  volatile uint32_t *GPIOB_AFHR_Addr = (volatile uint32_t *)(0x40020400+0x24);
  volatile uint32_t *GPIOC_AFLR_Addr = (volatile uint32_t *)(0x40020800+0x20); 

  //Mapping SCK, MOSI, and MISO, to their corresponding alternate function(AF5 here)
    //SCK 
    *GPIOB_AFHR_Addr = *GPIOB_AFHR_Addr & ~(0xF << 8);//clear bit field
    *GPIOB_AFHR_Addr = *GPIOB_AFHR_Addr | (0x5 << 8);//write value (0101) for pin PB10 

    //MOSI
    *GPIOC_AFLR_Addr = *GPIOC_AFLR_Addr & ~(0xF << 12);//clear bit field
    *GPIOC_AFLR_Addr = *GPIOC_AFLR_Addr | (0x5 << 12);//write value (0101) for pin PC3

    //MISO
    *GPIOC_AFLR_Addr = *GPIOC_AFLR_Addr & ~(0xF << 8);//clear bit field
    *GPIOC_AFLR_Addr = *GPIOC_AFLR_Addr | (0x5 << 8);//write value (0101) for pin PC2

  //Address for the GPIOB & GPIOC OSPEEDR register
  volatile uint32_t *GPIOB_OSPEEDR_Addr = (volatile uint32_t *)(0x40020400+0x08); 
  volatile uint32_t *GPIOC_OSPEEDR_Addr = (volatile uint32_t *)(0x40020800+0x08);

  //setoutput speed to high for SCK, MOSI, and MISO pins
    *GPIOB_OSPEEDR_Addr = *GPIOB_OSPEEDR_Addr & ~(0x3 << 20);//clear bit field
    *GPIOB_OSPEEDR_Addr = *GPIOB_OSPEEDR_Addr | (0x3 << 20);//write value (11) for pin PB10

    *GPIOC_OSPEEDR_Addr = *GPIOC_OSPEEDR_Addr & ~(0x3 << 6);//clear bit field
    *GPIOC_OSPEEDR_Addr = *GPIOC_OSPEEDR_Addr | (0x3 << 6);//write value (11) for pin PC3

    *GPIOC_OSPEEDR_Addr = *GPIOC_OSPEEDR_Addr & ~(0x3 << 4);//clear bit field
    *GPIOC_OSPEEDR_Addr = *GPIOC_OSPEEDR_Addr | (0x3 << 4);//write value (11) for pin PC2



  digitalGpioXWrite('A', 8, 1);//write 1 to CS Pin(PA8) such that chip not selected yet

  //Address for SPI2 SPI_CR1(Control) register 
  volatile uint32_t *SPI2_SPI_CR1_Addr = (volatile uint32_t *)(0x40003800);

  //SPI2 Hardware config
    *SPI2_SPI_CR1_Addr = *SPI2_SPI_CR1_Addr & ~(1 << 0);//write value (0) to bit field for CPHA
    *SPI2_SPI_CR1_Addr = *SPI2_SPI_CR1_Addr & ~(1 << 1);//write value (0) to bit field for CPOL

    *SPI2_SPI_CR1_Addr = *SPI2_SPI_CR1_Addr & ~(0x7 << 3);//write value (000) to bit field for Baud rate control(BR)

    *SPI2_SPI_CR1_Addr = *SPI2_SPI_CR1_Addr | (1 << 9);//write value (1) to bit field for Software Slave Managment(SSM)

    *SPI2_SPI_CR1_Addr = *SPI2_SPI_CR1_Addr | (1 << 8);//write value (1) to bit field for Internal Slave Select(SSI)

    *SPI2_SPI_CR1_Addr = *SPI2_SPI_CR1_Addr | (1 << 2);//write value (1) to bit field for MSTR to enable MCU as master

    *SPI2_SPI_CR1_Addr = *SPI2_SPI_CR1_Addr & ~(1 << 11);//write value (0) to bit field for Data frame format(DFF)

    *SPI2_SPI_CR1_Addr = *SPI2_SPI_CR1_Addr | (1 << 6);//write one to bit 6 to enable SPI
}


uint8_t SPI_Exchange(uint8_t val){
    //Address for the SPI2 hardware status register
    volatile uint32_t *SPI2_SPI_SR_Addr = (volatile uint32_t *)(0x40003800+0x08);
    //Address for the SPI2 hardware data register
    volatile uint8_t *SPI2_SPI_DR_Addr = (volatile uint8_t *)(0x40003800+0x0C);

    while(!(*SPI2_SPI_SR_Addr & (1 << 1)));//wait for transmit buffer to be empty. Break out of loop whenever bit for TXE is 1, else keep running in loop
    
    *SPI2_SPI_DR_Addr = val; //write value to the data register
    
    while(!(*SPI2_SPI_SR_Addr & (1 << 0)));//wait for Recieve buffer to be non-empty

    return *SPI2_SPI_DR_Addr;//return value in the data register(DR)
}