/*	Author: Anand Mahadevan
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #7  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
* 	Demo Link: https://www.youtube.com/watch?v=aNFI0KOETHU
 */
#include "../header/timer.h"
#include <avr/io.h>
#include "../header/io.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {start, wait, inc, dec, set_0} state;
//unsigned char lights[8] = {0x00, 0x21, 0x12, 0x0C, 0x3F, 0x00, 0x3F};
//const unsigned char SIZE = 8;
//unsigned char idx;
unsigned char t; // keep time for inc and dec
unsigned char num;
void Tick(){
	unsigned char btn = ~PINA & 0x03;
	switch(state){
		case start:
			num = 0; LCD_Cursor(1); LCD_WriteData(num + '0'); LCD_Cursor(2); state = wait; break;
		case wait:
			if(btn == 0x03)
				state = set_0;
			else if(btn == 0x02)
				state = dec;
			else if(btn == 0x01)
				state = inc; 
			else // btn == 0
				state = wait;
			break;
		case inc:
			if(btn == 0x03)
				state = set_0;
			else if(btn == 0x02)
				state = dec;
			else if(btn == 0x01)
				state = inc; 
			else // btn == 0
				state = wait;

			if(btn == 0x01)
				t++;
			else
				t = 0;
			break;
		case dec:
			if(btn == 0x03)
				state = set_0;
			else if(btn == 0x02)
				state = dec;
			else if(btn == 0x01)
				state = inc; 
			else // btn == 0
				state = wait;

			if(btn == 0x02)
				t++;
			else
				t = 0;
			break;
		case set_0:
			if(btn == 0x03)
				state = set_0;
			else if(btn == 0x02)
				state = dec;
			else if(btn == 0x01)
				state = inc; 
			else // btn == 0
				state = wait;
			break;
		default: state = start;
	}
	switch(state){
		case start:
			num = 0; LCD_Cursor(1); LCD_WriteData(num + '0'); LCD_Cursor(2); PORTB = start; break;
		case wait:
			PORTB = wait;break;
		case inc:
			if(num < 9 && t % 10 == 0)
				num++; LCD_Cursor(1); LCD_WriteData(num + '0'); LCD_Cursor(2); PORTB = wait;
			break;
		case dec:
			if(num > 0 && t % 10 == 0)
				num--; LCD_Cursor(1); LCD_WriteData(num + '0'); LCD_Cursor(2);PORTB = dec;
			break;
		case set_0:
			num = 0; LCD_Cursor(1); LCD_WriteData(num + '0'); LCD_Cursor(2); PORTB = set_0;break;
		default: num = 0; LCD_Cursor(1); LCD_WriteData(num + '0'); LCD_Cursor(2);
	}
}
int main(void) {
    // Insert DDR and PORT initializations 
	DDRC = 0xFF; PORTC = 0x00; DDRD = 0xFF; PORTD = 0x00;
	DDRA = 0; PORTA = 0xFF; DDRB = 0xFF; PORTB = 0;
	LCD_init();

    // Insert your solution below 
    state = start;
    TimerSet(100);
    TimerOn();
    while (1) {
	Tick();
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;

}
