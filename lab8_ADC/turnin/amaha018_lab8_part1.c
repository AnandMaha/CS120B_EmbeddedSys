/*	Author: Anand Mahadevan
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #8  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
* 	Demo Link: https://www.youtube.com/watch?v=5IwhWq5H9Pc
 */
#include "../header/timer.h"
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void ADC_init(){
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

int main(void) {
    // Insert DDR and PORT initializations 
	DDRA = 0x00; PORTA = 0xFF;
	DDRD = 0xFF; PORTD = 0x00; DDRB = 0xFF; PORTB = 0;
	
    // Insert your solution below 
    ADC_init();
    while (1) {
	unsigned short x = ADC;
	PORTB = (x & 0xF); // port B has 4 lowest bits
	PORTD = (x >> 3); // x[9:4] -> D[6:1]
    }
    return 1;
}
