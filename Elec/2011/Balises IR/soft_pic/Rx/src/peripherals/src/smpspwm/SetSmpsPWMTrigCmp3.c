#if defined(__dsPIC30F__) 
#include <p30fxxxx.h>
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#endif

#include "smpspwm.h"
#ifdef _ADCP0IF
#ifdef _PWM3IF 



/*************************************************************************
* Function Name     : SetSmpsPWMTrigCmp3
* Description       : This function configures the Trigger compare value  for generating trigger to the 
*                     ADC module for initiating a sample and conversion process or generating 
*                     trigger interrupt to PWM3 output pair
*					  if defined(__dsPIC33F__)
*					  This function configures the Trigger compare value  for generating trigger to the 
*                     ADC module for initiating a sample and conversion process or generating 
*                     trigger interrupt to PWM3H output pair                       
* Parameters        : unsigned int trigcmp_val
* Return Value      : None 
**************************************************************************/

void SetSmpsPWMTrigCmp3(unsigned int trigcmp_val )
{
    TRIG3 = trigcmp_val ; 
	
}
#else
#warning "Does not build on this target"
#endif
#endif 
