/*
 * FINAL PROJECT
 *
 * File Name: HMI_ECU.c
 *
 * Author: MARCO
 */

#include <avr/io.h>
#include <util/delay.h>
#include "gpio.h"
#include "Keypad.h"
#include "lcd.h"
#include "timer1.h"
#include "uart.h"


/****************************************************************************************************
 * 												MACROS
 ***************************************************************************************************/
#define READY                  0x10 //used in sync_send() and sync_recive() functions for synchronization

#define MATCHED_PASSWORDS      1    // agreement between the two MCUs after checking the passwords
#define UNMATCHED_PASSWORDS    0    // agreement between the two MCUs after checking the passwords
/*
 * agreement between the two MCUS when we want to create system password
 */
#define CREATE_SYSTEM_PASSWORD 0xFA
/*
 * agreement between the two MCUS when we want to start MENU + or - for open or change passwords
 */
#define MENU                   0xFB
/* this Macros is used for door operation
 * agreement between the two MCUS when the door operation is finished
 */
#define DONE_DOOR_OPERATION    0xFC
/*
 * agreement between the two MCUS when the system is locked
 */
#define LOCKED                 0xFD
/*******************************************************************************************************
 * 										 Functions Definitions
 ******************************************************************************************************/


uint8 seconds = 0 ;
/*timer1 call back function*/
void count_second()
{
	seconds++;
}

/*
 * Description :
 * intializations of the global interrupt , LCD , UART and TIMER1
 */
void initialization(void)
{
	SREG|=(1<<7);
	LCD_init();
	UART_ConfigType uart_configurations = {DATA_8_BIT,DISABLED,ONE_STOP_BIT,9600};
	UART_init(&uart_configurations);
	Timer1_setCallBack(count_second);
	//Timer1_ConfigType timer1_configurations = {0,7812,F_CPU_1024,COMPARE};
	//Timer1_init(&timer1_configurations);
	_delay_ms(300);
}

/*
 * Description :
 * sync the time of send and recive between the UARTs
 */
void sync_send(void)
{
	UART_sendByte(READY);
	while(UART_recieveByte()!=READY);
}

/*
 * Description :
 * sync the time of send and recive between the UARTs
 */
void sync_recive(void)
{
	while(UART_recieveByte()!=READY);
	UART_sendByte(READY);
}

/*
 * Description :
 * Sends a 5-digits password via UART to another microcontroller
 *  after initiating transmission with sync_send().
 *
 * Parameters:
 * uint8 *pass: Pointer to an array containing the password to be sent.
 */
void send_pass(uint8 *pass)
{

	uint8 i;
	sync_send();
	for (i = 0 ; i<5 ; i++)
	{
			UART_sendByte(pass[i]);
	}
}

/*
 * Description:
 * Receives a 5-digits password via UART from another microcontroller
 * after initiating reception with sync_receive().
 * Stores the received password in the specified array.
 *
 * Parameters:
 * uint8 *receive_pass: Pointer to an array where the received password will be stored.
 */
void recive_password(uint8 *recive_pass)
{
	uint8 i ;
	sync_recive();
	for (i = 0 ; i<5 ; i++)
		{
			recive_pass[i] = UART_recieveByte();
		}
}

/*
 * Description:
 * Takes a 5-digit password from a keypad, displays on LCD, waits for '='.
 * Parameters:
 *  uint8 *password: Pointer to array storing the password.
 */
void entering_password(uint8 *password)
{
	uint8 i=0;
	uint8 key;
	while(i<5)
		{
			key = KEYPAD_getPressedKey();
			if ((key >=0) && (key <=9) )
			{
				key = KEYPAD_getPressedKey();
				password[i] = key;
				LCD_displayCharacter('*');
				i++;
				_delay_ms(500); /* Press time */
			}
		}
	while(KEYPAD_getPressedKey() != '=' ){};
}

/*
 * Description:
 *  Initiates timer1 and start count the seconds to display "Unlocking Door" on the screen
 *
 */
