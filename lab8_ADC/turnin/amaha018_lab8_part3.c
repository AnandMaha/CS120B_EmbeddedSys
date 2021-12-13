/*	Author: Anand Mahadevan
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #8  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
* 	Demo Link: https://www.youtube.com/watch?v=92CYhNQ_bb0
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
	//PORTB = (x & 0xF); // port B has 4 lowest bits
	//PORTD = (x >> 3); // x[9:4] -> D[6:1]
	
	// Turn PD6 on if ADC >= MAX / 2
	if(x >= MAX / 2)
		PORTD = 0x40;
	else
		PORTD = 0;
    }
    return 1;
}
