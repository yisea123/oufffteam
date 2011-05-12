/*
*********************************************************************************************************
*                                         Association OufffTEAM
*                                     Coupe de France de Robotique
*
* File : LibMoving.h
*********************************************************************************************************
*/

#ifndef LIBMOVING_H
#define LIBMOVING_H 

#include "AppIncludes.h"
#include "mathf.h"
#include "math.h"

void LibMoving_MoveInMM(int Dist, INT8U Speed, StructCmd *NextSetpoint);
	// Dist         : in mm for the movement (>0 go forward, <0 go back)
    // Speed        : Speed to use 
	// NextSetpoint	: pointer to the struct for containing new position
	// MvtMode is set to 'MvtMode_Simple'

void LibMoving_RotateInDeg(float AngleInDeg, INT8U Speed, StructCmd *NextSetpoint);
	// AngleInDeg   : in mm for the movement (>0 go forward, <0 go back)
    // Speed        : Speed to use 
	// NextSetpoint	: pointer to the struct for containing new position
	// MvtMode is set to 'MvtMode_Simple'

void LibMoving_MoveToAngleInDeg(float AngleToGoInDeg, INT8U Speed, StructCmd *NextSetpoint);
	// AngleToGoInDeg	: angle in degree we want to go
    // Speed            : Speed to use 
	// NextSetpoint	    : pointer to the struct for containing new position
	// MvtMode is set to 'MvtMode_Simple'

void LibMoving_ComputeNewPath(StructCmd *ExpectedCmd, StructCmd *NewPath, INT8S *NewPathLength);
	// ExpectedCmd					: Command we have to reach 
	// NewPath						: All commands necessary for reaching ExpectedCmd
	// NewPathLength				: Nb of command defined in NewPath (0 means, no Path is available)

void LibMoving_CreateEscapeSeq(INT8U EscapeSeqType, INT8U Speed, StructCmd *NewPath, INT8S *NewPathLength);
	// EscapeSeqType				: Seq type to use for create escape seq
	//									- APP_MOVING_ESCAPE_SEQ_RIGHT
	//									- APP_MOVING_ESCAPE_SEQ_LEFT
	//									- APP_MOVING_ESCAPE_SEQ_BACK
	// Speed						: Speed to use for this escape sequence
	// NewPath						: All commands necessary for this escape seq
	// NewPathLength				: Nb of command defined in NewPath (0 means, no Path is available)

BOOLEAN LibMoving_IsSetpointReached(StructCmd *SetpointToTest);
    // *SetpointToTest              : Setpoint we have to test

#endif // LIBMOVING
