#ifndef _COMMON_H
#define COMMON_H

#include "device.h"

/***********************************************************************
 * ------------------------ Timer settings -----------------------------
 **********************************************************************/
#define _10MHZ	63320
#define _16MHZ	61768
#define _20MHZ	60768
#define _32MHZ	57768
#define _40MHZ 	55768

/***********************************************************************
 * ----------------------------- Events --------------------------------
 **********************************************************************/
#define ALARM_EVENT       0x80
#define RECEIVE_EVENT   0x40
#define TRANSMIT_EVENT  0x01
#define LCD_EVENT       0X02

/***********************************************************************
 * ----------------------------- Task ID -------------------------------
 **********************************************************************/
/* Info about the tasks:
 * TASK0: USART
 * TASK1: USART
 */
#define TASK0_ID             1
#define TASK1_ID             2
#define LCD_ID               3


/* Priorities of the tasks */
#define TASK0_PRIO           8
#define TASK1_PRIO           8
#define LCD_PRIO			 10

#define ALARM_TSK0           0
#define ALARM_TSK1           1
#define LCD_ALARM_ID		 2
/**********************************************************************
 * ----------------------- GLOBAL DEFINITIONS -------------------------
 **********************************************************************/

/* System States */
#define _WAITING	0		// waiting state
#define _OPERATING      3

#define SIZE_TM_BUF 100
#define SIZE_RC_BUF 100


/**********************************************************************
 * ----------------------- FUNCTION PROTOTYPES ------------------------
 **********************************************************************/
 /* transmits data using serial communication */
void transmitData();
/* Invoked when receive interrupt occurs; meaning that data is received */
void dataReceived();

void transmitBuffer_push(char *str,unsigned char size);

unsigned char receiveBuffer_pop(char *str);

//communication functions to get current maze map 
char* getFirstRow();
char* getSecondRow();
char* getThirdRow();
char* getFourthRow();

//communication function to get whether to display first or second two rows
char getDisplayMode();

//communication function to get number of tick counts 
int getTickCount();
#endif

/* End of File : common.h */
