/*	Author: Anand Mahadevan
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #6  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
* 	Demo Link: https://www.youtube.com/watch?v=KlbqLtCoxTk
 */
#include "../header/timer.h"
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {start, pb0, pb1, pb2, btn_press, wait} state;
//unsigned char lights[8] = {0x00, 0x21, 0x12, 0x0C, 0x3F, 0x00, 0x3F};
//const unsigned char SIZE = 8;
//unsigned char idx;
unsigned char prev_state;
void Tick(){
	unsigned char btn = ~PINA & 0x01;
	switch(state){
		case start:
			PORTB = 0; state = pb0; break;
		case pb0:
			if(btn)
				state = btn_press;
			else
				state = pb1;
			break;
		case pb1:
			if(btn)
				state = btn_press;
			else if(prev_state == pb0)
				state = pb2;
			else if(prev_state == pb2)
				state = pb0;
			break;
		case pb2:
			if(btn)
				state = btn_press;
			else
				state = pb1;
			break;
		case btn_press: // must release button to go to wait
			if(btn)
				state = btn_press;
			else
				state = wait;
			break;
		case wait: // wait till btn press to restart game
			if(btn) 
				state = pb0;
			else
				state = wait;
			break;
		default: state = start;
	}
	switch(state){
		case start:
			PORTB = 0; break;
		case pb0:
			PORTB = 0x01; prev_state = pb0;
			break;
		case pb1:
			PORTB = 0x02;
			break;
		case pb2:
			PORTB = 0x04; prev_state = pb2;
			break;
		case btn_press:
			prev_state = pb0; break;
		case wait:
			break;
		default: PORTB = 0;
	}
}
int main(void) {
    // Insert DDR and PORT initializations 
	DDRA = 0x00; PORTA = 0xFF; DDRB = 0xFF; PORTB = 0;
    // Insert your solution below 
    state = start;
    TimerSet(300);
    TimerOn();
    while (1) {
	Tick();
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
