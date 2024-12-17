
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

// MCC header files
#include "timer/sccp1.h"
#include "adc/adc1.h"
#include "system/pins.h"

// DAB header files
#include "config/macros.h"
#include "config/config.h"
#include "pwrctrl.h"
#include "fault/fault.h"

#include "pwrctrl/vcomp/VCOMP.h"   
#include "pwrctrl/pwrctrl_pwm.h"

// PRIVATE FUNCTIONS
void PwrCtrl_PrimToSecPHDegree(void);
void PwrCtrl_DeadTimeAdjust(void);
void PwrCtrl_PeriodModulator(void);
void PwrCtrl_UpdateADConverterData(void);
void PwrCtrl_ControlLoopExecute(void);
void PwrCtrl_10KHzVPLoopPrepareData(void);
uint16_t VCOMP_ControlObject_Initialize(void);

/*******************************************************************************
 * @ingroup pwrctrl-isr
 * @brief  Executes the power converter control loop
 * @return void
 * 
 * @details This interrupt function is a timing interrupt executed every 100KHz
 *   and calls an ADC function that updates the DAB data member with ADC raw values. 
 *   The fault check is also called in this function to detect any fault events.
 *   It is followed by the control loop execution for Voltage, Current and Power
 *   Loop Controllers. The end result of the control loop is handed over to the 
 *   PWM distribution. 
 *********************************************************************************/
void ControlLoop_Interrupt_CallBack(void)
{      
    // GPIO_1_SetHigh();
    
    // Update the ADC data member
    PwrCtrl_UpdateADConverterData();
    
    // Execute the fault detection
    Fault_Execute();
    
    
    // Enable the ADC sampling
    ADC1_SoftwareTriggerEnable();

    
    //dutycycle updates for 2 and 4
    if (VCOMP.status.bits.enabled) 
    {
        // GPIO_1_SetLow();
        VCOMP_PTermUpdate(&VCOMP);                         // Call control loop
        PwrCtrl_PWM_Update(&PhaseShiftDistribution);
//        PG4DC = PG4PER - PG1TRIGC;
//        PG2DC = PG2PER - PG1TRIGC;
//        PG1STATbits.UPDREQ = 1;
    }
}



volatile uint16_t VCOMP_ControlObject_Initialize(void)
{
    volatile uint16_t retval = 0;                 // Auxiliary variable for function call verification (initially set to ZERO = false)

    /* Controller Input and Output Ports Configuration */

    // fixed point number accumulator saturation behaviour : keep saturated instead of rolling over
    CORCONbits.ACCSAT = 0;
    CORCONbits.SATA = 1;
    CORCONbits.SATB = 1;
    
    // Configure Controller Primary Input Port
    VCOMP.Ports.Source.ptrAddress = &ADCBUF2; // Pointer to primary feedback source (e.g. ADC buffer register or variable)
/*     VCOMP.Ports.Source.Offset = <signed int>;     // Primary feedback signal offset
    VCOMP.Ports.Source.NormScaler = <signed int>; // Primary feedback normalization factor bit-shift scaler
    VCOMP.Ports.Source.NormFactor = <signed int/Q15 fractional>; // Primary feedback normalization factor fractional */

    // Configure Controller Primary Output Port
    VCOMP.Ports.Target.ptrAddress = &PG1TRIGC; // Pointer to primary output target (e.g. SFR register or variable)
/*     VCOMP.Ports.Target.Offset = <signed int>;     // Primary output offset value
    VCOMP.Ports.Target.NormScaler = <signed int>; // Primary output normalization factor bit-shift scaler
    VCOMP.Ports.Target.NormFactor = <signed int/Q15 fractional>; // Primary output normalization factor fractional */

    // Configure Control Reference Port
    VCOMP.Ports.ptrControlReference = &psfb_ptr->VLoop.Reference; // Pointer to control reference (user-variable)

    /* Controller Output Limits Configuration */

    // Primary Control Output Limit Configuration
    VCOMP.Limits.MinOutput = 0;        // Minimum control output value
    VCOMP.Limits.MaxOutput = 9576;        // Maximum control output value

    /* ADC Trigger Positioning Configuration */

    // ADC Trigger A Control Configuration
    VCOMP.ADCTriggerControl.ptrADCTriggerARegister = &PG1TRIGA; // Pointer to ADC trigger A register
    // VCOMP.ADCTriggerControl.ADCTriggerAOffset = <unsigned int>; // user-defined trigger delay (
    // for current it matters.

    /* Advanced Parameter Configuration */

    // Initialize User Data Space Buffer Variables
    VCOMP.Advanced.usrParam0 = 0;                 // No additional advanced control options used
    VCOMP.Advanced.usrParam1 = 0;                 // No additional advanced control options used
    VCOMP.Advanced.usrParam2 = 0;                 // No additional advanced control options used
    VCOMP.Advanced.usrParam3 = 0;                 // No additional advanced control options used
    VCOMP.Advanced.usrParam4 = 0;                 // No additional advanced control options used
    VCOMP.Advanced.usrParam5 = 0;                 // No additional advanced control options used
    VCOMP.Advanced.usrParam6 = 0;                 // No additional advanced control options used
    VCOMP.Advanced.usrParam7 = 0;                 // No additional advanced control options used

    /* Controller Status Word Configuration */

    VCOMP.status.bits.enabled = false;            // Keep controller disabled

    // Call Assembler Control Library Initialization Function
    retval = VCOMP_Initialize(&VCOMP);            // Initialize controller data arrays and number scalers

    return(retval);

}