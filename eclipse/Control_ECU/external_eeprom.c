 /******************************************************************************
 *
 * Module: External EEPROM
 *
 * File Name: external_eeprom.c
 *
 * Description: Source file for the External EEPROM Memory
 *
 * Author: Marco
 *
 *******************************************************************************/
#include "external_eeprom.h"
#include "twi.h"

uint8 EEPROM_writeByte(uint16 u16addr, uint8 u8data)
{

	/*
	 * كل حاجه هعملها هتشيك ان هي حصلت مظبوط عن طريق ان اقرا الرقم الموجود جوه الاستتس رجيستر لو حصلت هنكمل محصلتش هنرجع ايرور
	 */
	/* Send the Start Bit */
    TWI_start();
    if (TWI_getStatus() != TWI_START)
        return ERROR;

    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=0 (write) */
    TWI_writeByte((uint8)(0xA0 | ((u16addr & 0x0700)>>7)));
    if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
        return ERROR;

    /* Send the required memory location address */
    /*
     * when you are sending the address inside the memory just sent the first 8-bit ,
     * the last three bit we have already sent it with the device address
     * that's why we made casting to uint8
     */
    TWI_writeByte((uint8)(u16addr));
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return ERROR;

    /* write byte to eeprom */
    TWI_writeByte(u8data);
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return ERROR;

    /* Send the Stop Bit */
    TWI_stop();

    return SUCCESS;
}

uint8 EEPROM_readByte(uint16 u16addr, uint8 *u8data)
{
	/* Send the Start Bit */
    TWI_start();
    if (TWI_getStatus() != TWI_START)
        return ERROR;

    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=0 (write) */
    TWI_writeByte((uint8)((0xA0) | ((u16addr & 0x0700)>>7)));
    if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
        return ERROR;

    /* Send the required memory location address */
    TWI_writeByte((uint8)(u16addr));
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return ERROR;

    /* Send the Repeated Start Bit */
    TWI_start();
    if (TWI_getStatus() != TWI_REP_START)
        return ERROR;

    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=1 (Read) */
    TWI_writeByte((uint8)((0xA0) | ((u16addr & 0x0700)>>7) | 1));
    if (TWI_getStatus() != TWI_MT_SLA_R_ACK)
        return ERROR;

    /* Read Byte from Memory without send ACK */
    *u8data = TWI_readByteWithNACK();
    if (TWI_getStatus() != TWI_MR_DATA_NACK)
        return ERROR;

    /* Send the Stop Bit */
    TWI_stop();

    return SUCCESS;
}
