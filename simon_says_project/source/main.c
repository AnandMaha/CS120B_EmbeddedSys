/*	Author: Anand Mahadevan
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Musical Simon Says
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
* 	Demo Link: https://www.youtube.com/watch?v=jlmExL7IwaY
 */
#include "../header/timer.h"
#include "../header/adc.h"
#include "../header/scheduler.h"
#include <avr/eeprom.h>
#include <avr/io.h>
#include <stdlib.h>
#include <math.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"

#define MAX_SEQ 5 // for max sequence length

#endif

enum direction {right, down, up, left, mid} dir; // to avoid using abstract 1, 2, etc.. 

enum direction joy_seq[MAX_SEQ]; // for easy joystick checking
unsigned char led_seq[MAX_SEQ]; // for easy led output

unsigned char score; // for current score
unsigned char rd; // random direction determine next input, range [0,3]

unsigned char getHighScore(){ 
	unsigned char hs = eeprom_read_byte(0); 	
	if(hs == 0xFF){ // default if nothing written to byte before
		hs = 0;
	}
	return hs;
}

// make sure this only runs once per game because of limited EEPROM write lifespan
void updateHighscore(){ // call when incorrect input entered, and max sequence receached
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

enum rand_states {rand_start, rand_1,rand_2,rand_3,rand_4,rand_5,rand_6,rand_7,rand_8,rand_9,rand_10,rand_11,
rand_12,rand_13,rand_14,rand_15,rand_16, rand_17} rand_state;
int rand_Tick(){ // use joystick dir, B[3:0], (PINC, D6, and D7)for generator input
	unsigned char B0 = PINB & 0x01;
	unsigned char B1 = (PINB & 0x02) >> 1;
	unsigned char B2 = (PINB & 0x04) >> 2;
        unsigned char B3 = (PINB & 0x08) >> 3;
        unsigned char C0 = PINC & 0x01;
        unsigned char C1 = (PINC & 0x02) >> 1;
        unsigned char C2 = (PINC & 0x04) >> 2;
        unsigned char C3 = (PINC & 0x08) >> 3;
        unsigned char C4 = (PINC & 0x10) >> 4;
        unsigned char C5 = (PINC & 0x20) >> 5;
        unsigned char C6 = (PINC & 0x40) >> 6;
        unsigned char C7 = (PINC & 0x80) >> 7;
        unsigned char D6 = (PIND & 0x40) >> 6;
        unsigned char D7 = (PIND & 0x80) >> 7;
              
	switch(rand_state){
		case rand_start:
			rand_state = (rand() % 17) + 1; break;
		case rand_1:
			if(B0)
				rand_state = rand_3;
			else if(C4)
				rand_state = rand_7;
			else if(C7)
				rand_state = rand_13;
			else if(C2)
				rand_state = rand_7;
			else if(B3)
				rand_state = rand_3;
			else if(dir == left)
				rand_state = rand_6;
			else if(B2)
				rand_state = rand_9;
			else if(D6)
				rand_state = rand_16;
			else if(C3)
				rand_state = rand_8;
			else if(dir == right)
				rand_state = rand_10;
			break;
		case rand_2:
			if(dir == down)
				rand_state = rand_15;
			else if(B1)
				rand_state = rand_11;
			else if(C3)
				rand_state = rand_3;
			else if(C5)
				rand_state = rand_1;
			else if(D7)
				rand_state = rand_8;
			else if(dir == right)
				rand_state = rand_13;
			else if(C2)
				rand_state = rand_11;
			else if(dir == left)
				rand_state = rand_5;
			else if(B0)
				rand_state = rand_10;
			else if(C7)
				rand_state = rand_7;
			break;
		case rand_3:
			if(C5)
				rand_state = rand_17;
			else if(dir == left)
				rand_state = rand_13;
			else if(B0)
				rand_state = rand_12;
			else if(C2)
				rand_state = rand_8;
			else if(C1)
				rand_state = rand_2;
			else if(D6)
				rand_state = rand_9;
			else if(dir == up)
				rand_state = rand_4;
			else if(B3)
				rand_state = rand_1;
			else if(C6)
				rand_state = rand_13;
			else if(C0)
				rand_state = rand_6;
			break;
		case rand_4:
			if(C3)
				rand_state = rand_9;
			else if(D6)
				rand_state = rand_4;
			else if(C2)
				rand_state = rand_15;
			else if(C7)
				rand_state = rand_1;
			else if(dir == down)
				rand_state = rand_7;
			else if(dir == up)
				rand_state = rand_12;
			else if(C0)
				rand_state = rand_11;
			else if(dir == left)
				rand_state = rand_10;
			else if(C1)
				rand_state = rand_3;
			else if(B2)
				rand_state = rand_8;
			break;
		case rand_5:
			if(B1)
				rand_state = rand_8;
			else if(dir == left)
				rand_state = rand_1;
			else if(D7)
				rand_state = rand_5;
			else if(C4)
				rand_state = rand_16;
			else if(C2)
				rand_state = rand_3;
			else if(dir == right)
				rand_state = rand_2;
			else if(C6)
				rand_state = rand_9;
			else if(C5)
				rand_state = rand_6;
			else if(B3)
				rand_state = rand_11;
			else if(dir == right)
				rand_state = rand_15;
			break;
		case rand_6:
			if(C6)
				rand_state = rand_7;
			else if(C1)
				rand_state = rand_6;
			else if(dir == left)
				rand_state = rand_12;
			else if(C5)
				rand_state = rand_7;
			else if(D7)
				rand_state = rand_15;
			else if(dir == right)
				rand_state = rand_8;
			else if(C2)
				rand_state = rand_1;
			else if(dir == left)
				rand_state = rand_2;
			else if(B2)
				rand_state = rand_3;
			else if(C0)
				rand_state = rand_16;
			break;
		case rand_7:
			if(C2)
				rand_state = rand_5;
			else if(C0)
				rand_state = rand_7;
			else if(dir == up)
				rand_state = rand_16;
			else if(C5)
				rand_state = rand_1;
			else if(C1)
				rand_state = rand_12;
			else if(C6)
				rand_state = rand_17;
			else if(C3)
				rand_state = rand_13;
			else if(C2)
				rand_state = rand_9;
			else if(C4)
				rand_state = rand_15;
			else if(D7)
				rand_state = rand_11;
			break;
		case rand_8:
			if(B2)
				rand_state = rand_9;
			else if(C7)
				rand_state = rand_8;
			else if(D6)
				rand_state = rand_12;
			else if(C1)
				rand_state = rand_2;
			else if(C0)
				rand_state = rand_6;
			else if(dir == down)
				rand_state = rand_4;
			else if(B0)
				rand_state = rand_15;
			else if(C3)
				rand_state = rand_3;
			else if(C6)
				rand_state = rand_10;
			else if(B3)
				rand_state = rand_6;
			break;
		case rand_9:
			if(C5)
				rand_state = rand_11;
			else if(C0)
				rand_state = rand_1;
			else if(dir == down)
				rand_state = rand_15;
			else if(B1)
				rand_state = rand_17;
			else if(C7)
				rand_state = rand_8;
			else if(D7)
				rand_state = rand_2;
			else if(C6)
				rand_state = rand_13;
			else if(C3)
				rand_state = rand_5;
			else if(C5)
				rand_state = rand_9;
			else if(B0)
				rand_state = rand_10;
			break;
		case rand_10:
			if(C4)
				rand_state = rand_12;
			else if(B3)
				rand_state = rand_5;
			else if(C6)
				rand_state = rand_7;
			else if(dir == left)
				rand_state = rand_1;
			else if(C0)
				rand_state = rand_3;
			else if(C2)
				rand_state = rand_15;
			else if(C5)
				rand_state = rand_8;
			else if(D6)
				rand_state = rand_16;
			else if(dir == right)
				rand_state = rand_11;
			else if(C7)
				rand_state = rand_9;
			break;
		case rand_11:
			if(C0)
				rand_state = rand_3;
			else if(B1)
				rand_state = rand_7;
			else if(D7)
				rand_state = rand_15;
			else if(B0)
				rand_state = rand_2;
			else if(C7)
				rand_state = rand_6;
			else if(dir == up)
				rand_state = rand_10;
			else if(C1)
				rand_state = rand_8;
			else if(C4)
				rand_state = rand_1;
			else if(B3)
				rand_state = rand_5;
			else if(C5)
				rand_state = rand_12;
			break;
		case rand_12:
			if(B3)
				rand_state = rand_1;
			else if(D6)
				rand_state = rand_8;
			else if(dir == left)
				rand_state = rand_11;
			else if(C4)
				rand_state = rand_5;
			else if(B0)
				rand_state = rand_4;
			else if(C3)
				rand_state = rand_15;
			else if(C6)
				rand_state = rand_17;
			else if(D7)
				rand_state = rand_2;
			else if(dir == up)
				rand_state = rand_7;
			else if(C2)
				rand_state = rand_10;
			break;
		case rand_13:
			if(C4)
				rand_state = rand_3;
			else if(C0)
				rand_state = rand_13;
			else if(dir == down)
				rand_state = rand_2;
			else if(C2)
				rand_state = rand_4;
			else if(B3)
				rand_state = rand_8;
			else if(D6)
				rand_state = rand_11;
			else if(D7)
				rand_state = rand_3;
			else if(dir == right)
				rand_state = rand_7;
			else if(C4)
				rand_state = rand_6;
			else if(C7)
				rand_state = rand_5;
			break;
		case rand_14:
			if(C0)
				rand_state = rand_6;
			else if(C3)
				rand_state = rand_2;
			else if(dir == up)
				rand_state = rand_13;
			else if(C4)
				rand_state = rand_12;
			else if(B2)
				rand_state = rand_8;
			else if(D7)
				rand_state = rand_9;
			else if(dir == down)
				rand_state = rand_3;
			else if(B2)
				rand_state = rand_16;
			else if(C3)
				rand_state = rand_4;
			else if(C6)
				rand_state = rand_15;
			break;
		case rand_15:
			if(B3)
				rand_state = rand_2;
			else if(C6)
				rand_state = rand_15;
			else if(C1)
				rand_state = rand_9;
			else if(C4)
				rand_state = rand_5;
			else if(C7)
				rand_state = rand_7;
			else if(dir == down)
				rand_state = rand_9;
			else if(dir == up)
				rand_state = rand_12;
			else if(D6)
				rand_state = rand_17;
			else if(C0)
				rand_state = rand_16;
			else if(C3)
				rand_state = rand_4;
			break;
		case rand_16:
			if(C1)
				rand_state = rand_7;
			else if(dir == up)
				rand_state = rand_14;
			else if(B0)
				rand_state = rand_17;
			else if(C2)
				rand_state = rand_7;
			else if(C6)
				rand_state = rand_8;
			else if(D6)
				rand_state = rand_12;
			else if(C5)
				rand_state = rand_1;
			else if(dir == left)
				rand_state = rand_3;
			else if(dir == mid)
				rand_state = rand_9;
			else if(C7)
				rand_state = rand_13;
			break;
		case rand_17:
			if(B0)
				rand_state = rand_3;
			else if(dir == down)
				rand_state = rand_7;
			else if(C2)
				rand_state = rand_12;
			else if(C7)
				rand_state = rand_11;
			else if(B2)
				rand_state = rand_10;
			else if(dir == up)
				rand_state = rand_2;
			else if(C6)
				rand_state = rand_1;
			else if(B1)
				rand_state = rand_9;
			else if(D7)
				rand_state = rand_8;
			else if(C1)
				rand_state = rand_16;
			break;
		default: rand_state = rand_start; break;
	}
	switch(rand_state){
		case rand_start:
			break;
		case rand_1:
			rd = 0;
			break;
		case rand_2:
			rd = 1;
			break;
		case rand_3:
			rd = 2;
			break;
		case rand_4:
			rd = 3;
			break;
		case rand_5:
			rd = 0;
			break;
		case rand_6:
			rd = 1;
			break;
		case rand_7:
			rd = 2;
			break;
		case rand_8:
			rd = 3;
			break;
		case rand_9:
			rd = 0;
			break;
		case rand_10:
			rd = 1;
			break;
		case rand_11:
			rd = 2;
			break;
		case rand_12:
			rd = 3;
			break;
		case rand_13:
			rd = 0;
			break;
		case rand_14:
			rd = 1;
			break;
		case rand_15:
			rd = 2;
			break;
		case rand_16:
			rd = 3;
			break;
		case rand_17:
			rd = rand() % 4;
		default: break;
	}
	return rand_state;
}


// game logic and LEDs, speaker, LED display output
enum game_states {game_start, game_wait, game_add_light, game_seq, game_delay, game_check, game_win} game_state;
int game_Tick(){
	unsigned char START_out[] = "Simon Says: UP  TO START/HIGH:  ";
	const unsigned char WIN_out[] = "YOU WIN!        DOWN TO RESTART";

	const unsigned char highScore = getHighScore();
	static unsigned char t; // for showing sequence for a while
	static unsigned char idx, seq_l; // idx for traversing sequence, seq_l determine length
	switch(game_state){
		case game_start:
			game_state = game_wait; idx = 0; seq_l = 0; t = 0; score = 0; 
			LCD_DisplayString(1, START_out); LCD_Cursor(31); LCD_WriteData(highScore/10 + '0'); LCD_Cursor(32); LCD_WriteData(highScore%10 + '0'); 
			break;
		case game_wait:
			if(dir != up)
				game_state = game_wait;
			else {
				game_state = game_add_light;
			}
			break;
		case game_add_light:
			if(seq_l > MAX_SEQ) {
				game_state = game_win; 
				LCD_DisplayString(1, WIN_out);

				updateHighscore();
			} else {
				game_state = game_seq;
				LCD_DisplayString(1, "Sequence:");
			}
			break;
		case game_seq: // wait for the sequence to finish showing
			if(idx == seq_l){
				game_state = game_delay; // player should mimic now
				idx = 0;
				PORTB = PINB & 0xF0;
			}
			else{
				game_state = game_seq;
			}
			break;
		case game_delay:
			if(t >= 5){
				LCD_DisplayString(1, "Checking Match:");
				game_state = game_check;
				t = 0;
			} else{
				t++;
			}
		break;
		case game_check: // now check for matching
			PORTB = PORTB & 0xF0; // have led only show for a tick
			if(idx == seq_l && idx == 0){ // when incorrect reset game
				game_state = game_start;
			} else if(idx == seq_l){
				game_state = game_add_light; // when correct get new sequence
				score++; // every sequence correct increases score
			}
			else{ // otherwise keep checking
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
		case game_add_light:
			if(++seq_l <= MAX_SEQ) {
				idx = 0;
				joy_seq[seq_l - 1] = rd; // from rand_tick		
                                led_seq[seq_l - 1] = round(pow(2,rd)); // order of enumeration allows this to work
			}
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
				idx++;
			}
			else if(dir != mid){
				idx = 0;
				seq_l = 0; // to initiate restart sequence
				LCD_DisplayString_NoClear(17, "INCORRECT!");
				PORTB = PINB & 0xF0;
				// important to update highscore before resetting score :)
				updateHighscore(); 
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

	LCD_Custom_Char(0, right_smile);
	LCD_Custom_Char(1, sword);
	LCD_Custom_Char(2, amongE);
	LCD_Custom_Char(3, vent);
    // Insert your solution below 

    static task task1, task2, task3;
    task *tasks[] = { &task1, &task2, &task3 };
    const unsigned short numTasks = sizeof(tasks) / sizeof(task*);
    const char start = 0;

    task1.state = start;
    task1.period = 500;
    task1.elapsedTime = task1.period;
    task1.TickFct = &joy_Tick;

    task2.state = start;
    task2.period = 100;
    task2.elapsedTime = task2.period;
    task2.TickFct = &rand_Tick;

    task3.state = start;
    task3.period = 500;
    task3.elapsedTime = task3.period;
    task3.TickFct = &game_Tick;

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
