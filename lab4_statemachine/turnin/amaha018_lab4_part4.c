/*	Author: Anand Mahadevan
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #4  Exercise #4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
enum States {start, empty, hash_press, hash_release, unlock, lock} state;

void Tick(){
	switch(state){
		case start:
			PORTB = 0; state = empty; break;
		case empty:
			if(PINA == 0x04)
				state = hash_press;
			else if(PINA == 0x80)
				state = lock;
			else
				state = empty;
			break;
		case hash_press:
			if(PINA == 0x00)
				state = hash_release;
			else if(PINA == 0x80)
				state = lock;
			else if(PINA == 0x02)
				state = unlock;
			else if(PINA == 0x04)
				state = hash_press;
			else if(PINA == 0x01)
				state = empty;
			break;
		case hash_release:
			if(PINA == 0x00)
				state = hash_release;
			else if(PINA == 0x80)
				state = lock;
			else if(PINA == 0x02)
				state = unlock;
			else
				state = empty;
			break;
		case unlock:
			if(PINA == 0x80)
				state = lock;
			else
				state = empty;
			break;
		case lock:
			state = empty;
			 break;
		default: state = start;
	}
	switch(state){
		case start:
			PORTB = 0; PORTC = start; break;
		case empty:
			PORTC = empty;break;
		case hash_press:
			PORTC = hash_press;break;
		case hash_release:
			PORTC = hash_release;break;
		case unlock:
			if(PINB == 0)
				PORTB = 1;
			else
				PORTB = 0;			
			PORTC = unlock;
			break;
		case lock:
			PORTB = 0;PORTC = lock;
			 break;
		default: PORTB = 0;PORTC = start;
	}
}
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0; DDRB = 0xFF; DDRC = 0xFF; PORTA = 0; PORTB = 0xFF; PORTC = 0xFF;
    /* Insert your solution below */
    state = start;
    while (1) {
	Tick();
    }
    return 1;
}
