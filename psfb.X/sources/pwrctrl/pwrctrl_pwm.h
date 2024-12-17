/**
 * @file pwrctrl_pwm.h     
 * @ingroup pwrctrl-pwm   
 * @brief Contains public functions for PWM.    
 */

#ifndef DRV_PWRCTRL_PWM_H
#define	DRV_PWRCTRL_PWM_H

#include "pwrctrl_typedef.h"

void PwrCtrl_PWM_Update(POWER_CONTROL_t* pcInstance);
void PwrCtrl_PWM_Enable(void);
void PwrCtrl_PWM_Disable(void);
struct PWM_DISTRIBUTION_s {
    uint16_t PhaseShift;        // target buffer of the control output value to be distributed across PWM registers
    uint16_t* ptrPeriod;        // Pointer to the leading PWM channel period register
    uint16_t* ptrPhaseShift;    // Pointer to leading PWM channel trigger register setting the phase shift between right- and left-leg
    uint16_t* ptrDCSRR;         // Pointer to Duty Cycle register of the right-leg synchronous rectifier
    uint16_t* ptrDCSRL;         // Pointer to Duty Cycle register of the left-leg synchronous rectifier
    uint16_t* ptrUpdateReg;     // Pointer to Status register to set the UPDATE_REQUEST bit
}__attribute__((aligned));
typedef struct PWM_DISTRIBUTION_s PWM_DISTRIBUTION_t;

extern PWM_DISTRIBUTION_t PhaseShiftDistribution;

//extern void PwrCtrl_PWM_Update(POWER_CONTROL_t* pcInstance);
extern void PwrCtrl_PWM_Update(PWM_DISTRIBUTION_t* instance);

#endif	/* DRV_PWRCTRL_PWM_H */

