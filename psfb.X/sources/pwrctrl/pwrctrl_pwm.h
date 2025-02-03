/**
 * @file pwrctrl_pwm.h     
 * @ingroup pwrctrl-pwm   
 * @brief Contains public functions for PWM.    
 */

#ifndef DRV_PWRCTRL_PWM_H
#define	DRV_PWRCTRL_PWM_H

#include "pwrctrl_typedef.h"
#include "pwrctrl/pwrctrl.h"


//extern PWM_DISTRIBUTION_t PhaseShiftDistribution;

 void PwrCtrl_PWM_Update(void);
//extern void PwrCtrl_PWM_Update(PWM_DISTRIBUTION_t* instance);

 void PwrCtrl_PWM_Enable(void);
 void PwrCtrl_PWM_Disable(void);
 void PwrCtrl_PWM_Stop_Switching(void);
 void PwrCtrl_PWM_Initialize(void);
 void PwrCtrl_PWM_SetDutyCyclePrimary(uint16_t dutycycle);

#endif	/* DRV_PWRCTRL_PWM_H */

