/*	Author: Anand Mahadevan
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #2  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
int main(void) {
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 	0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs
				// Initialize output on PORTC to 0x00
	unsigned char cntOpen = 0;
	unsigned char tmp3, tmp2, tmp1, tmp0 = 0; // get each bit
	while(1) {
		//need to count 0 bits in [A3:A0] and set that to PORTC
		tmp3 = ((PINA & 0x08) == 0x08) ? 0 : 1;
		tmp2 = ((PINA & 0x04) == 0x04) ? 0 : 1;
		tmp1 = ((PINA & 0x02) == 0x02) ? 0 : 1;
		tmp0 = ((PINA & 0x01) == 0x01) ? 0 : 1;
		cntOpen = tmp3 + tmp2 + tmp1 + tmp0;
		PORTC = cntOpen;

	}
	return 1;
}

