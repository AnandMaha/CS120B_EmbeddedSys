/*	Author: Anand Mahadevan
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #11  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
* 	Demo Link: https://www.youtube.com/watch?v=i4qjJYcnL8E
 */
#include "../header/timer.h"
#include "../header/keypad.h"
#include "../header/scheduler.h"
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

/*
enum key_states {key_start, key_display} key_state;
int key_Tick(){
	switch(key_state){
		case key_start:
			key_state = key_display; break;
		case key_display:
			key_state = key_display;
			break;
		default: key_state = key_start;
	}
	switch(key_state){
		case key_start:
			break;
		case key_display:;
			unsigned char x = GetKeypadKey();
			switch(x){
				case '\0': PORTB = 0x1F; break;
				case '1': PORTB = 0x01; break;
				case '2': PORTB = 0x02; break;
				case '3': PORTB = 0x03; break;
				case '4': PORTB = 0x04; break;
				case '5': PORTB = 0x05; break;
				case '6': PORTB = 0x06; break;
				case '7': PORTB = 0x07; break;
				case '8': PORTB = 0x08; break;
				case '9': PORTB = 0x09; break;
				case 'A': PORTB = 0x0A; break;
				case 'B': PORTB = 0x0B; break;
				case 'C': PORTB = 0x0C; break;
				case 'D': PORTB = 0x0D; break;
				case '*': PORTB = 0x0E; break;
				case '0': PORTB = 0x00; break;
				case '#': PORTB = 0x0F; break;
				default: PORTB = 0x1B; break;
			}
			break;
		default: break;
	}
	return key_state;
}
*/


enum msg_states {msg_start, msg_display} msg_state;
int msg_Tick(){
	static unsigned char idx;
	const unsigned char MSG[] = "     CS120B is Legend... wait for it DARY!";
	unsigned char tmp;
	const unsigned char MSG_L = 42;
	switch(msg_state){
		case msg_start:
			msg_state = msg_display; idx = 0; break;
		case msg_display:
			msg_state = msg_display;
			break;
		default: msg_state = msg_start;
	}
	switch(msg_state){
		case msg_start:
			break;
		case msg_display:;
			//PLAN: display substring of 16 characters by write data at the correct cursor position
			//Update each character in tmp each runthrough, have idx be starting position in MSG
			for(unsigned char i = 0; i < 16; i++){
				tmp = MSG[ (idx + i) % MSG_L ]; // idx start, i to get 16 substring
				LCD_Cursor(i+1); LCD_WriteData(tmp);
			}

			idx = (idx+1 == MSG_L) ? 0 : idx + 1; // if the next increment would make start invalid, simple reset

			break;
		default: break;
	}
	return msg_state;
}

int main(void) {
    // Insert DDR and PORT initializations 
	DDRC = 0xFF; PORTC = 0x00; DDRD = 0xFF; PORTD = 0x00;
	LCD_init();
    // Insert your solution below 

    static task task1;
    task *tasks[] = { &task1 };
    const unsigned short numTasks = sizeof(tasks) / sizeof(task*);
    const char start = 0;
    task1.state = start;
    task1.period = 500;
    task1.elapsedTime = task1.period;
    task1.TickFct = &msg_Tick;

    unsigned long GCD = tasks[0]->period;
    for(unsigned i = 1; i < numTasks; i++){
	GCD = findGCD(GCD, tasks[i]->period);
    }

    TimerSet(GCD);
    TimerOn();


    while (1) {
	for (unsigned short i = 0; i < numTasks; ++i) {
		if (tasks[i]->elapsedTime == tasks[i]->period) {
			tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			tasks[i]->elapsedTime = 0;
		}
		tasks[i]->elapsedTime += GCD;
	}
	while(!TimerFlag);
	TimerFlag = 0;
    }
    

    return 1;
}
