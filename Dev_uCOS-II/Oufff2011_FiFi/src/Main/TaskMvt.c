/*
*********************************************************************************************************
*                                         Association OufffTEAM
*                                     Coupe de France de Robotique
*
*
* File : TaskMvt.c
*
* Suivi de version :
* 2011-04-01 | PBE | Creation de la version de base pour la coupe 2010
*********************************************************************************************************
*/

#include "TaskMvt.h"
#include "AppGlobalVars.h"

#define TASK_MVT_FLAGS_TO_READ	(APP_PARAM_APPFLAG_TIMER_STATUS + APP_PARAM_APPFLAG_START_BUTTON + APP_PARAM_APPFLAG_ALL_SENSORS)

// ------------------------------------------------------------------------------------------------
// TaskMvt_Main()
// ------------------------------------------------------------------------------------------------
void TaskMvt_Main(void *p_arg)
{
	// Vars
	StructCmd	    CurrentPath[APP_MOVING_SEQ_LEN];		// Data used for storing path orders
    StructCmd       CurrentCmd;                             // Data for storing current order (to be done)
	StructPos	    CurrentPos;		    					// Data used for storing current pos from TaskOdo
	OS_FLAGS		CurrentFlag;							// Var to read current flag								
	INT8U			CurrentState;							// Var used for storing current state for state machine
	INT8U			NextState;								// Var used for storing next state for state machine
	INT8U			Err;									// Var to get error status								
    INT8S           CurrentSetpoint;                        // Pointer to Current setpoint to reach
    unsigned int    LastMainCmdId;                          // Var to store last command received from TaskMain
    unsigned int    NextAsserCmdId;                         // Var to store next command to send to TaskAsser

	#ifdef _TARGET_440H
		char Debug_State[4];
	#endif

	// Init
	memset(CurrentPath,         0, APP_MOVING_SEQ_LEN * sizeof(StructCmd));
    memset(&CurrentCmd,       0, 1 * sizeof(StructCmd));
	memset(&CurrentPos,         0, 1 * sizeof(StructPos));
	CurrentFlag			= 0;
	CurrentState		= 0;
	NextState			= 0;
	Err					= 0;
    CurrentSetpoint     = -1;
    LastMainCmdId       = 0;
    NextAsserCmdId      = 0;
					
	#ifdef _TARGET_440H
		memset(Debug_State, 0, 4*sizeof(char));
	#endif

	putsUART2("OUFFF TEAM 2011 : Mvt online\n");

	// MAIN LOOP ==================================================================================
	do
	{
		// Proc Release
		OSTimeDly(10);

		// Update current state
		CurrentState = NextState;

		// Check FLAGS for MvtTask
		CurrentFlag = OSFlagAccept(AppFlags, TASK_MVT_FLAGS_TO_READ, OS_FLAG_WAIT_SET_ANY, &Err);


		#ifdef _TARGET_440H
			sprintf(Debug_State, "%03d", CurrentState);
			Set_Line_Information( 1, 0, Debug_State, 3);
			OSTimeDlyHMSM(0, 0, DELAY_S_BETWEEN_NEXT_STATE, DELAY_MS_BETWEEN_NEXT_STATE);
		#endif

		// State machine
		switch(CurrentState)
		{
			// CASE 000 ---------------------------------------------------------------------------
			case 0:		// Init state
				// Check for 'start button' status
				if((CurrentFlag & APP_PARAM_APPFLAG_START_BUTTON) != 0)
				{
					// Start button has been pressed
					NextState = 1;
				}
				else
				{
					// Start button is still inactive
					NextState = 0;

                    // Place here code for executing Cmd (if necessary)
				}
				break;

			// CASE 001 ---------------------------------------------------------------------------
			case 1:		// Read Timer flag
				if((CurrentFlag & APP_PARAM_APPFLAG_TIMER_STATUS) == 0)
					NextState = 255;		// Time is up
				else
					NextState = 2;			// Time is running

				break;

			// CASE 002 ---------------------------------------------------------------------------
			case 2:		// Read Msg from QMvt Queue
				//Check Last CmdID used, if a new msg is ready, use it
				if(App_CmdToTaskMvtId > LastMainCmdId)										
				{	
                    // Ask for Mutex
                    OSMutexPend(App_MutexCmdToTaskMvt, WAIT_FOREVER, &Err);
	                {	
                        // Get current Cmd
		                memcpy(&CurrentCmd, &App_CmdToTaskMvt, sizeof(StructCmd));
	                }	
	                OSMutexPost(App_MutexCmdToTaskMvt);

                    // Update last CmdID used
                    LastMainCmdId = App_CmdToTaskMvtId;

                    // Analyse Cmd
                    switch(CurrentCmd.Cmd)
                    {
                    case Mvt_Stop:
                        NextState = 254;
                        break;

                    case Mvt_UseAngleOnly:
                    case Mvt_Simple:
                    case Mvt_UseDistOnly:
                    case Mvt_UseMixedMode:
                    case Mvt_UsePivotMode:
                        NextState = 253;
                        break;

                    default:
                        // Invalid Command
                        NextState = 3;
                        break;
                    }
				}
				else
				{	// There is no msg available
					NextState = 3;
				}

				break;

			// CASE 003 ---------------------------------------------------------------------------
			case 3:		// Read Bumpers Status
				if((CurrentFlag & APP_PARAM_APPFLAG_ALL_SENSORS) == 0)
				{	// All bumpers are inative
					NextState = 4;
				}
				else
				{	// There is one or more sensors active
					// Which sensors are active
					if((CurrentFlag & APP_PARAM_APPFLAG_FRONT_SENSORS) == 0)
					{	// Front sensors are activated
						NextState = 5;
					}
					else if((CurrentFlag & APP_PARAM_APPFLAG_LEFT_SENSORS) == 0)
					{	// Left sensors are activated
						NextState = 7;
					}
					else if((CurrentFlag & APP_PARAM_APPFLAG_RIGHT_SENSORS) == 0)
					{	// Right sensors are activated
						NextState = 8;
					}
					else if((CurrentFlag & APP_PARAM_APPFLAG_BACK_SENSORS) == 0)
					{	// Back sensors are activated
						NextState = 6;
					}
					else
					{	// There is no expected sensors activated
						NextState = 4;
					}
				}
				break;

			// CASE 004 ---------------------------------------------------------------------------
			case 4:		// Read Mvt Flag
				// Read Current Pos
				if(AppGetCurrentPos(&CurrentPos) == ERR__NO_ERROR)
                {   // Check CurrentState Value
                    if(CurrentPos.CurrentState == CURRENT_STATE__STOP)
                        NextState = 9;
                    else
                        NextState = 0;
                }
                else
                {   // We are unable to read current pos
                    NextState = 0;
                }
				break;

			// CASE 005 ---------------------------------------------------------------------------
			case 5:		// Escape Seq (Front)
				// Ask for stopping Mvt
				NextState = 254;
				break;

			// CASE 006 ---------------------------------------------------------------------------
			case 6:		// Escape Seq (Back)
				// Ask for stopping Mvt
				NextState = 254;
				break;

			// CASE 007 ---------------------------------------------------------------------------
			case 7:		// Escape Seq (Right)
				// Ask for stopping Mvt
				NextState = 254;
				break;

			// CASE 008 ---------------------------------------------------------------------------
			case 8:		// Escape Seq (Left)
				// Ask for stopping Mvt
				NextState = 254;
				break;

			// CASE 009 ---------------------------------------------------------------------------
			case 9:		// Is temporary setpoint reached ?
                // Check if we have a setpoint to reach
                if((CurrentSetpoint >= 0) && (LibMoving_IsSetpointReached(CurrentPath + CurrentSetpoint) == OS_TRUE))
                {   
                    // Setpoint has been reached
                    NextState = 10;
                }
                else
                {
                    NextState = 0;
                }
 				break;

			// CASE 010 ---------------------------------------------------------------------------
			case 10:	// Is final setpoint reached ?
                // Check if we have a setpoint to reach
                if(LibMoving_IsSetpointReached(&CurrentCmd) == OS_TRUE)
                {
                    // Setpoint has been reached
                    NextState = 254;
                }
                else
                {   // Search for next setpoint
                    NextState = 11;
                }
				break;

			// CASE 011 ---------------------------------------------------------------------------
			case 11:	// Does next setpoint exist ?
				if(CurrentSetpoint > 0)
                {   // Load next Setpoint
                    NextState = 12;
                }
                else
                {   // There is no other setpoint in memory, We have to compute a new path
                    NextState = 253;
                }
				break;

			// CASE 012 ---------------------------------------------------------------------------
			case 12:	// Use next setpoint 
                CurrentSetpoint--;
				break;

			// CASE 253 ---------------------------------------------------------------------------
			case 253:	// Compute new traj
				// Todo
				NextState = 0;
				break;

			// CASE 254 ---------------------------------------------------------------------------
			case 254:	// Ask for stopping mvt
				// Todo
				NextState = 0;
				break;

			// CASE 255 ---------------------------------------------------------------------------
			case 255:	// End
				// Todo
				// Ask for stopping action if Mvt Moving Flag is set
				break;

			// DEFAULT ----------------------------------------------------------------------------
			default: // Undefined state
				// Current state is undefined. Clear current path and ask for stopping mvt
				memset(CurrentPath, 0, APP_MOVING_SEQ_LEN * sizeof(StructCmd));
				NextState = 254;		// Ask for Stopping action
				break;
		}
	
	// We loop until timer is set
	}while(CurrentState != 255);
	// ===============================================================================================

	// Time is up : Wait for ever
	while(OS_TRUE)
	{	
		OSTimeDlyHMSM(1, 0, 0, 0);		
	}

	return;
}
