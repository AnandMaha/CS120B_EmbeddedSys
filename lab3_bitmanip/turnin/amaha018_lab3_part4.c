/*	Author: Anand Mahadevan
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #3  Exercise #4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0; DDRB = 0x0F; DDRC = 0xF0; PORTA = 0; PORTB = 0x0F; PORTC = 0xF0;
    /* Insert your solution below */
	
    while (1) {
	PORTB = PINA >> 4;
	PORTC = PINA << 4;
    }
    return 1;
}
