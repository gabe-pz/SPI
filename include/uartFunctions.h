#pragma once 

#include <stdint.h> 
#include "gpioFunctions.h"

void initUART();

void uart_printMsg(char *msg);
void uart_printInt(int val); 
