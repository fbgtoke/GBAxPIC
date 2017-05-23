/*
 * File:   main.c
 * Author: student
 *
 * Created on 27 / març / 2017, 15:49
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
#include <time.h>
#include <stdlib.h>

//#define _XTAL_FREQ 8000000

unsigned long long _sleep_c = 0;
#define sleep(x) for (_sleep_c = 0; _sleep_c < x; ++_sleep_c) __asm__("nop");

/*
 *
 */
int main(int argc, char** argv) {

    int i;

    // Configure Oscilator
    RCONbits.SWDTEN = 0;
    OSCCONbits.COSC = 0b111;
    OSCCONbits.NOSC = 0b111;

    // AD disabled
    AD1PCFG = 0xFFFF;

    // RA0 output
    TRISB = 0x0000;

    // SI = 0
    PORTB = 0x0000;

    for (i = 0; i < 20; ++i) sleep(25000);

    // SI = 1
    PORTB = 0x4080;
    
    sleep(25000);

    unsigned char data = 69;

    for (i = 0; i < 8; ++i) {
        unsigned char value = data >> (7-i);
        // SD = value (1 or 0)
        PORTB = 0x4000 | ((value & 0x0001) << 13);

        sleep(25000);

        // SC = 1
        PORTB |= 0x8000;

        // delay
        sleep(25000);

        // SC = 0
        PORTB &= ~0x8000;
        sleep(25000);
    }

    // SI = 0
    PORTB = 0x0000;

    while ( 1 );

    return (EXIT_SUCCESS);
}   
