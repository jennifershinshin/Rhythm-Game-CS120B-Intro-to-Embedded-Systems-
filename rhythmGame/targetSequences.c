#ifndef TARGETSEQUENCES_C
#define TARGETSEQUENCES_C

#include <avr/io.h>
#include <ucr/timer.h>

#include "bitAccessFunctions.c"
#include "updateInputFunctions.c"


const int seq1_arrSize = 1;
const int seq2_arrSize = 5;
const int seq3_arrSize = 2;
const int seq4_arrSize = 2;
const int seq5_arrSize = 3;
const int seq6_arrSize = 3;
const int seq7_arrSize = 8;
const int seq8_arrSize = 7;

//Array resets-------------------------------------------
static unsigned char seq1_sel_reset[1] = { 0x50 };		//(column) selects column
static unsigned char seq1_val_reset[1] = { 0x04 };		//(row) pattern displayed on column

static unsigned char seq2_sel_reset[5] = { 0xFE, 0xFE, 0xFE, 0xFE, 0xF5 };		//(column) selects column
static unsigned char seq2_val_reset[5] = { 0x01, 0x02, 0x04, 0x10, 0x20 };		//(row) pattern displayed on column

static unsigned char seq3_sel_reset[2] = { 0xAF, 0x0F };
static unsigned char seq3_val_reset[2] = { 0x01, 0x04 };
	
static unsigned char seq4_sel_reset[2] = { 0x7F, 0x51 };
static unsigned char seq4_val_reset[2] = { 0x40, 0x20 };

static unsigned char seq5_sel_reset[3] = { 0xFA, 0xFB, 0xFA };
static unsigned char seq5_val_reset[3] = { 0x20, 0x40, 0x80 };

static unsigned char seq6_sel_reset[3] = { 0xDF, 0xEF, 0xF7 };
static unsigned char seq6_val_reset[3] = { 0x20, 0x40, 0x80 };

static unsigned char seq7a_sel_reset[8] = { 0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE };
static unsigned char seq7a_val_reset[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

static unsigned char seq8a_sel_reset[7] = { 0x7F, 0xBF, 0xDF, 0xEF, 0xF3, 0xFD, 0xFE };
static unsigned char seq8a_val_reset[7] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02 };

static unsigned char seq7b_sel_reset[8] = { 0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F };
static unsigned char seq7b_val_reset[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

static unsigned char seq8b_sel_reset[7] = { 0xFE, 0xFD, 0xFB, 0xF7, 0xCF, 0xBF, 0x7F };
static unsigned char seq8b_val_reset[7] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02 };
//-------------------------------------------------------

static unsigned char sequence1_sel_arr[1] = { 0x50 };		//(column) selects column
static unsigned char sequence1_val_arr[1] = { 0x04 };		//(row) pattern displayed on column

static unsigned char sequence2_sel_arr[5] = { 0xFE, 0xFE, 0xFE, 0xFE, 0xF5 };		//(column) selects column
static unsigned char sequence2_val_arr[5] = { 0x01, 0x02, 0x04, 0x10, 0x20 };		//(row) pattern displayed on column

static unsigned char sequence3_sel_arr[2] = { 0xAF, 0x0F };
static unsigned char sequence3_val_arr[2] = { 0x01, 0x04 };
	
static unsigned char sequence4_sel_arr[2] = { 0x7F, 0x51 };
static unsigned char sequence4_val_arr[2] = { 0x40, 0x20 };
	
static unsigned char sequence5_sel_arr[3] = { 0xFA, 0xFB, 0xFA };
static unsigned char sequence5_val_arr[3] = { 0x20, 0x40, 0x80 };
	
static unsigned char sequence6_sel_arr[3] = { 0xDF, 0xEF, 0xF7 };
static unsigned char sequence6_val_arr[3] = { 0x20, 0x40, 0x80 };
	
static unsigned char sequence7a_sel_arr[8] = { 0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE };
static unsigned char sequence7a_val_arr[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

static unsigned char sequence8a_sel_arr[7] = { 0x7F, 0xBF, 0xDF, 0xEF, 0xF3, 0xFD, 0xFE };
static unsigned char sequence8a_val_arr[7] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02 };

