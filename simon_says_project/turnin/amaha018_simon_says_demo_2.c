/*	Author: Anand Mahadevan
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Musical Simon Says
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
* 	Demo Link: https://www.youtube.com/watch?v=kw54CxuRrDg
 */
#include "../header/timer.h"
#include "../header/adc.h"
#include "../header/scheduler.h"
#include <avr/eeprom.h>
#include <avr/io.h>
#include <stdlib.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum direction {left, right, up, down, mid} dir; // to avoid using abstract 1, 2, etc.. 
enum direction joy_seq[4] = {left, right, down, up}; // for easy joystick checking
unsigned char led_seq[4] = {0x08, 0x01, 0x02, 0x04}; // for easy led output
unsigned char idx = 0; // for both above arrays

unsigned char score; // for current score

unsigned char getHighScore(){ 
	unsigned char hs = eeprom_read_byte(0); 	
	if(hs == 0xFF){ // default if nothing written to byte before
		hs = 0;
	}
	return hs;
}

// make sure this only runs once per game because of limited EEPROM write lifespan
void updateEEPROM(){ 
	unsigned char hs = getHighScore(); 	
	if(hs < score){ // save new highscore only if current score is greater than old score
		eeprom_write_byte(0, score); 
	}
}

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
enum game_states {game_start, game_wait, game_seq, game_delay, game_check, game_win} game_state;
int game_Tick(){
	unsigned char START_out[] = "Simon Says: UP  TO START/HIGH:  ";
	const unsigned char WIN_out[] = "YOU WIN!        DOWN TO RESTART";
	const unsigned char highScore = getHighScore();
	static unsigned char t; // for showing sequence for a while
	switch(game_state){
		case game_start:
			game_state = game_wait; idx = 0; t = 0; score = 0; LCD_DisplayString(1, START_out); LCD_Cursor(31); LCD_WriteData(highScore/10 + '0'); LCD_Cursor(32); LCD_WriteData(highScore%10 + '0'); break;
		case game_wait:
			if(dir != up)
				game_state = game_wait;
			else {
				game_state = game_seq;
				LCD_DisplayString(1, "Sequence:");
			}
			break;
		case game_seq: // wait for the sequence to finish showing
			if(idx == 4){
				game_state = game_delay; // player should mimic now
				idx = 0;
				PORTB = PINB & 0xF0;
			}
			else{
				game_state = game_seq;
			}
			break;
		case game_delay:
			if(t >= 6){
				LCD_DisplayString(1, "Checking Match:");
				game_state = game_check;
				t = 0;
			} else{
				t++;
			}
		break;
		case game_check: // now check for matching
			PORTB = PORTB & 0xF0; // have led only show for a tick
			if(idx == 4){
				game_state = game_win;
				idx = 0;
				LCD_DisplayString(1, WIN_out);
				updateEEPROM();
			} 
			else{
				game_state = game_check;
			}
			break;
		case game_win: // display win
			if(dir != down)
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
			PORTB = (PINB & 0xF0) | (led_seq[idx]);
			LCD_Cursor(17 + idx); 
			LCD_WriteData( joy_seq[idx] );
			idx++;
			break;
		case game_delay:
			break;
		case game_check: // now check for matching
			if(idx == 0)
				LCD_DisplayString_NoClear(17, "                ");
			if(joy_seq[idx] == dir){
				PORTB = (PINB & 0xF0) | (led_seq[idx]);
				LCD_Cursor(17 + idx); 
				LCD_WriteData( joy_seq[idx] );
				score++;
				idx++;
			}
			else if(dir != mid){
				idx = 0;
				score = 0;
				LCD_DisplayString_NoClear(17, "INCORRECT!");
				PORTB = PINB & 0xF0;
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

	//eeprom_write_byte(0, 0); 

	//Build 4 Custom Characters
	unsigned char vent[8] = {0x7, 0x5, 0x7, 0x5, 0x0, 0x18, 0x14, 0x1c};
	unsigned char right_smile[8] = {0xf, 0x10, 0x11, 0x10, 0x14, 0x13, 0x10, 0xf};
	unsigned char amongE[8] = {0x1f, 0x11, 0x11, 0x1f, 0x1f, 0x1f, 0x1b, 0x1b};
	unsigned char sword[8] = {0x8, 0x1e, 0xc, 0xd, 0xc, 0x4, 0x16, 0x2};

	LCD_Custom_Char(0, vent);
	LCD_Custom_Char(1, right_smile);
	LCD_Custom_Char(2, amongE);
	LCD_Custom_Char(3, sword);
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
