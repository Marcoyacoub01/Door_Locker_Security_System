
/*
 * Module: dcMotor
 *
 * File Name: dcMotor.c
 *
 * Description: source file for the AVR dcMotor driver
 *
 * Author: MARCO
 */

#include "dcMotor.h"
#include "pwm.h"
#include "gpio.h"




void DcMotor_Init(void)
{
	GPIO_setupPinDirection(DC_MOTOR_PORT_ID,DC_MOTOR_PIN1_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(DC_MOTOR_PORT_ID,DC_MOTOR_PIN2_ID, PIN_OUTPUT);

	GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN1_ID,LOGIC_LOW);
	GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN2_ID,LOGIC_LOW);
}


void DcMotor_Rotate(DcMotor_State state,uint8 speed)
{
	if (state == STOP)
	{
		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN1_ID,LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN2_ID,LOGIC_LOW);
	}
	else if ( state == CW )
	{
		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN1_ID,LOGIC_HIGH);
		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN2_ID,LOGIC_LOW);
	}
	else if (state == A_CW)
	{
		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN1_ID,LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN2_ID,LOGIC_HIGH);
	}


	/*speed Controlling*/
	PWM_Timer0_Start( speed);
}


