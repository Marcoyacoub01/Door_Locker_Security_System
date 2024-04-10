/******************************************************************************
 *
 * Module: BUZZER
 *
 * File Name: buzzer.h
 *
 * Description: Header file for the BUZZER driver
 *
 * Author: Marco
 *
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define BUZZER_PORT   PORTD_ID
#define BUZZER_PIN    PIN3_ID

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 *  Description:
 *  Setup the direction for the buzzer pin as output pin
 *  Turn off the buzzer in the beginning
 */
void Buzzer_init(void);
/*
 * Description:
 * Function to enable the Buzzer
 */
void Buzzer_on(void);
/*
 * Description:
 * Function to disable the Buzzer
 */
void Buzzer_off(void);

#endif /* BUZZER_H_ */
