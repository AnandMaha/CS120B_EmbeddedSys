/*	Author: Anand Mahadevan
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #11  Exercise #4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
* 	Demo Link: https://www.youtube.com/watch?v=95dj1571_A0
 */
#include "../header/timer.h"
#include "../header/keypad.h"
#include "../header/scheduler.h"
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char input; // for storing keypad entry and outputting to LCD display

enum key_states {key_start, key_display, key_release} key_state;
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
			input = GetKeypadKey();
			break;
		default: break;
	}
	return key_state;
}

enum msg_states {msg_start, msg_display} msg_state;
int msg_Tick(){
	static unsigned char idx;
	const unsigned char MSG[] = "Congratulations!";
	switch(msg_state){
		case msg_start:
			msg_state = msg_display; idx = 1; LCD_DisplayString(1, MSG); break;
		case msg_display:
			msg_state = msg_display;
			break;
		default: msg_state = msg_start;
	}
	switch(msg_state){
		case msg_start:
			break;
		case msg_display:
			if(input != '\0'){
				LCD_Cursor(idx); LCD_WriteData(input); 
				idx = (idx + 1 == 17) ? 1 : idx+1;
			}
			break;
		default: break;
	}
	return msg_state;
}

int main(void) {
    // Insert DDR and PORT initializations 
	DDRC = 0xFF; PORTC = 0x00; DDRD = 0xFF; PORTD = 0x00;
	DDRA = 0xF0; PORTA = 0x0F;
	LCD_init();
    // Insert your solution below 

    static task task1, task2;
    task *tasks[] = { &task1, &task2 };
    const unsigned short numTasks = sizeof(tasks) / sizeof(task*);
    const char start = 0;

    task1.state = start;
    task1.period = 100;
    task1.elapsedTime = task1.period;
    task1.TickFct = &key_Tick;

    task2.state = start;
    task2.period = 1000;
    task2.elapsedTime = task2.period;
    task2.TickFct = &msg_Tick;

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
