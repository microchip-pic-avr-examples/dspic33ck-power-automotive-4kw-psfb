/**
 * @file pwrctrl_pwm.h     
 * @ingroup pwrctrl-pwm   
 * @brief Contains public functions for PWM.    
 */

#ifndef DRV_PWRCTRL_PWM_H
#define	DRV_PWRCTRL_PWM_H

#include "pwrctrl_typedef.h"
#include "pwrctrl/pwrctrl.h"


 void PwrCtrl_PWM_Update(void);


 void PwrCtrl_PWM_Enable(void);
 void PwrCtrl_PWM_Disable(void);
 void PwrCtrl_PWM_Stop_Switching(void);
 void PwrCtrl_PWM_Initialize(void);
 void PwrCtrl_PWM_SetDutyCyclePrimary(uint16_t dutycycle);
 void PwrCtrl_PWM_UpdateSecondaryRectifiers(void);
#endif	/* DRV_PWRCTRL_PWM_H */

