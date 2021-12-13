/*	Author: Anand Mahadevan
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #11  Exercise #5
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
* 	Demo Link: https://www.youtube.com/watch?v=0eC5VlZp7Bc
 */
#include "../header/timer.h"
#include "../header/keypad.h"
#include "../header/scheduler.h"
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char pause; // boolean for start / pause
unsigned char u_pos; // 1 for top row, 17 for bottom

enum pause_states {pause_start, pause_check, pause_release} pause_state;
int pause_Tick(){
	unsigned char btn = ~PINA & 0x04;
	switch(pause_state){
		case pause_start:
			pause_state = pause_check; pause = 1; break;
		case pause_check:
			if(btn){
				pause_state = pause_release;
				pause = ~pause & 0x01;
			} else{
				pause_state = pause_check;
			}
			break;
		case pause_release:
			if(btn){
				pause_state = pause_release;
			} else{
				pause_state = pause_check;
			}
			break;
		default: pause_state = pause_start;
	}
	switch(pause_state){
		case pause_start:
			break;
		case pause_check:
			break;
		case pause_release:
			break;
		default: break;
	}
	return pause_state;
}

enum user_states {user_start, user_check, user_release} user_state;
int user_Tick(){
	unsigned char btn = ~PINA & 0x03;
	switch(user_state){
		case user_start:
			user_state = user_check; u_pos = 17; break;
		case user_check:
			if(btn == 0x01){
				user_state = user_release;
				u_pos = 1; 
			} else if(btn == 0x02){
				user_state = user_release;
				u_pos = 17;
			} else
				user_state = user_check;
			break;
		case user_release:
			if(btn == 0x01 || btn == 0x02){
				user_state = user_release;
			} else{
				user_state = user_check;
			}
			break;
		default: user_state = user_start;
	}
	switch(user_state){
		case user_start:
			break;
		case user_check:
			break;
		case user_release:
			break;
		default: break;
	}
	return user_state;
}

enum msg_states {msg_start, msg_display, msg_lose} msg_state;
int msg_Tick(){
	static unsigned char idx;
	const unsigned char OBSTACLE_1[] = "^               "; 
	const unsigned char OBSTACLE_2[] = "     ^          ";
	const unsigned char START_MSG[] = "PLAY DODGE THE  CARET? PRESS A3 ";
	const unsigned char LOSE_MSG[] = "YOU LOSE!       HAHAHAHA!";
	switch(msg_state){
		case msg_start:
			msg_state = msg_display; idx = 0; LCD_DisplayString(1, START_MSG); break;
		case msg_display:
			if(OBSTACLE_1[idx] == '^' && u_pos == 1 || OBSTACLE_2[idx] == '^' && u_pos == 17){
				msg_state = msg_lose; pause = 1; LCD_DisplayString(1, LOSE_MSG);
			}
			else
				msg_state = msg_display;
			break;
		case msg_lose:
			if(pause)
				msg_state = msg_lose;
			else{
				msg_state = msg_display;
				idx = 0; u_pos = 17;
			}
			break;
		default: msg_state = msg_start;
	}
	switch(msg_state){
		case msg_start:
			break;
		case msg_display:
			if(!pause){
				//print out the obstacles, then set cursor to u_pos
				for(unsigned i = 0; i < 16; i++){
					LCD_Cursor(i+1); LCD_WriteData(OBSTACLE_1[(idx+i) % 16]);
					LCD_Cursor(i+17); LCD_WriteData(OBSTACLE_2[(idx+i) % 16]);
				}
				idx = (idx+1 == 16) ? 0 : idx+1;
				LCD_Cursor(u_pos);
			}
			break;
		case msg_lose:
			
			break;
		default: break;
	}
	return msg_state;
}

int main(void) {
    // Insert DDR and PORT initializations 
	DDRC = 0xFF; PORTC = 0x00; DDRD = 0xFF; PORTD = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	LCD_init();
    // Insert your solution below 

    static task task1, task2, task3;
    task *tasks[] = { &task1, &task2, &task3 };
    const unsigned short numTasks = sizeof(tasks) / sizeof(task*);
    const char start = 0;

    task1.state = start;
    task1.period = 100;
    task1.elapsedTime = task1.period;
    task1.TickFct = &pause_Tick;

    task2.state = start;
    task2.period = 100;
    task2.elapsedTime = task2.period;
    task2.TickFct = &user_Tick;

    task3.state = start;
    task3.period = 250;
    task3.elapsedTime = task3.period;
    task3.TickFct = &msg_Tick;

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
