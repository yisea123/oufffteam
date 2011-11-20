#include "qei32.h"

#ifdef _QEI2IF

/*********************************************************************
* Function Name     : Write32bitQEI2Initialization()
* Description       : This function writes to initialization 
                      register
* Parameters        : Pointer to union qeiCounter, which has the
		      value to be written to initialization register
* Return Value      : None
*********************************************************************/

void Write32bitQEI2Initialization(qeiCounter *ptr)
{
	QEI2ICH  = ptr->f.hiWord;
	QEI2ICL  = ptr->f.loWord;
}

#else
#warning "Does not build on this target"
#endif
