/******************************************************************************
 *
 * Module: TWI(I2C)
 *
 * File Name: twi.h
 *
 * Description: Source file for the TWI(I2C) AVR driver
 *
 * Author: Marco
 *
 *******************************************************************************/

#include "twi.h"
#include "common_macros.h"
#include <avr/io.h>

void TWI_init(const TWI_ConfigType * Config_Ptr)
{

	 TWCR = (1<<TWEN); /* enable TWI */

	 /* Bit Rate: 400.000 kbps using zero pre-scaler TWPS=00 and F_CPU=8Mhz */


	 TWSR = 0x00;
	 /*
	  * TWPS should change based on the other factors
	  * we will assume here TWPS =0 , so 4 power 0 = 1
	  */
	 TWBR  = ((F_CPU /(Config_Ptr->bit_rate)) - 16) / (2 * 1);

	  /* Two Wire Bus address my address if any master device want to call me:with my address
	   *  (used in case this MC is a slave device) General Call Recognition: Off
	   */

	  TWAR = ((Config_Ptr-> address)<<1); // my address = 0x01 :)
}

void TWI_start(void)
{
    /*
	 * Clear the TWINT flag before sending the start bit TWINT=1
	 * send the start bit by TWSTA=1
	 * Enable TWI Module TWEN=1
	 *
	 * unfortunatily the enable in the same register
	 * if you did this to enable start bit  TWCR = TWCR | (1 << TWSTA) by this way you disabled TWEN which is the module enable (12c)
	 *
	 * * you can not use ORing equal technique(|=) here , because sometimes you won't need to save the old bits
	 * في اوقات هتعوز تخلي بيتس معينه بوحايد والباقي باصفاربس الاينيبل ثابته معاك دايما
	 * مثلا انت عاوز تكتب بايت او تقرا بايت انت ولا عاوز استارت ولا استوب ولا ااك
	 * EVERYTIME YOU WILL HAVE TO ENABLE THE TWEN
	 * | (1 << TWINT) ----> دي بتمسح الفلاج احنا هنا بنمسحه واحنا داخلين عشان نمسح الفلاج بنحطه بواحد
	 * | (1 << TWEN) ---> enable twi module
	 * | (1 << TWSTA) ---> clear start bit at the start or end of the function
	 *
	 */
    TWCR = (1 << TWEN) | (1 << TWSTA) | (1 << TWINT );

    /* Wait for TWINT flag set in TWCR Register (start bit is send successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));    //طول ما هو ب زيرو هو قاعد مستني اول مايبقي  ب 1 يبقي خلص بعت يخرج
}


void TWI_stop(void)
{
    /*
	 * Clear the TWINT flag before sending the stop bit TWINT=1
	 * send the stop bit by TWSTO=1
	 * Enable TWI Module TWEN=1
	 *  (using polling like in the start function)مالهاش لازمه انك تمسح الفلاج هنا زي في الستارت انت كده كده خلصت فملهاش معني
	 */
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}


void TWI_writeByte(uint8 data)
{
	 /* Put data On TWI data Register */
	  TWDR = data;
	 /*
	  * Clear the TWINT flag before sending the data TWINT=1 (تمسح الفلاج قبل ما تدخل كل فانكشن زي ما قولنا )
	  * Enable TWI Module TWEN=1  (and enable the module)
	  */
	  TWCR = (1 << TWINT) | (1 << TWEN);
	 /* Wait for TWINT flag set in TWCR Register(data is send successfully) */
	  while(BIT_IS_CLEAR(TWCR,TWINT));
}

uint8 TWI_readByteWithACK(void)
{
	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable sending ACK after reading or receiving data TWEA=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
    /* Read Data */
    return TWDR;
}

uint8 TWI_readByteWithNACK(void)
{
	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable TWI Module TWEN=1
	 * الفرق بين دي والتانيه ان هنا مش بتعمل اينبل لل ااك
	 */
    TWCR = (1 << TWINT) | (1 << TWEN);
    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
    /* Read Data */
    return TWDR;
}

uint8 TWI_getStatus(void)
{
    uint8 status;
    /* masking to eliminate first 3 bits and get the last 5 bits (status bits) */
    status = TWSR & 0xF8;
    return status;
}




