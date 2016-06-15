/**********************************************************************************************************************
File: user_app.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app as a template:
 1. Copy both user_app.c and user_app.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserAppInitialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserAppRunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserAppFlags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp_StateMachine;            /* The state machine function pointer */
static u32 UserApp_u32Timeout;                      /* Timeout counter used across states */

 static u32 UserApp_u32UserAppFlags;                      
 static u8  UserApp_u8WinnerPlayer=9;                  
 static u8  UserApp_u8ModeNumber;                    
 static u8  UserApp_u8ChannelNumber;                 
 static u8  UserApp_u8DeviceIdHigh;                  
 static u8  UserApp_u8DeviceIdLow;                   

/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserAppInitialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserAppInitialize(void)
{
  
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp_StateMachine = UserAppSM_IdentificationInitialize;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp_StateMachine = UserAppSM_FailedInit;
  }

} /* end UserAppInitialize() */


/*----------------------------------------------------------------------------------------------------------------------
Function UserAppRunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserAppRunActiveState(void)
{
  UserApp_StateMachine();

} /* end UserAppRunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/





/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void UserAppSM_FailedInit(void)          
{
    
} /* end UserAppSM_FailedInit() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* initialize */
static void UserAppSM_IdentificationInitialize(void)
{
  static u8 au8ModeMessage[] = "Please choose your identification (0-referee,1-player):";
  static u8 au8ChannelMessage[] = "Please input your channel (0 ~ 3):";
  static u8 au8DeviceMessage[] = "Please input Device ID:";
  static u8 au8ErrorMessage[] = "Warning! Invalid input";
  
  static u8 au8DebugInputBuffer[8];
  static u8 u8DebugInputState = 0;
  
  static u8 u8CounterFor5ms = 0;
  
  
  AntAssignChannelInfoType sChannelInfo;
  u8CounterFor5ms++;
  if(u8DebugInputState == 0)
  {
    DebugLineFeed();
    DebugPrintf(au8ModeMessage);
    DebugLineFeed();
    u8DebugInputState++;
  }
  
  if(u8DebugInputState == 2)
  {
    DebugLineFeed();
    DebugPrintf(au8ChannelMessage);
    DebugLineFeed();
    u8DebugInputState++;   
  }
  
  if(u8DebugInputState == 4)
  {
    DebugLineFeed();
    DebugPrintf(au8DeviceMessage);
    DebugLineFeed();
    u8DebugInputState++;   
  }
  
  if(u8DebugInputState == 9)
  {
    u8DebugInputState = 0;
    u8CounterFor5ms = 0;
    UserApp_StateMachine = UserAppSM_AssignChannel;
  }
 
  if(u8CounterFor5ms == 5)
  {
    u8CounterFor5ms = 0;
    
    if(DebugScanf(au8DebugInputBuffer))
    {
      switch(u8DebugInputState)
      {
        case 1:
          if(au8DebugInputBuffer[0] == '0' || au8DebugInputBuffer[0] == '1')
          {     
            UserApp_u8ModeNumber = au8DebugInputBuffer[0] - 0x30;
            if(UserApp_u8ModeNumber == 0)
            {
              u8DebugInputState++;
              u8DebugInputState++;
            }
            u8DebugInputState++;
          }
          else
          {
            DebugLineFeed();
            DebugPrintf(au8ErrorMessage);
            DebugLineFeed();
          }
          break;
        case 3:
          if(au8DebugInputBuffer[0] >= 0x30 && au8DebugInputBuffer[0] <= 0x33)
          {
            UserApp_u8ChannelNumber = au8DebugInputBuffer[0] - 0x30;
            u8DebugInputState++;         
          }
          else
          {
            DebugLineFeed();
            DebugPrintf(au8ErrorMessage);
            DebugLineFeed();
          }
          break;
        case 5:
          if(au8DebugInputBuffer[0] >= 0x30 && au8DebugInputBuffer[0] <= 0x39)
          {
            UserApp_u8DeviceIdHigh = au8DebugInputBuffer[0] - 0x30;
            u8DebugInputState++;                    
          }
          else
          {
            DebugLineFeed();
            DebugPrintf(au8ErrorMessage);
            DebugLineFeed();
          }
          break;
        case 6:          
          if(au8DebugInputBuffer[0] >= 0x30 && au8DebugInputBuffer[0] <= 0x39)
          {
            UserApp_u8DeviceIdHigh = UserApp_u8DeviceIdHigh * 10 + au8DebugInputBuffer[0] - 0x30;
            u8DebugInputState++;                    
          }
          else
          {
            DebugLineFeed();
            DebugPrintf(au8ErrorMessage);
            DebugLineFeed();
            u8DebugInputState = 5;
          }
          break; 
        case 7:
          if(au8DebugInputBuffer[0] >= 0x30 && au8DebugInputBuffer[0] <= 0x39)
          {
            UserApp_u8DeviceIdLow = au8DebugInputBuffer[0] - 0x30;
            u8DebugInputState++;                    
          }
          else
          {
            DebugLineFeed();
            DebugPrintf(au8ErrorMessage);
            DebugLineFeed();
            u8DebugInputState = 5;
          }
          break;
        case 8:          
          if(au8DebugInputBuffer[0] >= 0x30 && au8DebugInputBuffer[0] <= 0x39)
          {
            UserApp_u8DeviceIdLow = UserApp_u8DeviceIdLow * 10 + au8DebugInputBuffer[0] - 0x30;
            u8DebugInputState++;                    
          }
          else
          {
            DebugLineFeed();
            DebugPrintf(au8ErrorMessage);
            DebugLineFeed();
            u8DebugInputState = 5;
          }
          break;   
        default:
          break;
      }
    }
  }
} /* end UserAppSM_IdentificationInitialize() */

/*-------------------------------------------------------------------------------------------------------------------*/
/* assign ANT */
static void UserAppSM_AssignChannel(void)
{
} /* end UserAppSM_AssignChannel() */

/*-------------------------------------------------------------------------------------------------------------------*/
/* open ANT */
static void UserAppSM_OpenChannel(void)
{
} /* end UserAppSM_OpenChannel() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* pair to all players */
static void UserAppSM_WaitForPairing(void)
{
}/* end UserAppSM_WaitForPairing() */

/*-------------------------------------------------------------------------------------------------------------------*/
/* referee launches the game */
static void UserAppSM_LaunchGame(void)
{
}/* end UserAppSM_LaunchGame() */

/*-------------------------------------------------------------------------------------------------------------------*/
/* referee ends the game */
static void UserAppSM_EndGame(void)
{
} /* end UserAppSM_EndGame() */

/*-------------------------------------------------------------------------------------------------------------------*/
/* players wait for the game */
static void UserAppSM_WaitGameStart(void)
{
} /* end UserAppSM_WaitGameStart() */

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for a message to be queued */
static void UserAppSM_Idle(void)
{  
} /* end UserAppSM_Idle() */

/*-------------------------------------------------------------------------------------------------------------------*/
/* change the number */
static void UserAppSM_MyTurn(void)
{ 
} /* end UserAppSM_MyTurn() */

/*-------------------------------------------------------------------------------------------------------------------*/
/* someone wins the game */
static void UserAppSM_Congratulation(void)
{
} /* end UserAppSM_Congratulation() */

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserAppSM_Error(void)          
{  
} /* end UserAppSM_Error() */
