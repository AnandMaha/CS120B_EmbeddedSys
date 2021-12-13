/*	Author: Anand Mahadevan
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #10  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
* 	Demo Link: https://www.youtube.com/shorts/sG5s0Zp5Ckk
 */
#include "../header/timer.h"
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

const unsigned short syncPeriod = 1;
const unsigned short threeLEDPeriod = 300;
const unsigned short blinkLEDPeriod = 1000;
const unsigned short combineLEDPeriod = 1;
unsigned char threeLEDs, blinkingLED;

enum three_States {three_start, three_pb0, three_pb1, three_pb2} three_state;
void three_Tick(){
	switch(three_state){
		case three_start:
			three_state = three_pb0; threeLEDs = 1; break;
		case three_pb0:
			three_state = three_pb1;
			break;
		case three_pb1:
			three_state = three_pb2;
			break;
		case three_pb2:
			three_state = three_pb0;
			break;
		default: three_state = three_start;
	}
	switch(three_state){
		case three_start:
			break;
		case three_pb0:
			threeLEDs = 1;
			break;
		case three_pb1:
			threeLEDs = 2;
			break;
		case three_pb2:
			threeLEDs = 4;
			break;
		default: threeLEDs = 1;
	}
}

enum blink_States {blink_start, blink_blink} blink_state;
void blink_Tick(){
	switch(blink_state){
		case blink_start:
			blink_state = blink_blink; blinkingLED = 1; break;
		case blink_blink:
			blink_state = blink_blink;
			break;
		default: blink_state = blink_start;
	}
	switch(blink_state){
		case blink_start:
			break;
		case blink_blink:
			blinkingLED = ~(blinkingLED & 0x01);
			break;
		default: blinkingLED = 0;
	}
}


enum combineStates {combine_start, combine_combine, pb1, pb2} combineStates;
void combine_Tick(){
	switch(combineStates){
		case combine_start:
			combineStates = combine_combine; break;
		case combine_combine:
			combineStates = combine_combine;
			break;
		default: combineStates = combine_start;
	}
	switch(combineStates){
		case combine_start:
			PORTB = 0; break;
		case combine_combine:
			PORTB = ((blinkingLED << 3) | (threeLEDs));
			break;
		default: PORTB = 0;
	}
}

int main(void) {
    // Insert DDR and PORT initializations 
	DDRB = 0xFF; PORTB = 0;
    // Insert your solution below 
    unsigned short three_elTime = threeLEDPeriod;
    unsigned short blink_elTime = blinkLEDPeriod;
    unsigned short combine_elTime = combineLEDPeriod;

    three_state = three_start;
    blink_state = blink_start;
    combineStates = combine_start;

    TimerSet(syncPeriod);
    TimerOn();

    while (1) {
	if(three_elTime >= threeLEDPeriod){
		three_Tick(); three_elTime = 0;
	}
	if(blink_elTime >= blinkLEDPeriod){
		blink_Tick(); blink_elTime = 0;
	}
	if(combine_elTime >= combineLEDPeriod){
		combine_Tick(); combine_elTime = 0;
	}
	while(!TimerFlag);
	TimerFlag = 0;
	three_elTime += syncPeriod;
	blink_elTime += syncPeriod;
	combine_elTime += syncPeriod;
    }
    return 1;
}
