#include "gpioFunctions.h"



void gpioXPinMode(char X, int pin,  int pinMode){
    volatile uint32_t *gpioXMODERAddr;
    volatile uint32_t *rccEnAddr = (volatile uint32_t *)0x40023830;

    switch(X){
        case 'A':
            gpioXMODERAddr = (volatile uint32_t *)0x40020000;

            *rccEnAddr = *rccEnAddr | (1 << 0);
            break;
        case 'B':
            gpioXMODERAddr = (volatile uint32_t *)0x40020400;

            *rccEnAddr = *rccEnAddr | (1 << 1);
            break;
        case 'C':
            gpioXMODERAddr = (volatile uint32_t *)0x40020800;

            *rccEnAddr = *rccEnAddr | (1 << 2);
            break;
        case 'D':
            gpioXMODERAddr = (volatile uint32_t *)0x40020C00;

            *rccEnAddr = *rccEnAddr | (1 << 3);
            break;
        case 'E':
            gpioXMODERAddr = (volatile uint32_t *)0x40021000;
                
            *rccEnAddr = *rccEnAddr | (1 << 4);
            break;
        case 'F':
            gpioXMODERAddr = (volatile uint32_t *)0x40021400;

            *rccEnAddr = *rccEnAddr | (1 << 5);
            break;
        case 'G':
            gpioXMODERAddr = (volatile uint32_t *)0x40021800;

            *rccEnAddr = *rccEnAddr | (1 << 5);
            break;
        case 'H':
            gpioXMODERAddr = (volatile uint32_t *)0x40021C00;
            
            *rccEnAddr = *rccEnAddr | (1 << 5);
            break;
        default:
            gpioXMODERAddr = (volatile uint32_t *)0x40020000;

            *rccEnAddr = *rccEnAddr | (1 << 1);
            break;
    }

    switch (pinMode)
    {
    case 00:
        *gpioXMODERAddr = *gpioXMODERAddr & ~(0x3 << (2*pin));
        *gpioXMODERAddr = *gpioXMODERAddr | (0x0 << (2*pin));
        break;
    case 01:
        *gpioXMODERAddr = *gpioXMODERAddr & ~(0x3 << (2*pin));
        *gpioXMODERAddr = *gpioXMODERAddr | (0x1 << (2*pin));
        break;
    case 10:
        *gpioXMODERAddr = *gpioXMODERAddr & ~(0x3 << (2*pin));
        *gpioXMODERAddr = *gpioXMODERAddr | (0x2 << (2*pin));
        break;
    case 11:
        *gpioXMODERAddr = *gpioXMODERAddr & ~(0x3 << (2*pin));
        *gpioXMODERAddr = *gpioXMODERAddr | (0x3 << (2*pin));
        break;
    default:
        *gpioXMODERAddr = *gpioXMODERAddr & ~(0x3 << (2*pin));
        *gpioXMODERAddr = *gpioXMODERAddr | (0x0 << (2*pin));
        break;
    }
}



void digitalGpioXWrite(char X, int pin, int val){
    volatile uint32_t *gpioXBSRRAddr;

    switch(X){
        case 'A':
            gpioXBSRRAddr = (volatile uint32_t *)(0x40020000+0x18);
            break;
        case 'B':
            gpioXBSRRAddr = (volatile uint32_t *)(0x40020400+0x18);
            break;
        case 'C':
            gpioXBSRRAddr = (volatile uint32_t *)(0x40020800+0x18);
            break;
        case 'D':
            gpioXBSRRAddr = (volatile uint32_t *)(0x40020C00+0x18);
            break;
        case 'E':
            gpioXBSRRAddr = (volatile uint32_t *)(0x40021000+0x18);
            break;
        case 'F':
            gpioXBSRRAddr = (volatile uint32_t *)(0x40021400+0x18);
            break;
        case 'G':
            gpioXBSRRAddr = (volatile uint32_t *)(0x40021800+0x18);
            break;
        case 'H':
            gpioXBSRRAddr = (volatile uint32_t *)(0x40021C00+0x18);
            break;
        default:
            gpioXBSRRAddr = (volatile uint32_t *)(0x40020000+0x18);
            break;
    }

    switch (val)
    {
    case 0:
        *gpioXBSRRAddr = (1 << (pin+16));
        break;
    case 1:
        *gpioXBSRRAddr = (1 << pin);
        break;
    
    default:
        *gpioXBSRRAddr = (1 << (pin+16));
        break;
    }


}

