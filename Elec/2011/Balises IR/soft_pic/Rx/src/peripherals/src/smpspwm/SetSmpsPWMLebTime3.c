#if defined(__dsPIC30F__) 
#include <p30fxxxx.h>
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#endif

#include "smpspwm.h"
#ifdef _ADCP0IF
#ifdef _PWM3IF 

/*************************************************************************
* Function Name     : SetSmpsPWMLebTime3
* Description       : This function configures the assignment of Leading edge blanking 
*                     units to rising edge or falling edge of PWM3 output                        
* Parameters        : unsigned int leb, or unsigned int leb1, leb2
* Return Value      : None 
**************************************************************************/
#ifdef _ADCP7IF
void SetSmpsPWMLebTime3(unsigned int leb1, unsigned int leb2 )
{
    LEBCON3 = leb1; 
    LEBDLY3 = leb2; 
}
#else
void SetSmpsPWMLebTime3(unsigned int leb)
{
    LEBCON3 = leb; 
}
#endif

#else
#warning "Does not build on this target"
#endif
#endif 