void open_door(void)
{
	LCD_clearScreen();
	Timer1_ConfigType timer1_configurations = {0,7812,F_CPU_1024,COMPARE};
	Timer1_init(&timer1_configurations);
	seconds=0;
	LCD_displayString("Unlocking Door");
	while(seconds !=15)
	{
	}
	Timer1_deInit();
}

/*
 * Description:
 *  Initiates timer1 and start count the seconds to display "DOOR IS OPEN" on the screen
 *
 */
void hold_door(void)
{

	LCD_clearScreen();
	Timer1_ConfigType timer1_configurations = {0,7812,F_CPU_1024,COMPARE};
	Timer1_init(&timer1_configurations);
	seconds=0;
	LCD_displayString("DOOR IS OPEN");
	while(seconds !=3)
	{
	}
	Timer1_deInit();
}

/*
 * Description:
 *  Initiates timer1 and start count the seconds to display "Door Is Locking" on the screen
 *
 */

void close_door(void)
{

	LCD_clearScreen();
	Timer1_ConfigType timer1_configurations = {0,7812,F_CPU_1024,COMPARE};
	Timer1_init(&timer1_configurations);
	seconds=0;
	LCD_displayString("Door Is Locking");
	while(seconds !=15)
	{
	}
	Timer1_deInit();
}


/*
 * Description:
 * this function call 3 functions open_door(), hold_door()and close_door()
 * at the end of the function
 * receives DONE_DOOR_OPERATION macro from the Control_ECU to understand that the Control_ECU done the operation
 */
void door_operation(void)
{
	open_door();
	hold_door();
	close_door();

	sync_recive();
	while(UART_recieveByte()!= DONE_DOOR_OPERATION);
	LCD_clearScreen();
}

/*
 * Description:
 *  this function is responsible for display "WARNING WRONG PASSWORD" for 1 minute in HMI_ECU using timer1
 *  during the buzzer is active in Control_ECU
 *  and display "LOCKED SYSTEM FOR 1 MIN" in HMI_ECU using timer1
 *  and the saytem has no input during this period
 *
 */

void system_locked(void)
{
	LCD_clearScreen();
	Timer1_ConfigType timer1_configurations = {0,7812,F_CPU_1024,COMPARE};
	Timer1_init(&timer1_configurations);
	seconds=0;
	LCD_moveCursor(0,5);
	LCD_displayString("WARNING");
	LCD_moveCursor(1,1);
	LCD_displayString("WRONG PASSWORD");
	while(seconds !=60)
	{
	}
	LCD_clearScreen();
	seconds=0;
	LCD_displayString("SYSTEM LOCKED");
	LCD_moveCursor(1,0);
	LCD_displayString("FOR 1 MIN");
	while(seconds !=60)
	{
	}
	LCD_clearScreen();
	Timer1_deInit();
}

/*
 * Description:
 * This function prints "MATCHED PASSWORDS" in lcd
 */
void display_matched_on_lcd(void)
{
	LCD_clearScreen();
	LCD_moveCursor(0,4);
	LCD_displayString("MATCHED");
	LCD_moveCursor(1,3);
	LCD_displayString("PASSWORDS");
	_delay_ms(1000);
	LCD_clearScreen();
}

/*
 * Description:
 * This function prints "UNMATCHED PASSWORDS" in lcd
 */
void display_unmatched_on_lcd(void)
{
	LCD_clearScreen();
	LCD_moveCursor(0,3);
	LCD_displayString("UNMATCHED");
	LCD_moveCursor(1,3);
	LCD_displayString("PASSWORDS");
	_delay_ms(1000);
	LCD_clearScreen();
}





