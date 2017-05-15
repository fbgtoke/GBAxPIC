/*
 * File:   main.c
 * Author: fbgtoke, jfons
 *
 * Created on 7 / april / 2017, 12:50
 */

/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__PIC24E__)
    	#include <p24Exxxx.h>
    #elif defined (__PIC24F__)||defined (__PIC24FK__)
	#include <p24Fxxxx.h>
    #elif defined(__PIC24H__)
	#include <p24Hxxxx.h>
    #endif
#endif

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

    // Configure Oscilator
    RCONbits.SWDTEN = 0;
    //OSCCON = 0b01110000;
    OSCCONbits.COSC = 0b111;
    OSCCONbits.NOSC = 0b111;

    // RA0 output
    TRISA = 0xFFFE;
    PORTA = 0x0000;

    while ( 1 ) {
        PORTA ^= 0x1;

        int j;
        for (j = 0; j < 1000000; ++j)
                __asm__("nop");
    }

    return (EXIT_SUCCESS);
}

