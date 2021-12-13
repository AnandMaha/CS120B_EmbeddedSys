/*	Author: Anand Mahadevan
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #9  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
* 	Demo Link: https://www.youtube.com/watch?v=g0S8I8Hn4cA
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

enum States {start, wait, playSong, wait_release} state;
//double notes[8] = {261.63,293.66,329.63,349.23,392.00,440.00,493.88,523.23};
//const unsigned char SIZE = 8;
//unsigned char idx;
unsigned char t;
void Tick(){
	unsigned char btn = ~PINA & 0x01; // A0 starts melody
	switch(state){
		case start:
			set_PWM(0); t = 0; state = wait; break;
		case wait:
			if(btn == 0x01)
				state = playSong;
			else
				state = wait;

			break;
		case playSong:
			if(t < 50)
				state = playSong;
			else
				state = wait_release;
			break;
		case wait_release:
			if(btn != 0x00)
				state = wait_release;
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
		case playSong:
			if(t < 5)
				set_PWM(440);
			else if(t < 10)
				set_PWM(349.23);
			else if(t < 15)
				set_PWM(392.00);
			else if(t < 18)
				set_PWM(0);
			else if(t < 23)
				set_PWM(392.00);
			else if(t < 28)
				set_PWM(329.63);
			else if(t < 33)
				set_PWM(349.23);
			else if(t < 36)
				set_PWM(0);
			else if(t < 41)
				set_PWM(349.23);
			else if(t < 45)
				set_PWM(293.66);
			else if(t < 50)
				set_PWM(261.63);

			t++;
			break;
		case wait_release:
			t = 0;
			set_PWM(0);
			break;
		default: state = start;
	}
}
int main(void) {
    // Insert DDR and PORT initializations 
	DDRA = 0x00; PORTA = 0xFF; DDRB = 0xFF; PORTB = 0;
    // Insert your solution below 
    state = start;
    TimerSet(100);
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
