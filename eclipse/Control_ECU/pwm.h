/*
 * Module: TIMER0_PWM
 *
 * File Name: pwm.h
 *
 * Description: Header file for the AVR TIMER0_PWM driver
 *
 * Author: MARCO
 */

#ifndef PWM_H_
#define PWM_H_

#include "std_types.h"


/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define PWM_TIMER0_PORT_ID                PORTB_ID
#define PWM_TIMER0_PIN_ID                 PIN3_ID




/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/




void PWM_Timer0_Start(uint8 duty_cycle);





#endif /* PWM_H_ */
