/******************************************************************************
 *
 * Module: TIMER1
 *
 * File Name: timer1.c
 *
 * Description: Source file for the TIMER1 driver
 *
 * Author: MARCO
 *
 *******************************************************************************/
#include "timer1.h"
#include "avr/interrupt.h"
#include <avr/io.h>
#include "gpio.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;




/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}



/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void Timer1_init(const Timer1_ConfigType * Config_Ptr)
{

	// Set initial value
	TCNT1 = (Config_Ptr -> initial_value);


	if((Config_Ptr->mode)==COMPARE)
	{
		//choose CMP mode
		TCCR1B = (TCCR1B ) | (Config_Ptr -> mode);
		//set compare value
		OCR1A = (Config_Ptr -> compare_value);
		/*Force Output Compare for Compare unit A */
		TCCR1A = (1<<FOC1A);
		// Set prescaler
		TCCR1B = (TCCR1B) | (Config_Ptr -> prescaler);


		// Enable compare interrupt
		TIMSK =  (1<<OCIE1A);

	}
	if ((Config_Ptr -> mode) == NORMAL)
	{


		// Set prescaler
		TCCR1B = (TCCR1B) | (Config_Ptr -> prescaler);
		// Enable overflow interrupt
		TIMSK = (1<<TOIE1);

	}



}

void Timer1_deInit(void)
{
	TCCR1A =0 ;
	TCCR1B =0 ;
	TCNT1 = 0 ;
	OCR1A = 0 ;
	TIMSK = 0 ;
	//g_callBackPtr = NULL_PTR ;
}

void Timer1_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr = a_ptr;


}




