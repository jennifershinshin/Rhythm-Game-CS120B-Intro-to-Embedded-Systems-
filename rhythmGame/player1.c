#ifndef PLAYER1_C
#define PLAYER1_C

#include <avr/io.h>
#include <ucr/timer.h>

#include "bitAccessFunctions.c"
#include "updateInputFunctions.c"


static unsigned char column_val_Player = 0x80; // sets the pattern displayed on columns
static unsigned char column_sel_Player = 0x7F; // grounds column to display pattern


//ADC Joystick code-----------------------------------------------------
//taken from http://maxembedded.com/2011/06/the-adc-of-the-avr/
void adc_init()
{
	// AREF = AVcc
	ADMUX = (1<<REFS0);
	
	// ADC Enable and prescaler of 128
	// 16000000/128 = 125000
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

//The following code segment reads the value of the ADC.
//Always refer to the register description above for every line of code.
uint16_t adc_read(uint8_t ch)
{
	// select the corresponding channel 0~7
	// ANDing with ’7? will always keep the value
	// of ‘ch’ between 0 and 7
	ch &= 0b00000111;  // AND operation with 7
	ADMUX = (ADMUX & 0xF8)|ch; // clears the bottom 3 bits before ORing
	
	// start single convertion
	// write ’1? to ADSC
	ADCSRA |= (1<<ADSC);
	
	// wait for conversion to complete
	// ADSC becomes ’0? again
	// till then, run loop continuously
	while(ADCSRA & (1<<ADSC));
	
	return (ADC);
}
//--------------------------------------------------------------------

void targetAcquired(unsigned char targetSeq_val_arr[], unsigned char targetSeq_sel_arr[], int arrSize, unsigned char button) {
	for(int m = 0; m < arrSize; ++m) { //compare bits of player place and targets & change
		if( (column_val_Player == targetSeq_val_arr[m]) && button) {
			//FIND BIT PLACE where column_sel_player val is 0 and compare that place to seq1_sel
			for(int n = 0; n < 8; ++n) {
				if((GetBit(column_sel_Player, n) == 0) && (GetBit(targetSeq_sel_arr[m], n) == 0)) {
					//make target disappear if pressed on
					targetSeq_sel_arr[m] = SetBit(targetSeq_sel_arr[m], n, 1);
					if(score <= 50) {
						score++;
					}
				}
			}
		}
	}
};

int powerUpAcquired(unsigned char targetSeq_val_arr[], unsigned char targetSeq_sel_arr[], int arrSize, unsigned char button) {
	for(int m = 0; m < arrSize; ++m) { //compare bits of player place and targets & change
		if( (column_val_Player == targetSeq_val_arr[m]) && button) {
			//FIND BIT PLACE where column_sel_player val is 0 and compare that place to seq1_sel
			for(int n = 0; n < 8; ++n) {
				if((GetBit(column_sel_Player, n) == 0) && (GetBit(targetSeq_sel_arr[m], n) == 0)) {
					//make target disappear if pressed on
					targetSeq_sel_arr[m] = SetBit(targetSeq_sel_arr[m], n, 1);
					return 1;
				}
				else {
					return 0;
				}
			}
		}
	}
};


unsigned char readVert() {
	unsigned char light = 0x00;
	uint16_t joyVertTemp = adc_read(1);
	if(joyVertTemp >= 800) {		//up
		light = 0x01;
	}
	else if(joyVertTemp <= 100) {
		light = 0x02;
	}
	return light;
}

unsigned char readHoriz() {
	unsigned char light = 0x00;
	uint16_t joyHorizTemp = adc_read(0);
	if(joyHorizTemp >= 800) {		//up
		light = 0x04;
	}
	else if(joyHorizTemp <= 100) {
		light = 0x08;
	}
	return light;
}

enum joyStates {playerMovementStart, wait, up, down, left, right};
int playerMovement(int state)
{
	adc_init();
	
	// === Local Variables ===
	//static unsigned char column_val_Player = 0x80; // sets the pattern displayed on columns
	//static unsigned char column_sel_Player = 0x7F; // grounds column to display pattern
	uint16_t joyVert;
	uint16_t joyHoriz;
	
	switch(state) {
		case playerMovementStart:
			state = wait;
			break;
		case wait:
			joyVert = readVert();
			joyHoriz = readHoriz();
			
			if(joyVert == 0x01) {
				state = left;
			}
			else if(joyVert == 0x02) {
				state = right;
			}
			else if(joyHoriz == 0x04) {
				state = up;
			}
			else if(joyHoriz == 0x08) {
				state = down;
			}
			else {
				state = wait;
			}
			break;
		case up:
			state = wait;
			break;
		case down:
			state = wait;
			break;
		case left:
			state = wait;
			break;
		case right:
			state = wait;
			break;
		default:
			break;
	}
	
	switch(state) {
		case playerMovementStart:
			//column_sel_Player = 0x7F; // display far left column
			//column_val_Player = 0x80; // pattern illuminates bot row
			//transmit_data_PORTB(column_val_Player);
			//transmit_data_PORTDRed(column_sel_Player);
			break;
		case wait:
			transmit_data_PORTB(column_val_Player);
			//transmit_data_PORTDRed(column_sel_Player);
			PORTD = column_sel_Player;
			break;
		case up:
			// if at the very top
			if (column_val_Player != 0x01) {
				column_val_Player = column_val_Player >> 1; // shift up illuminated LED one row
			}
			transmit_data_PORTB(column_val_Player);
			//transmit_data_PORTDRed(column_sel_Player);
			PORTD = column_sel_Player;
			break;
		case down:
			// if at the very bottom
			if (column_val_Player != 0x80) {
				column_val_Player = column_val_Player << 1; // shift down illuminated LED one row
			}
			transmit_data_PORTB(column_val_Player);
			//transmit_data_PORTDRed(column_sel_Player);
			PORTD = column_sel_Player;
			break;
		case left:
			// if far left column was last to display (grounded)
			if (column_sel_Player != 0x7F) {
				//Shift displayed column one to the left
				column_sel_Player = (column_sel_Player << 1) | 0x01;
			}
			transmit_data_PORTB(column_val_Player);
			//transmit_data_PORTDRed(column_sel_Player);
			PORTD = column_sel_Player;
			break;
		case right:
			//if far right column was last to display (grounded)
			if (column_sel_Player != 0xFE) {
				//Shift displayed column one to the right
				column_sel_Player = (column_sel_Player >> 1) | 0x80;
			}
			transmit_data_PORTB(column_val_Player);
			//transmit_data_PORTDRed(column_sel_Player);
			PORTD = column_sel_Player;
			break;
		default:
			break;
	}
	
	return state;
}

#endif