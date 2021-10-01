/*	Author: Anand Mahadevan
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #2  Exercise #4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <stdlib.h> // for abs
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
int main(void) {
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0x00; PORTB = 0xFF; // Configure port B's 8 pins as inputs
	DDRC = 0x00; PORTC = 0xFF; // Configure port C's 8 pins as inputs
	DDRD = 	0xFF; PORTD = 0x00; // Configure port D's 8 pins as outputs
	while(1) {
		// first set PORTD to A+B+C
		PORTD = PINA + PINB + PINC;

		// check if A+B+C > 140, if yes then set PD0 to 1, 0 otherwise
		if(PINA + PINB + PINC > 140){
			PORTD = PIND | 0x01;
		} else{
			PORTD = PIND & 0xFE; 
		}

		//check if abs(A - C) > 80, if yes then set PD1 to 1, 0 otherwise
		if(abs(PINA - PINC) > 80){
			PORTD = PIND | 0x02;
		} else{
			PORTD = PIND & 0xFD; 
		}

	}
	return 1;
}

