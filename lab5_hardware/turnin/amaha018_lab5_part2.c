/*	Author: Anand Mahadevan
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #5  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
enum States {start, wait, inc, inc_press, dec, dec_press, set_0} state;
void Tick(){
	switch(state){
		case start:
			PORTC = 7; state = wait; break;
		case wait:
			if((~PINA & 0x03) == 0x03)
				state = set_0;
			else if((~PINA & 0x02) == 0x02)
				state = dec;
			else if((~PINA & 0x01) == 0x01)
				state = inc; 
			else
				state = wait;
			break;
		case inc:
			state = inc_press; break;
		case inc_press:
			if((~PINA & 0x03) == 0x03)
				state = set_0;
			else if((~PINA & 0x02) == 0x02)
				state = dec;
			else if((~PINA & 0x01) == 0x01)
				state = inc_press;
			else
				state = wait;
			 break;
		case dec:
			state = dec_press; break;
		case dec_press:
			if((~PINA & 0x03) == 0x03)
				state = set_0;
			else if((~PINA & 0x02) == 0x02)
				state = dec_press;
			else if((~PINA & 0x01) == 0x01)
				state = inc;
			else
				state = wait;
			 break;
		case set_0:
			if((~PINA & 0x03) == 0x03)
				state = set_0; 
			else if((~PINA & 0x02) == 0x02)
				state = dec_press;
			else if((~PINA & 0x01) == 0x01)
				state = inc_press;
			else
				state = wait;
			break;
		default: state = start;
	}
	switch(state){
		case start:
			PORTC = 7; break;
		case wait:
			break;
		case inc:
			if(PINC < 9)
				PORTC = PORTC + 1; 
			break;
		case inc_press:
			 break;
		case dec:
			if(PINC > 0)
				PORTC = PORTC - 1; 
			break;
		case dec_press:
			break;
		case set_0:
			PORTC = 0; break;
		default: PORTC = 7;
	}
}
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0; DDRC = 0xFF; PORTA = 0;  PORTC = 0xFF;
    /* Insert your solution below */
    state = start;
    while (1) {
	Tick();
    }
    return 1;
}
