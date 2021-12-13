/*	Author: Anand Mahadevan
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #3  Exercise #1
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
	DDRA = 0; DDRB = 0; DDRC = 0xFF; PORTA = 0; PORTB = 0; PORTC = 0xFF;
    /* Insert your solution below */
	
    while (1) {
	unsigned char cnt = 0;unsigned char tmpA = PINA; unsigned char tmpB = PINB;

	for(int i = 0; i <= 7; i++){
		if(((tmpA >> i) & 0x01) == 0x01) // means last bit = 1
			cnt++;
		if(((tmpB >> i) & 0x01) == 0x01) // means last bit = 1
			cnt++;
	}	

	PORTC = cnt;
    }
    return 1;
}
