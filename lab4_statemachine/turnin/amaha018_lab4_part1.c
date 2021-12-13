/*	Author: Anand Mahadevan
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #4  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
enum States {start, pb0_on_press, pb0_on_release, pb1_on_press, pb1_on_release} state;
void Tick(){
	switch(state){
		case start:
			state = pb0_on_release; break;
		case pb0_on_press:
			if((PINA & 0x01) == 0x01)
				state = pb0_on_press; 
			else
				state = pb0_on_release;
			break;
		case pb0_on_release:
			if((PINA & 0x01) == 0x01)
				state = pb1_on_press;
			else
				state = pb0_on_release;
			 break;
		case pb1_on_press:
			if((PINA & 0x01) == 0x01)
				state = pb1_on_press; 
			else
				state = pb1_on_release;
			break;
		case pb1_on_release:
			if((PINA & 0x01) == 0x01)
				state = pb0_on_press; 
			else
				state = pb1_on_release;
			break;
		default: state = start;
	}
	switch(state){
		case start:
			PORTB = 1; break;
		case pb0_on_press:
			PORTB = 1; break;
		case pb0_on_release:
			PORTB = 1; break;
		case pb1_on_press:
			PORTB = 2; break;
		case pb1_on_release:
			PORTB = 2; break;
		default: PORTB = 1;;
	}
}
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0; DDRB = 0xFF; PORTA = 0;  PORTB = 0xFF;
    /* Insert your solution below */
    state = start;
    while (1) {
	Tick();
    }
    return 1;
}
