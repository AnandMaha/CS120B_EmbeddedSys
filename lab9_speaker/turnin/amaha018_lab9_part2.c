/*	Author: Anand Mahadevan
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #9  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
* 	Demo Link: https://www.youtube.com/watch?v=NwKTBozplEo
 */
#include "../header/timer.h"
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void set_PWM(double frequency) {
	static double current_frequency;
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; }
		else { TCCR3B |= 0x03; }
		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		else if (frequency > 31250) { OCR3A = 0x0000; }
		else { OCR3A = (short) (8000000 / (128 * frequency)) -1; }
		TCNT3 = 0;
		current_frequency = frequency;
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

enum States {start, wait, turnOnOff, scaleUp, scaleDown, release} state;
double notes[8] = {261.63,293.66,329.63,349.23,392.00,440.00,493.88,523.23};
const unsigned char SIZE = 8;
unsigned char idx;
unsigned char isOn;
void Tick(){
	unsigned char btn = ~PINA & 0x07; // A0 turns on / off, A1 scales up, A2 scales down
	switch(state){
		case start:
			set_PWM(0); isOn = idx = 0; state = wait; break;
		case wait:
			if(btn == 0x01)
				state = turnOnOff;
			else if(btn == 0x02)
				state = scaleUp;
			else if(btn == 0x04)
				state = scaleDown;
			else
				state = wait;

			break;
		case turnOnOff:
			state = release;

			break;
		case scaleUp:
			state = release;

			break;
		case scaleDown:
			state = release; 
			break;
		case release:
			if(btn != 0x00)
				state = release;
			else
				state = wait;
			break;
		default: state = start;
	}
	switch(state){
		case start:
			set_PWM(0); break;
		case wait:
			break;
		case turnOnOff:
			if(isOn)
				set_PWM(0);	// if it's on turn it off
			else
				set_PWM(notes[idx]);	// if it's off turn it on
			isOn = ~isOn & 0x01; // invert last bit
			break;
		case scaleUp:
			if(idx < SIZE-1)
				idx++;
			if(isOn)
				set_PWM(notes[idx]);
			break;
		case scaleDown:
			if(idx > 0)
				idx--;
			if(isOn)
				set_PWM(notes[idx]);
			break;
		case release:
			break;
		default: set_PWM(0);
	}
}
int main(void) {
    // Insert DDR and PORT initializations 
	DDRA = 0x00; PORTA = 0xFF; DDRB = 0xFF; PORTB = 0;
    // Insert your solution below 
    state = start;
    TimerSet(50);
    TimerOn();
    PWM_on();
    while (1) {
	Tick();
	while(!TimerFlag);
	TimerFlag = 0;
    }
    PWM_off();
    return 1;
}
