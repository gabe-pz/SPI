#pragma once 

#include <stdint.h> 
#include <gpioFunctions.h>

void initUART();
void uart_sendMsg(char *msg);
void uart_printInt(int val); 