static unsigned char sequence7b_sel_arr[8] = { 0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F };
static unsigned char sequence7b_val_arr[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

static unsigned char sequence8b_sel_arr[7] = { 0xFE, 0xFD, 0xFB, 0xF7, 0xCF, 0xBF, 0x7F };
static unsigned char sequence8b_val_arr[7] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02 };



int timeCount = 0;
int firstHalfOfSongCompleted = 0;	//flag

static unsigned char column_val = 0x00; //nothing illuminated
static unsigned char column_sel = 0xFF; //nothing illuminated

static unsigned char powerUp_val[1] = { 0x04 };
static unsigned char powerUp_sel[1] = { 0xFE };
static unsigned char powerUp_arrSize = 1;
	
static int powerUp_slowTime1000 = 0;		//initialized to 0, then change to value when powerUpAcquired
static int powerUp_slowTime2000 = 0;


void initiatePowerUp() {		//power up slows down time
	powerUp_slowTime1000 = 1000;
	powerUp_slowTime2000 = 2000;
};

void resetTargetSeqArrays() {
	memcpy(sequence1_sel_arr, seq1_sel_reset, sizeof(sequence1_sel_arr));
	memcpy(sequence1_val_arr, seq1_val_reset, sizeof(sequence1_val_arr));
	
	memcpy(sequence2_sel_arr, seq2_sel_reset, sizeof(sequence2_sel_arr));
	memcpy(sequence2_val_arr, seq2_val_reset, sizeof(sequence2_val_arr));
	
	memcpy(sequence3_sel_arr, seq3_sel_reset, sizeof(sequence3_sel_arr));
	memcpy(sequence3_val_arr, seq3_val_reset, sizeof(sequence3_val_arr));
	
	memcpy(sequence4_sel_arr, seq4_sel_reset, sizeof(sequence4_sel_arr));
	memcpy(sequence4_val_arr, seq4_val_reset, sizeof(sequence4_val_arr));
	
	memcpy(sequence5_sel_arr, seq5_sel_reset, sizeof(sequence5_sel_arr));
	memcpy(sequence5_val_arr, seq5_val_reset, sizeof(sequence5_val_arr));
	
	memcpy(sequence6_sel_arr, seq6_sel_reset, sizeof(sequence6_sel_arr));
	memcpy(sequence6_val_arr, seq6_val_reset, sizeof(sequence6_val_arr));
	
	memcpy(sequence7a_sel_arr, seq7a_sel_reset, sizeof(sequence7a_sel_arr));
	memcpy(sequence7a_val_arr, seq7a_val_reset, sizeof(sequence7a_val_arr));
	
	memcpy(sequence8a_sel_arr, seq8a_sel_reset, sizeof(sequence8a_sel_arr));
	memcpy(sequence8a_val_arr, seq8a_val_reset, sizeof(sequence8a_val_arr));
	
	memcpy(sequence7b_sel_arr, seq7b_sel_reset, sizeof(sequence7b_sel_arr));
	memcpy(sequence7b_val_arr, seq7b_val_reset, sizeof(sequence7b_val_arr));
	
	memcpy(sequence8b_sel_arr, seq8b_sel_reset, sizeof(sequence8b_sel_arr));
	memcpy(sequence8b_val_arr, seq8b_val_reset, sizeof(sequence8b_val_arr));
};


enum targetLightsStates {targetStart, t1, t2, wait1, t3, t4, wait2, t5, t6,
							t7a, t8a, t7b, t8b, wait3, secondHalfReset};
