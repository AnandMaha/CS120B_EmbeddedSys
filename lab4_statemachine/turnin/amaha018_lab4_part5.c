/*	Author: Anand Mahadevan
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #4  Exercise #5
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
enum States {start, type_code, correct, unlock, lock} state;
unsigned char sym[4] = {4, 1, 2, 1}; // #-X-Y-X
unsigned char idx = 0;
const int SIZE = 4;
void Tick(){
	switch(state){
		case start:
			PORTB = 0; state = type_code; break;
		case type_code:
			if(PINA == sym[idx])
				state = correct;
			else if(PINA == 0x80)
				state = lock;
			else if(PINA == 0)
				state = type_code;
			else { //wrong symbol entered
				state = type_code; 
				idx = 0;
			}
			break;
		case correct:
			if(idx == SIZE)
				state = unlock;
			else
				state = type_code;
			break;
		case unlock:
			if(PINA == 0x80)
				state = lock;
			else
				state = type_code;
			break;
		case lock:
			state = type_code;
			 break;
		default: state = start;
	}
	switch(state){
		case start:
			PORTB = 0; PORTC = start; break;
		case type_code:
			PORTC = type_code;break;
		case correct:
			idx++;
			PORTC = correct;break;
		case unlock:
			idx = 0;
			if(PINB == 1)
				PORTB = 0;
			else
				PORTB = 1;
			PORTC = unlock;break;
		case lock:
			PORTB = 0;
			 PORTC = lock;break;
		default: PORTB = 0;
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
