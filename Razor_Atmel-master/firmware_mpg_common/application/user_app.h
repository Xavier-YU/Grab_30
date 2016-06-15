/**********************************************************************************************************************
File: user_app.h                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app as a template:
1. Follow the instructions at the top of user_app.c
2. Use ctrl-h to find and replace all instances of "user_app" with "yournewtaskname"
3. Use ctrl-h to find and replace all instances of "UserApp" with "YourNewTaskName"
4. Use ctrl-h to find and replace all instances of "USER_APP" with "YOUR_NEW_TASK_NAME"
5. Add #include yournewtaskname.h" to configuration.h
6. Add/update any special configurations required in configuration.h (e.g. peripheral assignment and setup values)
7. Delete this text (between the dashed lines)
----------------------------------------------------------------------------------------------------------------------

Description:
Header file for yournewtaskname.c

**********************************************************************************************************************/

#ifndef __USER_APP_H
#define __USER_APP_H

/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/


/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/
/*the definition of ANT_DATA*/
#define WHO_SEND_MESSAGE      (u8)0
#define CONTROL               (u8)1
#define RESPONSE              (u8)2
#define CURRENT_PLAYER        (u8)3
#define CURRENT_NUMBER        (u8)4
#define WINNER                (u8)5
#define DISCONNECTION_PLAYER  (u8)6

/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/
void UserAppInitialize(void);
void UserAppRunActiveState(void);


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void UserAppSM_Idle(void);    
static void UserAppSM_IdentificationInitialize(void);
static void UserAppSM_Error(void);         
static void UserAppSM_FailedInit(void);  
static void UserAppSM_WaitOpenChannel(void);
static void UserAppSM_OpenChannel(void);
static void UserAppSM_AssignChannel(void);
static void UserAppSM_WaitForPairing(void);
static void UserAppSM_LaunchGame(void);
static void UserAppSM_EndGame(void);
static void UserAppSM_WaitGameStart(void);
static void UserAppSM_MyTurn(void);
static void UserAppSM_Congratulation(void);
     
#endif /* __USER_APP_H */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
