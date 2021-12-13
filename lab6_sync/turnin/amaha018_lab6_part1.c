/*	Author: Anand Mahadevan
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #6  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
* 	Demo Link: https://www.youtube.com/watch?v=u6mLqfTIUsU
 */
#include "../header/timer.h"
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {start, pb0, pb1, pb2} state;
//unsigned char lights[8] = {0x00, 0x21, 0x12, 0x0C, 0x3F, 0x00, 0x3F};
//const unsigned char SIZE = 8;
//unsigned char idx;
void Tick(){
	switch(state){
		case start:
			PORTB = 0; state = pb0; break;
		case pb0:
			state = pb1;
			break;
		case pb1:
			state = pb2;
			break;
		case pb2:
			state = pb0;
			break;
		default: state = start;
	}
	switch(state){
		case start:
			PORTB = 0; break;
		case pb0:
			PORTB = 0x01;
			break;
		case pb1:
			PORTB = 0x02;
			break;
		case pb2:
			PORTB = 0x04;
			break;
		default: PORTB = 0;
	}
}
int main(void) {
    // Insert DDR and PORT initializations 
	DDRB = 0xFF; PORTB = 0;
    // Insert your solution below 
    state = start;
    TimerSet(1000);
    TimerOn();
    while (1) {
	Tick();
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
