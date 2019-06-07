#include "common.h"


/****************************************************************
 *  Written by ERTUGRUL AYPEK
 * 
 *****CODE DESIGN:*****
 *There are two tasks. One is  task1 where movements and rotations occur.
 *The other one is LCD task which waits for task1 to fire its semaphore (LCD_EVENT).
 *
 *
 *4 semaphores used for concurrency:
 *--ALARM_EVENT:    Fired every 40ms to wake task1 up
 *--TRANSMIT_EVENT: Set by transmitData() function in common.c. It is waited by each transmitBuffer_push() function to achieve transmit buffer concurrency.
 *--RECEIVE_EVENT:  Set by receiveData() function in common.c. It is waited by each transmitBuffer_pop() function to achieve receive buffer concurrency.
 *--LCD_EVENT:      Set by task1 to wake LCD task up and refresh LCD.
 *
 *
 * 
 * Task1 uses the algorithm provided among homework files. After receiving GO command and moving to ACTIVE state, 
 * it first looks around, then updates map, then decides the movement according to given algorithm. 
 * For simplicity, after rotating robot into correct orientation; it calculates robot's front cell and then makes robot move there. 
 * Task1 fires LCD task each 400 ms. 
 * 
 * 
 * There are some comminucation functions to notice:
 * In task1: communication functions to get current maze map 
 * --char* getFirstRow();
 * --char* getSecondRow();
 * --char* getThirdRow();
 * --char* getFourthRow();
 * 
 * In int.c: communication function to get whether to display first or second two rows
 * --char getDisplayMode();
 * 
 * In int.c: communication function to get number of tick counts to fire LCD task each 400ms
 * --int getTickCount();
 * 
 * 
 * For bonus part, a variable which counts each newly explored cells is kept and whenever it becomes 64 i.e all cells are explored,
 * it changes task1's state to Target ('T') state. In ('T') state the inverse of given algorithm is applied and the robot stops whenever it reachs
 * the cell with coordinate [0,15] i.e Target Cell. 
 * Speaking of inverse of given algorithm, which is a bit tricky, I mean:
 * --if robot's left is empty:          rotate left and move forward
 * --else if robot's front is empty:    move forward
 * --else:                              rotate right
 * 
 * Further brief explanations can be found on top of each helper and each configuration.
 * 
 * 
 * 
 *******************************************************************/





/**********************************************************************
 * --------------------- COUNTER & ALARM DEFINITION -------------------
 **********************************************************************/
Counter Counter_list[] = 
  {
   /*******************************************************************
    * -------------------------- First counter ------------------------
    *******************************************************************/
   {
     {
       200,                                /* maxAllowedValue        */
        10,                                /* ticksPerBase           */
       100                                 /* minCycle               */
     },
     0,                                    /* CounterValue           */
     0                                     /* Nbr of Tick for 1 CPT  */
   }
  };

Counter Counter_kernel = 
  {
    {
      65535,                              /* maxAllowedValue        */
          1,                              /* ticksPerBase           */
          0                               /* minCycle               */
    },
    0,                                    /* CounterValue           */
    0                                     /* Nbr of Tick for 1 CPT  */
  };

AlarmObject Alarm_list[] = 
  {
   /*******************************************************************
    * -------------------------- First task ---------------------------
    *******************************************************************/
   {
     OFF,                                  /* State                   */
     0,                                    /* AlarmValue              */
     0,                                    /* Cycle                   */
     &Counter_kernel,                      /* ptrCounter              */
     TASK0_ID,                             /* TaskID2Activate         */
     ALARM_EVENT,                          /* EventToPost             */
     0                                     /* CallBack                */
   },
   /*******************************************************************
    * -------------------------- Second task --------------------------
    *******************************************************************/
   {
     OFF,                                  /* State                   */
     0,                                    /* AlarmValue              */
     0,                                    /* Cycle                   */
     &Counter_kernel,                      /* ptrCounter              */
     TASK1_ID,                             /* TaskID2Activate         */
     ALARM_EVENT,                          /* EventToPost             */
     0                                     /* CallBack                */
   },
   /*******************************************************************
    * -------------------------- Third task --------------------------
    *******************************************************************/
   {
     OFF,                                  /* State                   */
     0,                                    /* AlarmValue              */
     0,                                    /* Cycle                   */
     &Counter_kernel,                      /* ptrCounter              */
     LCD_ID,                               /* TaskID2Activate         */
     ALARM_EVENT,                          /* EventToPost             */
     0                                     /* CallBack                */
   },
 };

