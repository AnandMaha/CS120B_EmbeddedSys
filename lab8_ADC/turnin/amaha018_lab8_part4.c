/*	Author: Anand Mahadevan
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #8  Exercise #4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
* 	Demo Link: https://www.youtube.com/watch?v=cyHeqXXkTN8
 */
#include "../header/timer.h"
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void ADC_init(){
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

const unsigned short MAX = 0x3FF; // determined in lab 8 part 2 to be 0x3FF
int main(void) {
    // Insert DDR and PORT initializations 
	DDRA = 0x00; PORTA = 0xFF;
	DDRD = 0xFF; PORTD = 0x00; DDRB = 0xFF; PORTB = 0;
	
    // Insert your solution below 
    ADC_init();
    while (1) {
	// Max ADC value - 0x2FF (From flashlight)
	//Min ADC value 0x1F (From covering hand over photoresistor)
	unsigned short x = ADC;
	// determine which led should turn on out of the 8
	// start from least to most
	unsigned short m8 = MAX / 8; // make typing easier
	//OUTPUT GLITCHING WILL NOT OCCUR
	if(x <= m8){
		PORTB = 0x01; PORTD = 0x00;
	}
	else if(x <= 2*m8){
		PORTB = 0x03; PORTD = 0x00;
	}
	else if(x <= 3*m8){
		PORTB = 0x07; PORTD = 0x00;
	}
	else if(x <= 4*m8){
		PORTB = 0x0F; PORTD = 0x00;
	}
	else if(x <= 5*m8){
		PORTB = 0x0F; PORTD = 0x02;
	}
	else if(x <= 6*m8){
		PORTB = 0x0F; PORTD = 0x06;
	}
	else if(x <= 7*m8){
		PORTB = 0x0F; PORTD = 0x0E;
	}
	else if(x <= 8*m8){
		PORTB = 0x0F; PORTD = 0x1E;
	}
    }
    return 1;
}
