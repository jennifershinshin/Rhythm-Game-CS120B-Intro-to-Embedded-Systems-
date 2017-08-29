/*
 * custom2.0(moveOnMatrix).c
 *
 * Created: 6/1/2017 1:09:19 AM
 * Author : Jennifer
 */ 

#ifndef PLAYER2_C
#define PLAYER2_C

#include <avr/io.h>
#include <ucr/timer.h>

#include "bitAccessFunctions.c"
#include "updateInputFunctions.c"


static unsigned char column_val_Player2 = 0xC0; // sets the pattern displayed on columns
static unsigned char column_sel_Player2 = 0xFC; // grounds column to display pattern

static int p2On = 0;



void p2TargetErase(unsigned char targetSeq_val_arr[], unsigned char targetSeq_sel_arr[], int arrSize) {
	for(int m = 0; m < arrSize; ++m) { //compare bits of player place and targets & change
		for(int z = 0; z < 8; ++z) {
			if( (GetBit(column_val_Player2, z) == 1) && (GetBit(targetSeq_val_arr[m], z) == 1) ) {
				//FIND BIT PLACE where column_sel_player val is 0 and compare that place to seq1_sel
				for(int y = 0; y < 8; ++y) {
					if((GetBit(column_sel_Player2, y) == 0) && (GetBit(targetSeq_sel_arr[m], y) == 0)) {
						//make target disappear if pressed on
						targetSeq_sel_arr[m] = SetBit(targetSeq_sel_arr[m], y, 1);
					}
				}
			}
		}
	}
};

enum player2MovementStates {player2MovementStart, p2Join_state, p2wait_state, p2Up_state, p2Down_state, p2Right_state, p2Left_state};
int player2Movement(int state)
{
	unsigned char p2Up_button = ~PINA & 0x10;
	unsigned char p2Down_button = ~PINA & 0x20;
	unsigned char p2Right_button = ~PINA & 0x40;
	unsigned char p2Left_button = ~PINA & 0x80;
	
	switch(state) {
		case player2MovementStart:
			state = p2Join_state;
			break;
		case p2Join_state:
			if(p2Up_button || p2Down_button || p2Right_button || p2Left_button) {
				state = p2wait_state;
			}
			else {
				state = p2Join_state;
			}
			break;
		case p2wait_state:
			if(p2Up_button) {
				state = p2Up_state;
			}
			else if(p2Down_button) {
				state = p2Down_state;
			}
			else if(p2Right_button) {
				state = p2Right_state;
			}
			else if(p2Left_button) {
				state = p2Left_state;
			}
			else {
				state = p2wait_state;
			}
			break;
		case p2Up_state:
			state = p2wait_state;
			break;
		case p2Down_state:
			state = p2wait_state;
			break;
		case p2Left_state:
			state = p2wait_state;
			break;
		case p2Right_state:
			state = p2wait_state;
			break;
		default:
			break;
	}
	
	switch(state) {
		case player2MovementStart:
			//column_sel_Player2 = 0x7F; // display far left column
			//column_val_Player2 = 0x80; // pattern illuminates bot row
			//transmit_data_PORTB(column_val_Player2);
			//transmit_data_PORTDRed(column_sel_Player2);
			//p2On = 0;
			break;
		case p2Join_state:
			break;
		case p2wait_state:
			transmit_data_PORTB(column_val_Player2);
			//transmit_data_PORTDRed(column_sel_Player2);
			PORTD = column_sel_Player2;
			p2On = 1;
			break;
		case p2Up_state:
			// if at the very top
			if (column_val_Player2 != 0x03) {
				column_val_Player2 = column_val_Player2 >> 1; // shift up illuminated LED one row
			}
			transmit_data_PORTB(column_val_Player2);
			//transmit_data_PORTDRed(column_sel_Player2);
			PORTD = column_sel_Player2;
			p2On = 1;
			break;
		case p2Down_state:
			// if at the very bottom
			if (column_val_Player2 != 0xC0) {
				column_val_Player2 = column_val_Player2 << 1; // shift down illuminated LED one row
			}
			transmit_data_PORTB(column_val_Player2);
			//transmit_data_PORTDRed(column_sel_Player2);
			PORTD = column_sel_Player2;
			p2On = 1;
			break;
		case p2Left_state:
			// if far left column was last to display (grounded)
			if (column_sel_Player2 != 0x3F) {
				//Shift displayed column one to the left
				column_sel_Player2 = (column_sel_Player2 << 1) | 0x01;
			}
			transmit_data_PORTB(column_val_Player2);
			//transmit_data_PORTDRed(column_sel_Player2);
			PORTD = column_sel_Player2;
			p2On = 1;
			break;
		case p2Right_state:
			//if far right column was last to display (grounded)
			if (column_sel_Player2 != 0xFC) {
				//Shift displayed column one to the right
				column_sel_Player2 = (column_sel_Player2 >> 1) | 0x80;
			}
			transmit_data_PORTB(column_val_Player2);
			//transmit_data_PORTDRed(column_sel_Player2);
			PORTD = column_sel_Player2;
			p2On = 1;
			break;
		default:
			break;
	}
	
	return state;
}

#endif