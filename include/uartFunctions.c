#include "uartFunctions.h" 


void initUART(){
    //RCC for GPIOA enabled in SPIinit 

    //address for rcc apb1
    volatile uint32_t *RCC_APB1_Addr = (volatile uint32_t *)(0x40023800+0x40);

    //Enable the RCC for USART2 hardware
    *RCC_APB1_Addr = *RCC_APB1_Addr | (1 << 17);//enable USART2 RCC 


    gpioXPinMode('A', 2, 10);//set pin mode for PA2 to AF
    gpioXPinMode('A', 3, 10);//set pin mode for PA3 to AF


    volatile uint32_t *GPIOA_AFRL_Addr = (volatile uint32_t *)(0x40020000+0x20);

    //Map pins to AF7
        *GPIOA_AFRL_Addr = *GPIOA_AFRL_Addr & ~(0xF << 8);//clear bit field
        *GPIOA_AFRL_Addr = *GPIOA_AFRL_Addr | (0x7 << 8);//write value (0111) for pin PA2 

        *GPIOA_AFRL_Addr = *GPIOA_AFRL_Addr & ~(0xF << 12);//clear bit field
        *GPIOA_AFRL_Addr = *GPIOA_AFRL_Addr | (0x7 << 12);//write value (0111) for pin PA3

    
    //address for the USART2_CR1 register
    volatile uint32_t *USART2_CR1_Addr = (volatile uint32_t *)(0x40004400+0x0C);
    volatile uint32_t *USART2_CR2_Addr = (volatile uint32_t *)(0x40004400+0x10);
    volatile uint32_t *USART2_BRR_Addr = (volatile uint32_t *)(0x40004400+0x08);


    *USART2_CR1_Addr = *USART2_CR1_Addr & ~(1 << 12);//set the word length to 1 start bit, 8 bits in data frame, and nstop bits


    *USART2_CR1_Addr = *USART2_CR1_Addr & ~(1 << 10);//set the word length to 1 start bit, 8 bits in data frame, and nstop bits


    *USART2_CR2_Addr = *USART2_CR2_Addr & ~(0x3 << 12);//set the stop bits to be one

    *USART2_CR1_Addr = *USART2_CR1_Addr & ~(1 << 15);//set the oversampling to 16-bits
    
    *USART2_BRR_Addr = *USART2_BRR_Addr & ~(0x0F << 0);//clear
    *USART2_BRR_Addr = *USART2_BRR_Addr | (0x0B << 0);//set the fraction part for the uart divder

    *USART2_BRR_Addr = *USART2_BRR_Addr & ~(0xFFF << 4);//clear
    *USART2_BRR_Addr = *USART2_BRR_Addr | (0x08 << 4);//set the mantissa part for the uart divder


    *USART2_CR1_Addr = *USART2_CR1_Addr | (1 << 3); //enable the transmitter 


    *USART2_CR1_Addr = *USART2_CR1_Addr | (1 << 13);//enable USART2

}

void uart_sendMsg(char *msg){
    char *p = msg;
    volatile uint8_t *USART2_DR_Addr = (volatile uint8_t *)(0x40004400+0x04);
    volatile uint32_t *USART2_SR_Addr = (volatile uint32_t *)(0x40004400+0x00); 


    while(*p != '\0'){
        
        while(!(*USART2_SR_Addr & (1 << 7))); 
        *USART2_DR_Addr = (uint8_t)*p; 

        p++;
        
    }
}

void uart_printInt(int val){
    //determine size
    int size = 1;
    for(int i = 1; 1 < val/(10*i); i*=10) size++; 
    int digits[size]; 
    
    //extract digits
    int i = 0; 
    while(val != 0){
        digits[i] = val%10;
        val = val/10; 
        i++;
    }

    //create the string
    char charDigits[size];
    for(int j = 0; j < size; j++){
        charDigits[j] = '0' + digits[(size-1)-j];
    }
    charDigits[size] = '\0';

    //print it
    uart_sendMsg(charDigits);
}