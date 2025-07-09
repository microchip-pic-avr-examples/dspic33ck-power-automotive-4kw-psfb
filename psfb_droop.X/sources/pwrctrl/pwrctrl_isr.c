
/**
 * @file      pwrctrl_isr.c
 * @ingroup   pwrctrl-isr   
 * @brief     Contains Control loop interrupt Callback that acquires the ADC raw 
 *  data and process it in the control loop, and use the control output for the 
 *  PWM distribution for this converter
 */

#include <xc.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>


#include "adc/adc1.h"
#include "system/pins.h"

#include "pwrctrl.h"
#include "fault/fault.h"

#include "pwrctrl/pwrctrl_pwm.h"


void PwrCtrl_UpdateADConverterData(void);
void PwrCtrl_ControlLoopExecute(void);
void PwrCtrl_DroopAverage(void);

/*******************************************************************************
 * @ingroup pwrctrl-isr
 * @brief  Executes the power converter control loop
 * @return void
 * 
 * @details This callback is executed every 100Khz. and is tied up to the ADC0 
 * conversion ISR. The ADC0 itself is triggererd by half of the phase difference
 * (half of the duty) between fixed and phase shifted legs
 *********************************************************************************/
void ControlLoop_Interrupt_CallBack(void)
{      
    PwrCtrl_ControlLoopExecute();

}
