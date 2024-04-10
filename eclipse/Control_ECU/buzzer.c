/******************************************************************************
 *
 * Module:BUZZER
 *
 * File Name: buzzer.c
 *
 * Description: Source file for the BUZZER driver
 *
 * Author: MARCO
 *
 *******************************************************************************/

#include "gpio.h"
#include "buzzer.h"
#include "avr/io.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 *  Description:
 *  Setup the direction for the buzzer pin as output pin
 *  Turn off the buzzer in the beginning
 */
void Buzzer_init(void)
{
	GPIO_setupPinDirection(BUZZER_PORT, BUZZER_PIN,PIN_OUTPUT);
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_LOW);
}
/*
 * Description:
 * Function to enable the Buzzer
 */
void Buzzer_on(void)
{
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_HIGH);
}

/*
 * Description:
 * Function to disable the Buzzer
 */
void Buzzer_off(void)
{
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_LOW);
}
