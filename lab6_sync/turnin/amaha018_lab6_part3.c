/*	Author: Anand Mahadevan
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #6  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
* 	Demo Link: https://www.youtube.com/watch?v=AujMsKQWl4U
 */
#include "../header/timer.h"
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {start, wait, inc, dec, set_0} state;
//unsigned char lights[8] = {0x00, 0x21, 0x12, 0x0C, 0x3F, 0x00, 0x3F};
//const unsigned char SIZE = 8;
//unsigned char idx;
unsigned char t; // keep time for inc and dec
void Tick(){
	unsigned char btn = ~PINA & 0x03;
	switch(state){
		case start:
			PORTB = 7; state = wait; break;
		case wait:
			if(btn == 0x03)
				state = set_0;
			else if(btn == 0x02)
				state = dec;
			else if(btn == 0x01)
				state = inc; 
			else // btn == 0
				state = wait;
			break;
		case inc:
			if(btn == 0x03)
				state = set_0;
			else if(btn == 0x02)
				state = dec;
			else if(btn == 0x01)
				state = inc; 
			else // btn == 0
				state = wait;

			if(btn == 0x01)
				t++;
			else
				t = 0;
			break;
		case dec:
			if(btn == 0x03)
				state = set_0;
			else if(btn == 0x02)
				state = dec;
			else if(btn == 0x01)
				state = inc; 
			else // btn == 0
				state = wait;

			if(btn == 0x02)
				t++;
			else
				t = 0;
			break;
		case set_0:
			if(btn == 0x03)
				state = set_0;
			else if(btn == 0x02)
				state = dec;
			else if(btn == 0x01)
				state = inc; 
			else // btn == 0
				state = wait;
			break;
		default: state = start;
	}
	switch(state){
		case start:
			PORTB = 7; break;
		case wait:
			break;
		case inc:
			if(PINB < 9 && t % 10 == 0)
				PORTB++; 
			break;
		case dec:
			if(PINB > 0 && t % 10 == 0)
				PORTB--; 
			break;
		case set_0:
			PORTB = 0; break;
		default: PORTB = 7;
	}
}
int main(void) {
    // Insert DDR and PORT initializations 
	DDRA = 0x00; PORTA = 0xFF; DDRB = 0xFF; PORTB = 0;
    // Insert your solution below 
    state = start;
    TimerSet(100);
    TimerOn();
    while (1) {
	Tick();
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
