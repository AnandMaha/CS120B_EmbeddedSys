/*	Author: Anand Mahadevan
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #5  Exercise #1
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
	DDRA = 0; DDRC = 0xFF; PORTA = 0;  PORTC = 0xFF;
    /* Insert your solution below */
	
    while (1) {
	unsigned char tmpA = ~PINA & 0x0F; unsigned char tmpC = 0; 
	if(tmpA >= 1)
		tmpC = tmpC | 0x20; 
	if(tmpA >= 3)
		tmpC = tmpC | 0x10; 
	if(tmpA >= 5)
		tmpC = tmpC | 0x08; 
	if(tmpA >= 7)
		tmpC = tmpC | 0x04; 
	if(tmpA >= 10)
		tmpC = tmpC | 0x02; 
	if(tmpA >= 13)
		tmpC = tmpC | 0x01; 
	// lights PC6 fuel warning
	if(tmpA <= 4)
		tmpC = tmpC | 0x40;
	PORTC = tmpC;
    }
    return 1;
}
