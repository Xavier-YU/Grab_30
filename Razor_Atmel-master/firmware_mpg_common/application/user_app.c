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



extern u32 G_u32AntApiCurrentDataTimeStamp;                       /* From ant_api.c */
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;    /* From ant_api.c */
extern u8 G_au8AntApiCurrentData[ANT_APPLICATION_MESSAGE_BYTES];  /* From ant_api.c */
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
 
 static u8  UserApp_au8GameSituationMessage[ANT_APPLICATION_MESSAGE_BYTES] = {0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80};

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
  static u16  UserApp_u16DeviceIdInt;  
  
  
  AntAssignChannelInfoType sChannelInfo;
  u8CounterFor5ms++;
  
  /*print some messages on the Tera Term to remind the user of what to do*/
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
    UserApp_u8DeviceIdHigh=UserApp_u16DeviceIdInt/256;
    UserApp_u8DeviceIdLow=UserApp_u16DeviceIdInt%256;
    if(UserApp_u8ModeNumber == 0)
    {
      DebugPrintf("\n\rplease press 4 buttons to assign the channels\n\r");
    }
    UserApp_StateMachine = UserAppSM_AssignChannel;
  }
 
  if(u8CounterFor5ms == 5)
  {
    u8CounterFor5ms = 0;
    
    if(DebugScanf(au8DebugInputBuffer))
    {
      switch(u8DebugInputState)
      {
        
        /*get the Mode*/
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
          
        /*get channel number*/  
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
          
        /*get device ID*/
        case 5:
          if(au8DebugInputBuffer[0] >= 0x30 && au8DebugInputBuffer[0] <= 0x39)
          {
            UserApp_u16DeviceIdInt = au8DebugInputBuffer[0] - 0x30;
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
            UserApp_u16DeviceIdInt = UserApp_u16DeviceIdInt * 10 + au8DebugInputBuffer[0] - 0x30;
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
            UserApp_u16DeviceIdInt = UserApp_u16DeviceIdInt * 10 + au8DebugInputBuffer[0] - 0x30;
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
            UserApp_u16DeviceIdInt = UserApp_u16DeviceIdInt * 10 + au8DebugInputBuffer[0] - 0x30;
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
  static u8 u8AllAssignIndex = 0;
   AntAssignChannelInfoType sChannelInfo;
   if(UserApp_u8ModeNumber == 0)
    {
      /*if BUTTON0 is pressed , assign Channel0*/
      if(WasButtonPressed(BUTTON0))
      {
        ButtonAcknowledge(BUTTON0);
        if(AntRadioStatusChannel(0) == ANT_UNCONFIGURED)
        {
          sChannelInfo.AntChannel = 0;
          sChannelInfo.AntChannelType = CHANNEL_TYPE_MASTER;
          sChannelInfo.AntChannelPeriodHi = 0x0c;
          sChannelInfo.AntChannelPeriodLo = 0xcd;
          
          sChannelInfo.AntDeviceIdHi = UserApp_u8DeviceIdHigh;
          sChannelInfo.AntDeviceIdLo = UserApp_u8DeviceIdLow;
          sChannelInfo.AntDeviceType = ANT_DEVICE_TYPE_DEFAULT;
          sChannelInfo.AntTransmissionType = ANT_TRANSMISSION_TYPE_DEFAULT;
          
          sChannelInfo.AntFrequency = 45;
          sChannelInfo.AntTxPower = ANT_TX_POWER_DEFAULT;
          sChannelInfo.AntNetwork = ANT_NETWORK_DEFAULT;
          
          for(u8 j = 0; j < ANT_NETWORK_NUMBER_BYTES; j++)
          {
            sChannelInfo.AntNetworkKey[j] = ANT_DEFAULT_NETWORK_KEY;
          }
          if(AntAssignChannel(&sChannelInfo))
          {
            u8AllAssignIndex++;
          }

        }        
        else
        {
          AntUnassignChannelNumber(0);
        }  
      }
      
       /*if BUTTON1 is pressed , assign Channel1*/     
      if(WasButtonPressed(BUTTON1))
      {
        ButtonAcknowledge(BUTTON1);
        if(AntRadioStatusChannel(1) == ANT_UNCONFIGURED)
        {
          sChannelInfo.AntChannel = 1;
          sChannelInfo.AntChannelType = CHANNEL_TYPE_MASTER;
          sChannelInfo.AntChannelPeriodHi = 0x0c;
          sChannelInfo.AntChannelPeriodLo = 0xcd;
          
          sChannelInfo.AntDeviceIdHi = UserApp_u8DeviceIdHigh;
          sChannelInfo.AntDeviceIdLo = UserApp_u8DeviceIdLow +1;
          sChannelInfo.AntDeviceType = ANT_DEVICE_TYPE_DEFAULT;
          sChannelInfo.AntTransmissionType = ANT_TRANSMISSION_TYPE_DEFAULT;
          
          sChannelInfo.AntFrequency = 45;
          sChannelInfo.AntTxPower = ANT_TX_POWER_DEFAULT;
          sChannelInfo.AntNetwork = ANT_NETWORK_DEFAULT;
          
          for(u8 j = 0; j < ANT_NETWORK_NUMBER_BYTES; j++)
          {
            sChannelInfo.AntNetworkKey[j] = ANT_DEFAULT_NETWORK_KEY;
          }
          if(AntAssignChannel(&sChannelInfo))
          {
            u8AllAssignIndex++;
          }
        }        
        else
        {
          AntUnassignChannelNumber(1);
        }    
      }
      
      /*if BUTTON2 is pressed , assign Channel2*/
      if(WasButtonPressed(BUTTON2))
      {
        ButtonAcknowledge(BUTTON2);
        if(AntRadioStatusChannel(2) == ANT_UNCONFIGURED)
        {
          sChannelInfo.AntChannel = 2;
          sChannelInfo.AntChannelType = CHANNEL_TYPE_MASTER;
          sChannelInfo.AntChannelPeriodHi = 0x0c;
          sChannelInfo.AntChannelPeriodLo = 0xcd;
          
          sChannelInfo.AntDeviceIdHi = UserApp_u8DeviceIdHigh;
          sChannelInfo.AntDeviceIdLo = UserApp_u8DeviceIdLow+2;
          sChannelInfo.AntDeviceType = ANT_DEVICE_TYPE_DEFAULT;
          sChannelInfo.AntTransmissionType = ANT_TRANSMISSION_TYPE_DEFAULT;
          
          sChannelInfo.AntFrequency = 45;
          sChannelInfo.AntTxPower = ANT_TX_POWER_DEFAULT;
          sChannelInfo.AntNetwork = ANT_NETWORK_DEFAULT;
          
          for(u8 j = 0; j < ANT_NETWORK_NUMBER_BYTES; j++)
          {
            sChannelInfo.AntNetworkKey[j] = ANT_DEFAULT_NETWORK_KEY;
          }
          if(AntAssignChannel(&sChannelInfo))
          {
            u8AllAssignIndex++;
          }

        }        
        else
        {
          AntUnassignChannelNumber(2);
        }    
      }
 
      /*if BUTTON3 is pressed , assign Channel3*/
      if(WasButtonPressed(BUTTON3))
      {
        ButtonAcknowledge(BUTTON3);
        if(AntRadioStatusChannel(3) == ANT_UNCONFIGURED)
        {
          sChannelInfo.AntChannel = 3;
          sChannelInfo.AntChannelType = CHANNEL_TYPE_MASTER;
          sChannelInfo.AntChannelPeriodHi = 0x0c;
          sChannelInfo.AntChannelPeriodLo = 0xcd;
          
          sChannelInfo.AntDeviceIdHi = UserApp_u8DeviceIdHigh;
          sChannelInfo.AntDeviceIdLo = UserApp_u8DeviceIdLow+3;
          sChannelInfo.AntDeviceType = ANT_DEVICE_TYPE_DEFAULT;
          sChannelInfo.AntTransmissionType = ANT_TRANSMISSION_TYPE_DEFAULT;
          
          sChannelInfo.AntFrequency = 45;
          sChannelInfo.AntTxPower = ANT_TX_POWER_DEFAULT;
          sChannelInfo.AntNetwork = ANT_NETWORK_DEFAULT;
          
          for(u8 j = 0; j < ANT_NETWORK_NUMBER_BYTES; j++)
          {
            sChannelInfo.AntNetworkKey[j] = ANT_DEFAULT_NETWORK_KEY;
          }
          if(AntAssignChannel(&sChannelInfo))
          {
            u8AllAssignIndex++;
          }

        }        
        else
        {
          AntUnassignChannelNumber(3);
        }    
      }
    } 
    
    else
    {
      /*for a player ,it only needs to set up its own channel*/
       if(AntRadioStatusChannel(UserApp_u8ChannelNumber) == ANT_UNCONFIGURED)
        {
          sChannelInfo.AntChannel = UserApp_u8ChannelNumber;
          sChannelInfo.AntChannelType = CHANNEL_TYPE_MASTER;
          sChannelInfo.AntChannelPeriodHi = 0x00;
          sChannelInfo.AntChannelPeriodLo = 0x64;
          
          sChannelInfo.AntDeviceIdHi = UserApp_u8DeviceIdHigh;
          sChannelInfo.AntDeviceIdLo = UserApp_u8DeviceIdLow;
          sChannelInfo.AntDeviceType = ANT_DEVICE_TYPE_DEFAULT;
          sChannelInfo.AntTransmissionType = ANT_TRANSMISSION_TYPE_DEFAULT;
          
          sChannelInfo.AntFrequency = 45;
          sChannelInfo.AntTxPower = ANT_TX_POWER_DEFAULT;
          sChannelInfo.AntNetwork = ANT_NETWORK_DEFAULT;
          
          for(u8 j = 0; j < ANT_NETWORK_NUMBER_BYTES; j++)
          {
            sChannelInfo.AntNetworkKey[j] = ANT_DEFAULT_NETWORK_KEY;
          }
          if(AntAssignChannel(&sChannelInfo))
          {
            u8AllAssignIndex++;
          }
        } 
        else
        {
          AntUnassignChannelNumber(UserApp_u8ChannelNumber);
        }      
    }
   
    if(u8AllAssignIndex == 4 && UserApp_u8ModeNumber == 0||u8AllAssignIndex == 1 && UserApp_u8ModeNumber == 1 )
    {
      u8AllAssignIndex = 0;
      UserApp_StateMachine = UserAppSM_WaitOpenChannel;
    }
} /* end UserAppSM_AssignChannel() */

/*-------------------------------------------------------------------------------------------------------------------*/
/* wait ANT to be configured*/
static void UserAppSM_WaitOpenChannel(void)
{
  if(UserApp_u8ModeNumber == 0)
  {
    /*referee has to wait for all 4 channels have been assigned correctly*/
    if(AntRadioStatusChannel(0) != ANT_UNCONFIGURED && AntRadioStatusChannel(1) != ANT_UNCONFIGURED && AntRadioStatusChannel(2) != ANT_UNCONFIGURED && AntRadioStatusChannel(3) != ANT_UNCONFIGURED)    
    {
       UserApp_StateMachine = UserAppSM_OpenChannel;     
    }
  }
  else
  {
    /*player need to wait for its channel has been assigned*/
    if(AntRadioStatusChannel(UserApp_u8ChannelNumber) != ANT_UNCONFIGURED)
    {
       UserApp_StateMachine = UserAppSM_OpenChannel;        
    }
  }




} /* end UserAppSM_WaitOpenChannel() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* open all channels*/
static void UserAppSM_OpenChannel(void)
{
  static bool bOpenFlag = TRUE;
  
  if(AntReadAppMessageBuffer())
  {
  }
  
  
  if(UserApp_u8ModeNumber == 0)
  {
    if(bOpenFlag)
    {
      /*referee should open all four channels*/
      for(u8 i=0;i<4;i++)
      {
        if(AntRadioStatusChannel(i) == ANT_CLOSED)
        {
          AntOpenChannelNumber(i);
        }
      }
      bOpenFlag = FALSE;
    }
      
      
    if(AntRadioStatusChannel(0) == ANT_OPEN && AntRadioStatusChannel(1) == ANT_OPEN && AntRadioStatusChannel(2) == ANT_OPEN&&AntRadioStatusChannel(3) == ANT_OPEN)
    {
      bOpenFlag = TRUE;
      UserApp_StateMachine = UserAppSM_WaitForPairing;
    }
  }
  else
  {
    if(bOpenFlag)
    {
      /*player has to open its own channel*/
      if(AntRadioStatusChannel(UserApp_u8ChannelNumber) == ANT_CLOSED)
      {
        AntOpenChannelNumber(UserApp_u8ChannelNumber);
      }
      bOpenFlag = FALSE;
    }
      
      
    if(AntRadioStatusChannel(UserApp_u8ChannelNumber) == ANT_OPEN)
    {
      bOpenFlag = TRUE;
      UserApp_StateMachine = UserAppSM_WaitGameStart;
    }
  }
} /* end UserAppSM_OpenChannel() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* pair to all players */
static void UserAppSM_WaitForPairing(void)
{
  static u8 u8WaitPlayerIndex = 0;
  static u8 au8PlayerConnectMessage[] = "0 is connecting";
  static u8 au8AllConnectMessage[] = "All players are connected";
  static bool bStartGame = FALSE;
  static bool bWaitFlag = TRUE;
  static bool bFinishFlag = TRUE;
  static u8 u8CounterFor100ms = 0;
  
  u8CounterFor100ms++;


#if 0
  AntReadAppMessageBuffer();
#endif
  
  /* after all players are connected to the referee , press BUUTON0 to start the game */

  if(WasButtonPressed(BUTTON0) && u8WaitPlayerIndex == 4)
  {
    ButtonAcknowledge(BUTTON0);
    bStartGame = TRUE;
  }
     
  /* confirm the connection one by one , and diplay the situation on the LCD*/
  if(u8WaitPlayerIndex == 0 && bWaitFlag)
  {
    bWaitFlag = FALSE;
    LCDClearChars(LINE2_START_ADDR , 20);
    LCDMessage(LINE2_START_ADDR,au8PlayerConnectMessage);    
  }
  
#if 1
  if(AntReadAppMessageBuffer() && u8WaitPlayerIndex < 4)
  {
    if(G_eAntApiCurrentMessageClass == ANT_DATA && G_au8AntApiCurrentData[WHO_SEND_MESSAGE] == u8WaitPlayerIndex && G_au8AntApiCurrentData[CONTROL] == 0xff )
    {
      if(u8WaitPlayerIndex++ < 3)
      {
        au8PlayerConnectMessage[0] = u8WaitPlayerIndex + 0x30 ;
        LCDClearChars(LINE2_START_ADDR , 20);
        LCDMessage(LINE2_START_ADDR,au8PlayerConnectMessage);
      }
    }
  }
  
  if(u8WaitPlayerIndex == 4 && bFinishFlag)
  {
      LCDClearChars(LINE2_START_ADDR , 20);
      LCDMessage(LINE2_START_ADDR,au8AllConnectMessage); 
      bFinishFlag = FALSE;
  }
  #endif
  
  if(bStartGame)
  {
    /*change CONTROL to inform players that game starts*/
    UserApp_au8GameSituationMessage[CONTROL] = 0xff;
  }
  else
  {
    UserApp_au8GameSituationMessage[CONTROL] = 0x80;
  }
  if(u8CounterFor100ms == 100)
  {
    u8CounterFor100ms = 0;

    AntQueueBroadcastMessage(0,UserApp_au8GameSituationMessage); 
    AntQueueBroadcastMessage(1,UserApp_au8GameSituationMessage);
    AntQueueBroadcastMessage(2,UserApp_au8GameSituationMessage); 
    AntQueueBroadcastMessage(3,UserApp_au8GameSituationMessage);  
  }
  
  
    /*bStartGame is TRUE , start the game and prepare some variations for the possible new game*/
  if(bStartGame && u8CounterFor100ms == 0)
  {
    bStartGame = FALSE;
    bWaitFlag = TRUE;
    bFinishFlag = TRUE;
    u8CounterFor100ms = 0;
    u8WaitPlayerIndex = 0;
    au8PlayerConnectMessage[0] = 0x30;
    UserApp_StateMachine = UserAppSM_LaunchGame;
  }

}/* end UserAppSM_WaitForPairing() */

/*-------------------------------------------------------------------------------------------------------------------*/
/* referee launches the game */
static void UserAppSM_LaunchGame(void)
{
   static u8 u8CurrentPlayerIndex = 0;
  static bool bOnlineFlag = FALSE;
  static u8 u8WinnerFlag = 0;
  static bool bNewGameFlag = TRUE;
  static u8 u8CurrentNumber = 0;
  static u32 u32TryConnectionTime;
  static u32 u32WaitResponseTime;
  static u8 au8DisconnectMessage[] = "Player x is disconnected";
  static u8 au8CurrentPlayerMessage[] = "Player x is playing";
  static u8 au8CurrentNumberMessage[] = "Current Number is:xx";
  static u8 u8CounterFor100ms = 0;
  
  u8CounterFor100ms++;
  
  if(bNewGameFlag)
  {
    u32TryConnectionTime = G_u32SystemTime1s;
    bNewGameFlag = FALSE;
  }
  

  
  if(!bOnlineFlag)
  {
    /* each player will have 10 seconds to respond to the referee , otherwise current player is disconnected*/
    if(u8CounterFor100ms == 100 && G_u32SystemTime1s - u32TryConnectionTime > 10)
    {
      UserApp_au8GameSituationMessage[DISCONNECTION_PLAYER] = u8CurrentPlayerIndex;
      au8DisconnectMessage[7] = u8CurrentPlayerIndex + 0x30;
      DebugLineFeed();
      DebugPrintf(au8DisconnectMessage);
      DebugLineFeed();
      if(u8CurrentPlayerIndex < 3)
      {
        u8CurrentPlayerIndex++;
      }
      else
      {
        u8CurrentPlayerIndex = 0;
      }
      u32TryConnectionTime = G_u32SystemTime1s;
    }
    else if(AntReadAppMessageBuffer() && G_eAntApiCurrentMessageClass == ANT_DATA && G_au8AntApiCurrentData[WHO_SEND_MESSAGE] == u8CurrentPlayerIndex && G_au8AntApiCurrentData[CONTROL] == 0xff )
    {
      bOnlineFlag = TRUE;
      u32WaitResponseTime = G_u32SystemTime1s;
    }
  }
  else
  {
    /*wait for the operation of current player for 15s , otherwise add 2 automatically*/
    if(u8CounterFor100ms == 100 && G_u32SystemTime1s - u32WaitResponseTime > 15)
    {
      if(u8CurrentNumber + 2 >= 30)
      {
        u8CurrentNumber = 30;
        u8WinnerFlag = 2;
        u8CurrentNumber = 0;
      }
      else
      {
        u8CurrentNumber++;
        u8CurrentNumber++;  
        u8WinnerFlag = 1;
        if(u8CurrentPlayerIndex < 3)
        {
          u8CurrentPlayerIndex++;
        }
        else
        {
          u8CurrentPlayerIndex = 0;
        }
        u32TryConnectionTime = G_u32SystemTime1s;
        bOnlineFlag = FALSE;
      }
    }
    else if(AntReadAppMessageBuffer())
    {
      if(G_eAntApiCurrentMessageClass == ANT_DATA && G_au8AntApiCurrentData[WHO_SEND_MESSAGE] == u8CurrentPlayerIndex)
      {
        
        /*change the number according to what current player does*/
        switch(G_au8AntApiCurrentData[RESPONSE])
        {
          
          /*player chooses to plus 1*/
          case 0x01:
            if(u8CurrentNumber + 1 >= 30)
            {
              u8CurrentNumber = 30;
              u8WinnerFlag = 2;
              u8CurrentNumber = 0;
            }
            else
            {
              u8CurrentNumber++;
              u8WinnerFlag = 1;
              if(u8CurrentPlayerIndex < 3)
              {
                u8CurrentPlayerIndex++;
              }
              else
              {
                u8CurrentPlayerIndex = 0;
              }
              u32TryConnectionTime = G_u32SystemTime1s;
              bOnlineFlag = FALSE;
            }
            break;
            
          /*player chooses to plus 2 , or player fails to make a decision in required time(10s)*/
          case 0x02:
          case 0xff:
            if(u8CurrentNumber + 2 >= 30)
            {
              u8CurrentNumber = 30;
              u8WinnerFlag = 2;
              u8CurrentNumber = 0;
            }
            else
            {
              u8CurrentNumber++;
              u8CurrentNumber++;  
              u8WinnerFlag = 1;
              if(u8CurrentPlayerIndex < 3)
              {
                u8CurrentPlayerIndex++;
              }
              else
              {
                u8CurrentPlayerIndex = 0;
              }
              u32TryConnectionTime = G_u32SystemTime1s;
              bOnlineFlag = FALSE;
            }
            break;
          default:
            break;           
        }
      }
    }
  }
  
  if(u8WinnerFlag == 0)
  {
    UserApp_au8GameSituationMessage[WINNER] = 0x80;
  }
  if(u8WinnerFlag == 1)
  {
    UserApp_au8GameSituationMessage[WINNER] = 0xff;
  }
  if(u8WinnerFlag == 2)
  { 
    UserApp_au8GameSituationMessage[WINNER] = u8CurrentPlayerIndex;
  }
    

  if(u8CounterFor100ms == 100)
  {
    u8CounterFor100ms = 0;    
    UserApp_au8GameSituationMessage[CURRENT_PLAYER] = u8CurrentPlayerIndex;
    UserApp_au8GameSituationMessage[CURRENT_NUMBER] = u8CurrentNumber; 
    au8CurrentPlayerMessage[7] = u8CurrentPlayerIndex + 0x30;
    au8CurrentNumberMessage[18] = u8CurrentNumber / 10 + 0x30;
    au8CurrentNumberMessage[19] = u8CurrentNumber % 10 + 0x30;
    LCDMessage(LINE1_START_ADDR , au8CurrentPlayerMessage);
    LCDMessage(LINE2_START_ADDR , au8CurrentNumberMessage);
    AntQueueBroadcastMessage(0,UserApp_au8GameSituationMessage);  
    AntQueueBroadcastMessage(1,UserApp_au8GameSituationMessage);  
    AntQueueBroadcastMessage(2,UserApp_au8GameSituationMessage);  
    AntQueueBroadcastMessage(3,UserApp_au8GameSituationMessage);  
    UserApp_au8GameSituationMessage[DISCONNECTION_PLAYER] = 0x80;
    UserApp_au8GameSituationMessage[WINNER] = 0x80;
    u8WinnerFlag = 0;
  }
  
  /*when someone wins , referee should end the game*/
  if(u8WinnerFlag == 2 && u8CounterFor100ms == 0)
  {
    u8CurrentPlayerIndex = 0;
    bOnlineFlag = FALSE;
    u8WinnerFlag = 0;
    bNewGameFlag = TRUE;
    u8CurrentNumber = 0;
    u8CounterFor100ms = 0;
    UserApp_au8GameSituationMessage[CURRENT_PLAYER] = 0x80;
    UserApp_au8GameSituationMessage[CURRENT_NUMBER] = 0x80;
    UserApp_au8GameSituationMessage[WINNER] = 0x80;
    UserApp_au8GameSituationMessage[DISCONNECTION_PLAYER] = 0x80;
    UserApp_StateMachine = UserAppSM_EndGame;
  }
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
    LedOn(RED);
} /* end UserAppSM_Error() */
