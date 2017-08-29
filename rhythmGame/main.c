#include <avr/io.h>
#include <ucr/timer.h>

int score = 0;

#include "player1.c"
#include "player2.c"
#include "targetSequences.c"


//--------Find GCD function (CODE TAKEN FROM CS120B LAB MANUAL)--------------------------------------------------
unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
	unsigned long int c;
	while(1){
		c = a%b;
		if(c==0){return b;}
		a = b;
b = c;
	}
	return 0;
}
//--------End find GCD function ----------------------------------------------
 
//--------Task scheduler data structure (CODE TAKEN FROM CS120B LAB MANUAL)---------------------------------------
// Struct for Tasks represent a running process in our simple real-time operating system.
typedef struct _task {
	/*Tasks should have members that include: state, period,
		a measurement of elapsed time, and a function pointer.*/
	signed char state; //Task's current state
	unsigned long int period; //Task period
	unsigned long int elapsedTime; //Time elapsed since last task tick
	int (*TickFct)(int); //Task tick function
} task;
 
//--------End Task scheduler data structure-----------------------------------



int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;		//porta = input
    DDRB = 0xFF; PORTB = 0x00;		//portb = output
    DDRC = 0xFF; PORTC = 0x00;		//portc = output
	DDRD = 0xFF; PORTD = 0x00;
	
	// Period for the tasks
	unsigned long int playerMovement_calc = 25;
	unsigned long int player2Movement_calc = 75;
	unsigned long int targetLights_calc = 1;
	//unsigned long int SMTick3_calc = 1000;
	//unsigned long int SMTick4_calc = 10;
	
	//Calculating GCD
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(playerMovement_calc, player2Movement_calc);
	tmpGCD = findGCD(tmpGCD, targetLights_calc);
	
	//Greatest common divisor for all tasks or smallest time unit for tasks.
	unsigned long int GCD = tmpGCD;
	
	//Recalculate GCD periods for scheduler
	unsigned long int playerMovement_period = playerMovement_calc/GCD;
	unsigned long int player2Movement_period = player2Movement_calc/GCD;
	unsigned long int targetLights_period = targetLights_calc/GCD;
	
	//Declare an array of tasks 
	static task task1, task2, task3;//, task4;
	task *tasks[] = { &task1, &task2, &task3};//, &task3, &task4 };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	 //Task 1
	 task1.state = playerMovementStart;//Task initial state.
	 task1.period = playerMovement_period;//Task Period.
	 task1.elapsedTime = playerMovement_period;//Task current elapsed time.
	 task1.TickFct = &playerMovement;//Function pointer for the tick.
	 
	 //Task 2
	 task2.state = player2MovementStart;//Task initial state.
	 task2.period = player2Movement_period;//Task Period.
	 task2.elapsedTime = player2Movement_period;//Task current elapsed time.
	 task2.TickFct = &player2Movement;//Function pointer for the tick.
	 
	 //Task 3
	 task3.state = targetStart;//Task initial state.
	 task3.period = targetLights_period;//Task Period.
	 task3.elapsedTime = targetLights_period;//Task current elapsed time.
	 task3.TickFct = &targetLights;//Function pointer for the tick.
	
	// Set the timer and turn it on
	adc_init();
	TimerSet(GCD);
	TimerOn();

	
	unsigned short i;
    while(1) {
		// Scheduler code
		for ( i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset the elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
		
	}

	// Error: Program should not exit!
	return 0;
}