int targetLights(int state)
{
	// === Local Variables ===
	//static unsigned char column_val = 0x00; //nothing illuminated
	//static unsigned char column_sel = 0xFF; //nothing illuminated
	unsigned char startButton = ~PINA & 0x08;
	unsigned char scoreButton = ~PINA & 0x04;
	
	switch(state) {
		case targetStart:
			if(startButton) {
				state = t1;
			}
			else {
				state = targetStart;
			}
			break;
		case t1:
			if(timeCount < 1000) {
				state = t1;
			}
			else {
				state = t2;
			}
			break;
		case t2:
			if(timeCount - powerUp_slowTime1000 < 2000) {
				state = t2;
			}
			else {
				state = wait1;
			}
			break;
		case wait1:
			if(timeCount - powerUp_slowTime1000 < 2500) {
				state = wait1;
			}
			else {
				state = t3;
			}
			break;
		case t3:
			if(timeCount - powerUp_slowTime2000 < 3500) {
				state = t3;
			}
			else {
				state = t4;
			}
			break;
		case t4:
			if(timeCount < 4500) {
				state = t4;
			}
			else {
				state = wait2;
			}
			break;
		case wait2:
			if(timeCount < 5500) {
				state = wait2;
			}
			else {
				state = t5;
			}
			break;
		case t5:
			if(timeCount < 6500) {
				state = t5;
			}
			else {
				state = t6;
			}
			break;
		case t6:
			if(timeCount < 7000) {
				state = t6;
			}
			else {
				if(!firstHalfOfSongCompleted) {
					state = t7a;
				}
				else {
					state = t7b;
				}
			}
			break;
		case t7a:
			if(timeCount < 8500) {
				state = t7a;
			}
			else {
				state = t8a;
			}
			break;
		case t8a:
			if(timeCount < 10000) {
				state = t8a;
			}
			else {
				state = wait3;
			}
			break;
		case t7b:
			if(timeCount < 8500) {
				state = t7b;
			}
			else {
				state = t8b;
			}
			break;
		case t8b:
			if(timeCount < 10000) {
				state = t8b;
			}
			else {
				state = wait3;
			}
			break;
		case wait3:
			if(timeCount < 11000) {
				state = wait3;
			}
			else {
				if(!firstHalfOfSongCompleted) {
					state = secondHalfReset;
				}
				else {
					state = targetStart;
				}
			}
			break;
		case secondHalfReset:
			state = t1;
			break;
		default:
			state = targetStart;
			break;
	}
	
	switch(state) {
		case targetStart:
			resetTargetSeqArrays();
			timeCount = 0;
			score = 0;
			powerUp_slowTime1000 = 0;
			powerUp_slowTime2000 = 0;
			firstHalfOfSongCompleted = 0;
			column_val = 0x00;
			column_sel = 0xFF;
			transmit_data_PORTB(column_val);
			//transmit_data_PORTDRed(column_sel);
			PORTD = column_sel;
			//PORTC = score;
			break;
		case t1:
			timeCount++;
			
			if(firstHalfOfSongCompleted && powerUpAcquired(powerUp_val, powerUp_sel, powerUp_arrSize, scoreButton)) {
				initiatePowerUp();
			}
			else {
				targetAcquired(sequence1_val_arr, sequence1_sel_arr, seq1_arrSize, scoreButton);
				if(p2On) {
					p2TargetErase(sequence1_val_arr, sequence1_sel_arr, seq1_arrSize);
				}
			}
			
			for(int i = 0; i < seq1_arrSize; ++i) {		//display target seq1
				transmit_data_PORTB(sequence1_val_arr[i]);
				PORTD = sequence1_sel_arr[i];
			}
			
			PORTC = score;
			break;
		case t2:
			timeCount++;
			targetAcquired(sequence2_val_arr, sequence2_sel_arr, seq2_arrSize, scoreButton);
			if(p2On) {
				p2TargetErase(sequence2_val_arr, sequence2_sel_arr, seq2_arrSize);
			}
			
			for(int i = 0; i < 5; ++i) {
				transmit_data_PORTB(sequence2_val_arr[i]);
				PORTD = sequence2_sel_arr[i];
			}
			PORTC = score;
			break;
		case wait1:
			timeCount++;
			column_val = 0x00;
			column_sel = 0xFF;
			transmit_data_PORTB(column_val);
			//transmit_data_PORTDRed(column_sel);
			PORTD = column_sel;
			PORTC = score;
			break;
		case t3:
			timeCount++;
			targetAcquired(sequence3_val_arr, sequence3_sel_arr, seq3_arrSize, scoreButton);
			if(p2On) {
				p2TargetErase(sequence3_val_arr, sequence3_sel_arr, seq3_arrSize);
			}
			
			for(int i = 0; i < seq3_arrSize; ++i) {
				transmit_data_PORTB(sequence3_val_arr[i]);
				PORTD = sequence3_sel_arr[i];
			}
			PORTC = score;
			break;
		case t4:
			timeCount++;
			targetAcquired(sequence4_val_arr, sequence4_sel_arr, seq4_arrSize, scoreButton);
			if(p2On) {
				p2TargetErase(sequence4_val_arr, sequence4_sel_arr, seq4_arrSize);
			}
			
			for(int i = 0; i < seq4_arrSize; ++i) {
				transmit_data_PORTB(sequence4_val_arr[i]);
				PORTD = sequence4_sel_arr[i];
			}
			PORTC = score;
			break;
		case wait2:
			timeCount++;
			column_val = 0x00;
			column_sel = 0xFF;
			transmit_data_PORTB(column_val);
			//transmit_data_PORTDRed(column_sel);
			PORTD = column_sel;
			PORTC = score;
			break;
		case t5:
			timeCount++;
			targetAcquired(sequence5_val_arr, sequence5_sel_arr, seq5_arrSize, scoreButton);
			if(p2On) {
				p2TargetErase(sequence5_val_arr, sequence5_sel_arr, seq5_arrSize);
			}
			
			for(int i = 0; i < seq5_arrSize; ++i) {
				transmit_data_PORTB(sequence5_val_arr[i]);
				PORTD = sequence5_sel_arr[i];
			}
			PORTC = score;
			break;
		case t6:
			timeCount++;
			targetAcquired(sequence6_val_arr, sequence6_sel_arr, seq6_arrSize, scoreButton);
			if(p2On) {
				p2TargetErase(sequence6_val_arr, sequence6_sel_arr, seq6_arrSize);
			}
			
			for(int i = 0; i < seq6_arrSize; ++i) {
				transmit_data_PORTB(sequence6_val_arr[i]);
				PORTD = sequence6_sel_arr[i];
			}
			PORTC = score;
			break;
		case t7a:
			timeCount++;
			targetAcquired(sequence7a_val_arr, sequence7a_sel_arr, seq7_arrSize, scoreButton);
			if(p2On) {
				p2TargetErase(sequence7a_val_arr, sequence7a_sel_arr, seq7_arrSize);
			}
			
			for(int i = 0; i < seq7_arrSize; ++i) {
				transmit_data_PORTB(sequence7a_val_arr[i]);
				PORTD = sequence7a_sel_arr[i];
			}
			PORTC = score;
			break;
		case t8a:
			timeCount++;
			targetAcquired(sequence8a_val_arr, sequence8a_sel_arr, seq8_arrSize, scoreButton);
			if(p2On) {
				p2TargetErase(sequence8a_val_arr, sequence8a_sel_arr, seq8_arrSize);
			}
			
			for(int i = 0; i < seq8_arrSize; ++i) {
				transmit_data_PORTB(sequence8a_val_arr[i]);
				PORTD = sequence8a_sel_arr[i];
			}
			PORTC = score;
			break;
		case t7b:
			timeCount++;
			targetAcquired(sequence7b_val_arr, sequence7b_sel_arr, seq7_arrSize, scoreButton);
			if(p2On) {
				p2TargetErase(sequence7b_val_arr, sequence7b_sel_arr, seq7_arrSize);
			}
			
			for(int i = 0; i < seq7_arrSize; ++i) {
				transmit_data_PORTB(sequence7b_val_arr[i]);
				PORTD = sequence7b_sel_arr[i];
			}
			PORTC = score;
			break;
		case t8b:
			timeCount++;
			targetAcquired(sequence8b_val_arr, sequence8b_sel_arr, seq8_arrSize, scoreButton);
			if(p2On) {
				p2TargetErase(sequence8b_val_arr, sequence8b_sel_arr, seq8_arrSize);
			}
			
			for(int i = 0; i < seq8_arrSize; ++i) {
				transmit_data_PORTB(sequence8b_val_arr[i]);
				PORTD = sequence8b_sel_arr[i];
			}
			PORTC = score;
			break;
		case wait3:
			timeCount++;
			column_val = 0x00;
			column_sel = 0xFF;
			transmit_data_PORTB(column_val);
			//transmit_data_PORTDRed(column_sel);
			PORTD = column_sel;
			PORTC = score;
			break;
		case secondHalfReset:
			timeCount = 0;
			firstHalfOfSongCompleted = 1;
			resetTargetSeqArrays();
			break;
		default: break;
	}
	return state;
}

#endif