int main (void){
	initialization();
	uint8 pass_one[5];  //array to save the first time the user enters a password and sent it to Control_ECU
	uint8 pass_two[5];  //array to save the second time the user enters a password and sent it to Control_ECU
	uint8 pass_three[5];//array to save the any more time the user enters password to check and sent it to Control_ECU
	uint8 flag;  //flag to start from CREATE_SYSTEM_PASSWORD or MENU

	sync_send();               //GETTING UART READY
	while (1)
	{
		/*
		 *  Waits to receive a flag from Control_ECU indicating the next action.
         *  Syncs communication and stores the flag in 'flag' variable.
		 */
		sync_recive();   // start receive a MACRO
		flag = UART_recieveByte();

		if (flag == CREATE_SYSTEM_PASSWORD)
		{
			uint8 matched_or_not;

			LCD_clearScreen();
			LCD_moveCursor(0,3);
			LCD_displayString("CREATE NEW");
			LCD_moveCursor(1,1);
			LCD_displayString("SYSTEM PASSWORD");
			_delay_ms(1500);


			LCD_clearScreen();
			LCD_displayString("Plz enter pass:");
			LCD_moveCursor(1,0);
			entering_password(pass_one);
			send_pass(pass_one);

			LCD_clearScreen();
			LCD_displayString("plz re-enter the");
			LCD_moveCursor(1,0);
			LCD_displayString("same pass :");
			entering_password(pass_two);
			send_pass(pass_two);
			sync_recive(); // sync start receive matched or unmatched bit
			matched_or_not = UART_recieveByte();

			if (matched_or_not == MATCHED_PASSWORDS)
			{
				//print matched passwords on the lcd
				display_matched_on_lcd();


				sync_recive();   // start receive a MACRO which is MENU
				flag = UART_recieveByte();
			}
			else if (matched_or_not == UNMATCHED_PASSWORDS)
			{
				//print UNmatched passwords on the lcd
				display_unmatched_on_lcd();

				// if not matched start while and wait for the flag
			}
		}

		if (flag == MENU)
		{
			uint8 open_or_change;
			LCD_clearScreen();
			LCD_displayString("+ : Open Door");
			LCD_moveCursor(1,0);
			LCD_displayString("- : Change Pass");
			open_or_change = KEYPAD_getPressedKey();

			sync_send();
			UART_sendByte(open_or_change); //sent the pressed key if + or - to Control_ECU

			if (open_or_change == '+')
			{

				uint8 check;
				while(1)
				{
					LCD_clearScreen();
					LCD_displayString("Plz enter pass:");
					LCD_moveCursor(1,0);
					entering_password(pass_three);
					send_pass(pass_three);


					sync_recive(); // sync start receive matched or unmatched bit
					check = UART_recieveByte();

					if (check == MATCHED_PASSWORDS)
					{
						/*
						 *  if matched do Door Operation and then break
						 *  and start the main while(1) and wait for the flag from Control_ECU
						 *  which will be MENU again
					     */
						door_operation();
						break;
					}
					else if (check== LOCKED)
					{
						/*
						 *  if LOCKED do system locked Operation
						 * and start the main while(1) and wait for the flag from Control_ECU
						 *  which will be MENU again
					     */
						system_locked();
						break;
					}
					else if (check== UNMATCHED_PASSWORDS)
					{
						LCD_clearScreen();
						LCD_displayString("please try again");
						_delay_ms(1000);
					}
				}

			}
			else if (open_or_change == '-')
			{
				uint8 check;
				while(1)
				{
					LCD_clearScreen();
					LCD_displayString("Plz enter pass:");
					LCD_moveCursor(1,0);
					entering_password(pass_three);
					send_pass(pass_three);

					sync_recive(); // sync start receive matched or unmatched bit
					check = UART_recieveByte();

					if (check == MATCHED_PASSWORDS)
					{
						/*
						 *  if  MATCHED_PASSWORDS start the main while(1) from begging and
						 *  wait for the flag from Control_ECU which will be CREATE_SYSTEM_PASSWORD
						 */
						break;
					}
					else if (check== LOCKED)
					{
						/*
						 *  if  LOCKED do the system locked operation
						 *   start the while from begging and
						 *  wait for the flag from Control_ECU which will be MENU
						 */
						system_locked();
						break;
					}
					else if (check== UNMATCHED_PASSWORDS)
					{
						LCD_clearScreen();
						LCD_displayString("please try again");
						_delay_ms(1000);
					}
				}
			}
		}
	}
}

