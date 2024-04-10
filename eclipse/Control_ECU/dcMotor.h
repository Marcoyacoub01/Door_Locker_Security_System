/*
 * Module: dcMotor
 *
 * File Name: dcMotor.h
 *
 * Description: Header file for the AVR dcMotor driver
 *
 * Author: MARCO
 */

#ifndef DCMOTOR_H_
#define DCMOTOR_H_

#include "std_types.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define DC_MOTOR_PORT_ID                PORTB_ID
#define DC_MOTOR_PIN1_ID                 PIN0_ID
#define DC_MOTOR_PIN2_ID                 PIN1_ID


typedef enum
{
	STOP,CW,A_CW
}DcMotor_State;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

void DcMotor_Init(void);



void DcMotor_Rotate(DcMotor_State state,uint8 speed);

#endif /* DCMOTOR_H_ */
