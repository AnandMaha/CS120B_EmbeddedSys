/*	Author: Anand Mahadevan
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #7  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
* 	Demo Link: https://www.youtube.com/watch?v=Yv9J5_Q-csk
 */
#include "../header/timer.h"
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {start, pb0, pb1, pb2, btn_press, wait} state;
//unsigned char lights[8] = {0x00, 0x21, 0x12, 0x0C, 0x3F, 0x00, 0x3F};
//const unsigned char SIZE = 8;
//unsigned char idx;
unsigned char prev_state;
unsigned char num;
unsigned char win;
unsigned char do_once;
void Tick(){
	unsigned char btn = ~PINA & 0x01;
	switch(state){
		case start:
			PORTB = 0; win = 0; do_once = 1; num = 5; LCD_Cursor(1); LCD_WriteData(num + '0'); LCD_Cursor(2); state = pb0; break;
		case pb0:
			if(btn){
				state = btn_press; win = 0;
			}
			else
				state = pb1;
			break;
		case pb1:
			if(btn){
				state = btn_press; win = 1;
			}
			else if(prev_state == pb0)
				state = pb2;
			else if(prev_state == pb2)
				state = pb0;
			break;
		case pb2:
			if(btn) {
				state = btn_press; win = 0;
			}
			else
				state = pb1;
			break;
		case btn_press: // must release button to go to wait
			if(btn){
				state = btn_press; do_once = 0;
			}
			else
				state = wait;
			break;
		case wait: // wait till btn press to restart game
			if(btn) 
				state = pb0;
			else
				state = wait;
			break;
		default: state = start;
	}
	switch(state){
		case start:
			PORTB = 0; num = 5; LCD_Cursor(1); LCD_WriteData(num + '0'); LCD_Cursor(2); break;
		case pb0:
			LCD_ClearScreen(); PORTB = 0x01; prev_state = pb0;LCD_Cursor(1); LCD_WriteData(num + '0'); LCD_Cursor(2);
			break;
		case pb1:
			PORTB = 0x02; LCD_Cursor(1); LCD_WriteData(num + '0'); LCD_Cursor(2);
			break;
		case pb2:
			PORTB = 0x04; prev_state = pb2;LCD_Cursor(1); LCD_WriteData(num + '0'); LCD_Cursor(2);
			break;
		case btn_press:
			if(do_once){			
				if(win && num < 9)
					{num++;}
				else if(!win && num > 0)
					{num--;}
				LCD_Cursor(1); LCD_WriteData(num + '0'); LCD_Cursor(2);
				if(num == 9){
					LCD_DisplayString(1, "YOU WIN!!! Press A0 to continue playing.");
					num = 5; // initial score for reset
				}
				prev_state = pb0; 
			}
			break;
		case wait:
			do_once = 1; break;
		default: PORTB = 0; num = 5; LCD_Cursor(1); LCD_WriteData(num + '0'); LCD_Cursor(2); 
	}
}
int main(void) {
    // Insert DDR and PORT initializations 
	DDRC = 0xFF; PORTC = 0x00; DDRD = 0xFF; PORTD = 0x00;
	DDRA = 0x00; PORTA = 0xFF; DDRB = 0xFF; PORTB = 0;
	LCD_init();
    // Insert your solution below 
    state = start;
    TimerSet(300);
    TimerOn();
    while (1) {
	Tick();
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
