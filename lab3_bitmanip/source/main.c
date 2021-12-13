/*	Author: Anand Mahadevan
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #3  Exercise #5
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
	DDRD = 0; DDRB = 0xFE; PORTD = 0;  PORTB = 0x00;
    /* Insert your solution below */
    while (1) {
	unsigned short weight = PIND;
	unsigned char tmpB = PINB & 0x01;
	weight = (weight << 1) + tmpB;
	if(weight >= 70)
		PORTB = tmpB | 0x02;
	else if(weight > 5 && weight < 70)
		PORTB = tmpB | 0x04;
	else
		PORTB = tmpB;
	PORTB = PORTB & 0xFE;
    }
    return 1;
}
