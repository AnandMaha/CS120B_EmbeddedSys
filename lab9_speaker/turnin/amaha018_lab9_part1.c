/*	Author: Anand Mahadevan
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #9  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
* 	Demo Link: https://www.youtube.com/watch?v=D7HUtDWCvjg
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

enum States {start, wait, note_C, note_D, note_E} state;
//unsigned char lights[8] = {0x00, 0x21, 0x12, 0x0C, 0x3F, 0x00, 0x3F};
//const unsigned char SIZE = 8;
//unsigned char idx;
void Tick(){
	unsigned char btn = ~PINA & 0x07;
	switch(state){
		case start:
			set_PWM(0); state = wait; break;
		case wait:
			if(btn == 0x01)
				state = note_C;
			else if(btn == 0x02)
				state = note_D;
			else if(btn == 0x04)
				state = note_E;
			else
				state = wait;

			break;
		case note_C:
			if(btn == 0x01)
				state = note_C;
			else if(btn == 0x02)
				state = note_D;
			else if(btn == 0x04)
				state = note_E;
			else
				state = wait;

			break;
		case note_D:
			if(btn == 0x01)
				state = note_C;
			else if(btn == 0x02)
				state = note_D;
			else if(btn == 0x04)
				state = note_E;
			else
				state = wait;

			break;
		case note_E:
			if(btn == 0x01)
				state = note_C;
			else if(btn == 0x02)
				state = note_D;
			else if(btn == 0x04)
				state = note_E;
			else
				state = wait;

			break;
		default: state = start;
	}
	switch(state){
		case start:
			set_PWM(0); break;
		case wait:
			set_PWM(0);
			break;
		case note_C:
			set_PWM(261.63);
			break;
		case note_D:
			set_PWM(293.66);
			break;
		case note_E:
			set_PWM(329.63);
			break;
		default: set_PWM(0);
	}
}
int main(void) {
    // Insert DDR and PORT initializations 
	DDRA = 0x00; PORTA = 0xFF; DDRB = 0xFF; PORTB = 0;
    // Insert your solution below 
    state = start;
    TimerSet(300);
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
