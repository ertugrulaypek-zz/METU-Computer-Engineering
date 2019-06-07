#include "common.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/
char receive_state = _WAITING;    // state of the system; _WAITING or _OPERATING
char systemState = _WAITING;    // state of the system; _WAITING or _OPERATING
char transmitBuffer[SIZE_TM_BUF];         // holds the bytes to be transmitted
unsigned char transmitBuffer_popPtr = 0;         
unsigned char transmitBuffer_pushPtr = 0;
char receiveBuffer[SIZE_RC_BUF]; // holds received bytes
unsigned char receiveBuffer_popPtr = 0;
unsigned char receiveBuffer_pushPtr = 0;
unsigned char i=0;
unsigned char j=0;
unsigned char excuse_me = 0;

/**********************************************************************
 * ----------------------- GLOBAL FUNCTIONS ---------------------------
 **********************************************************************/

void transmitBuffer_push(char *str,unsigned char size) // pushes reverse str to buffer. UNSAFE! LOCK FIRST!
{
    excuse_me = 1; //kindly interject transmission, this is atomic.
    i = 0;
    while(i < size)
    {
        transmitBuffer[transmitBuffer_pushPtr] = str[i];
        transmitBuffer_pushPtr = (transmitBuffer_pushPtr+1)%SIZE_TM_BUF;
        i++;
    }
    excuse_me = 0;
    TXSTA1bits.TXEN = 1; //enable transmission.
}

void transmitData()
{
    if(transmitBuffer_popPtr != transmitBuffer_pushPtr && !excuse_me)
    {

        TXREG1 = transmitBuffer[transmitBuffer_popPtr];
        transmitBuffer_popPtr = (transmitBuffer_popPtr+1)%SIZE_TM_BUF;
    }
    else
    {
    TXSTA1bits.TXEN = 0;// disable transmission, nothing to push, or excuse_me.
    SetEvent(TASK1_ID,TRANSMIT_EVENT);
    }
}

unsigned char receiveBuffer_pop(char *str) //pops str from recieved string $str:. UNSAFE! LOCK FIRST! returns the length of the popped string.
{
    j = 0;
    
    while(receiveBuffer[receiveBuffer_popPtr] != '$' && receiveBuffer_popPtr != receiveBuffer_pushPtr)
    {
        receiveBuffer_popPtr = (receiveBuffer_popPtr+1)%SIZE_RC_BUF;
    }

    if (receiveBuffer_popPtr == receiveBuffer_pushPtr) return 0; // ignore garbage

    receiveBuffer_popPtr = (receiveBuffer_popPtr+1)%SIZE_RC_BUF;; //skip $

    while(receiveBuffer[receiveBuffer_popPtr] != ':' && receiveBuffer_popPtr != receiveBuffer_pushPtr)
    {
        str[j] = receiveBuffer[receiveBuffer_popPtr];
        j++;
        receiveBuffer_popPtr = (receiveBuffer_popPtr+1)%SIZE_RC_BUF;
    }

    if (receiveBuffer_popPtr == receiveBuffer_pushPtr) return 0; //ignore garbage, the string did not end with ":".

    receiveBuffer_popPtr = (receiveBuffer_popPtr+1)%SIZE_RC_BUF; //skip ":"

    return j;
}


/* Invoked when receive interrupt occurs; meaning that data is received */
void dataReceived()
{
    unsigned char rxbyte = RCREG1;
    
    
    if(receive_state == _WAITING && rxbyte == '$')
    {
       receiveBuffer[receiveBuffer_pushPtr] = rxbyte;
       receiveBuffer_pushPtr = (receiveBuffer_pushPtr+1)%SIZE_RC_BUF;
       receive_state = _OPERATING;
    }
    else if(receive_state == _OPERATING && rxbyte != ':')
    {
       receiveBuffer[receiveBuffer_pushPtr] = rxbyte;
       receiveBuffer_pushPtr = (receiveBuffer_pushPtr+1)%SIZE_RC_BUF;
    }
    else if(receive_state == _OPERATING && rxbyte == ':')
    {
       receiveBuffer[receiveBuffer_pushPtr] = rxbyte;
       receiveBuffer_pushPtr = (receiveBuffer_pushPtr+1)%SIZE_RC_BUF;
       receive_state = _WAITING;
       
       
       SetEvent(TASK1_ID,RECEIVE_EVENT);
    }
    

}

/* End of File : common.c */