#define _ALARMNUMBER_          sizeof(Alarm_list)/sizeof(AlarmObject)
#define _COUNTERNUMBER_        sizeof(Counter_list)/sizeof(Counter)
unsigned char ALARMNUMBER    = _ALARMNUMBER_;
unsigned char COUNTERNUMBER  = _COUNTERNUMBER_;
unsigned long global_counter = 0;

/**********************************************************************
 * --------------------- COUNTER & ALARM DEFINITION -------------------
 **********************************************************************/
Resource Resource_list[] = 
  {
   {
      10,                                /* priority           */
       0,                                /* Task prio          */
       0,                                /* lock               */
   }
  };
  
#define _RESOURCENUMBER_       sizeof(Resource_list)/sizeof(Resource)
unsigned char RESOURCENUMBER = _RESOURCENUMBER_;

/**********************************************************************
 * ----------------------- TASK & STACK DEFINITION --------------------
 **********************************************************************/
#define DEFAULT_STACK_SIZE      256
DeclareTask(TASK0);
DeclareTask(TASK1);
DeclareTask(LCD);
// to avoid any C18 map error : regroup the stacks into blocks
// of 256 bytes (except the last one).
#pragma		udata      STACK_A   
volatile unsigned char stack0[DEFAULT_STACK_SIZE];
#pragma		udata      STACK_B   
volatile unsigned char stack1[DEFAULT_STACK_SIZE];
#pragma		udata
volatile unsigned char stack2[DEFAULT_STACK_SIZE];
#pragma		udata

/**********************************************************************
 * ---------------------- TASK DESCRIPTOR SECTION ---------------------
 **********************************************************************/
#pragma		romdata		DESC_ROM
const rom unsigned int descromarea;
/**********************************************************************
 * -----------------------------  task 0 ------------------------------
 **********************************************************************/


/**********************************************************************
 * -----------------------------  task 1 ------------------------------
 **********************************************************************/
rom_desc_tsk rom_desc_task1 = {
	TASK1_PRIO,                       /* prioinit from 0 to 15       */
	stack1,                           /* stack address (16 bits)     */
	TASK1,                            /* start address (16 bits)     */
	READY,                            /* state at init phase         */
	TASK1_ID,                         /* id_tsk from 0 to 15         */
	sizeof(stack1)                    /* stack size    (16 bits)     */
};

rom_desc_tsk rom_desc_task2 = {
	LCD_PRIO,                       /* prioinit from 0 to 15       */
	stack2,                           /* stack address (16 bits)     */
	LCD,                            /* start address (16 bits)     */
	READY,                            /* state at init phase         */
	LCD_ID,                         /* id_tsk from 0 to 15         */
	sizeof(stack2)                    /* stack size    (16 bits)     */
};
/**********************************************************************
 * --------------------- END TASK DESCRIPTOR SECTION ------------------
 **********************************************************************/
rom_desc_tsk end = {
	0x00,                              /* prioinit from 0 to 15       */
	0x0000,                            /* stack address (16 bits)     */
	0x0000,                            /* start address (16 bits)     */
	0x00,                              /* state at init phase         */
	0x00,                              /* id_tsk from 0 to 15         */
	0x0000                             /* stack size    (16 bits)     */
};

volatile rom unsigned int * taskdesc_addr = (&(descromarea)+1);
	
/* End of File : taskdesc.c */
