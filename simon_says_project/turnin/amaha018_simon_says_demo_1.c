/*	Author: Anand Mahadevan
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Musical Simon Says
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
* 	Demo Link: https://www.youtube.com/watch?v=EHdYm-xK4RE
 */
#include "../header/timer.h"
#include "../header/adc.h"
#include "../header/scheduler.h"
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum direction {left, right, up, down, mid} dir; // to avoid using abstract 1, 2, etc.. 
enum direction joy_seq[] = {left, down, up, right}; // for easy joystick checking
unsigned char led_seq[] = {0x08, 0x02, 0x04, 0x01}; // for easy led output
unsigned char idx = 0; // for both above arrays

// write to dir
void joystick_read(){
    //both axis joystick read
    unsigned short adcX = READ_ADC(1);
    unsigned short adcY = READ_ADC(0);
    // the below is perfectly fine because only 1 of the 5 positions is allowed at a time
    // ex. up-left is not a valid position
    if(adcX <= 150)
	dir = left;
    else if(adcX >= 650)
        dir = right;
    else if(adcY <= 150)
	dir = up;
    else if(adcY >= 850)
        dir = down;
    else 
	dir = mid;
}

enum joy_states {joy_start, joy_check, joy_release} joy_state;
int joy_Tick(){
	joystick_read();
	switch(joy_state){
		case joy_start:
			joy_state = joy_check; break;
		case joy_check:
			if(dir == mid){
				joy_state = joy_check;
			} else{
				joy_state = joy_release;		
			}
			break;
		case joy_release:
			if(dir == mid){
				joy_state = joy_check;
			} else{
				joy_state = joy_release;
				dir = mid; // for only accepting 1 tick of not middle
			}
			break;
		default: joy_state = joy_start;
	}
	switch(joy_state){
		case joy_start:
			break;
		case joy_check:
			break;
		case joy_release:
			break;
		default: break;
	}
	return joy_state;
}

// game logic and LEDs, speaker, LED display output
enum game_states {game_start, game_wait, game_seq, game_check, game_win} game_state;
int game_Tick(){
	const unsigned char START_out[] = "Simon Says      Up to Start";
	const unsigned char WIN_out[] = "YOU WIN!        UP TO RESTART";
	switch(game_state){
		case game_start:
			game_state = game_wait; idx = 0; LCD_DisplayString(1, START_out); break;
		case game_wait:
			if(dir != up)
				game_state = game_wait;
			else {
				game_state = game_seq;
				LCD_DisplayString(1, "Showing Sequence");
			}
			break;
		case game_seq: // wait for the sequence to finish showing
			if(idx == 4){
				game_state = game_check; // player should mimic now
				idx = 0;
	
				LCD_DisplayString(1, "Checking Match");
				PORTB = PORTB & 0xF0;
			}
			else{
				game_state = game_seq;
			}
			break;
		case game_check: // now check for matching
			PORTB = PORTB & 0xF0; // have led only show for a tick
			LCD_DisplayString(1, "Checking Match");
			if(idx == 4){
				game_state = game_win;
				idx = 0;
				LCD_DisplayString(1, WIN_out);
			} 
			else{
				game_state = game_check;
			}
			break;
		case game_win: // display win
			if(dir != up)
				game_state = game_win;
			else
				game_state = game_start;
			break;
		default: game_state = game_start;
	}
	switch(game_state){
		case game_start:
			break;
		case game_wait:
			break;
		case game_seq: // wait for the sequence to finish showing
			PORTB = (PORTB & 0xF0) | (led_seq[idx]);
			idx++;
			break;
		case game_check: // now check for matching
			if(joy_seq[idx] == dir){
				PORTB = (PORTB & 0xF0) | (led_seq[idx]);
				idx++;
			}
			else if(dir != mid){
				idx = 0;
				LCD_DisplayString(1, "INCORRECT!");
				PORTB = PORTB & 0xF0;
			}
			break;
		case game_win: // display win
			PORTB = 0;
			break;
		default: break;
	}
	return game_state;
}

int main(void) {
    // Insert DDR and PORT initializations 
	DDRC = 0xFF; PORTC = 0x00; DDRD = 0xFF; PORTD = 0x00;
	DDRA = 0x00; PORTA = 0xFF; DDRB = 0xFF; PORTB = 0x00;
	ADC_init();
	LCD_init();
    // Insert your solution below 

    static task task1, task2;
    task *tasks[] = { &task1, &task2 };
    const unsigned short numTasks = sizeof(tasks) / sizeof(task*);
    const char start = 0;

    task1.state = start;
    task1.period = 500;
    task1.elapsedTime = task1.period;
    task1.TickFct = &joy_Tick;

    task2.state = start;
    task2.period = 500;
    task2.elapsedTime = task2.period;
    task2.TickFct = &game_Tick;

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
