/*	Author: Anand Mahadevan
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #5  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
* 	Demo Link: https://www.youtube.com/watch?v=Q-yheojdM10
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
enum States {start, wait, btn_press, btn_hold} state;
unsigned char lights[8] = {0x00, 0x21, 0x12, 0x0C, 0x3F, 0x00, 0x3F};
const unsigned char SIZE = 8;
unsigned char idx;
void Tick(){
	switch(state){
		case start:
			PORTC = 0; state = wait; break;
		case wait:
			if(~PINA & 0x01)
				state = btn_press;
			else
				state = wait;
			break;
		case btn_press:
			state = btn_hold;
			break;
		case btn_hold:
			if(~PINA & 0x01)
				state = btn_hold;
			else
				state = wait;
			break;
		default: state = start;
	}
	switch(state){
		case start:
			PORTC = 0; break;
		case wait:
			break;
		case btn_press:
			PORTC = lights[idx];
			idx++;
			break;
		case btn_hold:
			if(idx == SIZE)
				idx = 0;
			break;
		
		default: PORTC = 0;
	}
}
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0; DDRC = 0xFF; PORTA = 0xFF;  PORTC = 0;
    /* Insert your solution below */
    state = start;
    while (1) {
	Tick();
    }
    return 1;
}
