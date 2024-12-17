
/**
 * @file      pwrctrl_pwm.c
 * @ingroup   pwrctrl-pwm   
 * @brief     Contains DAB control phase calculation between primary and 
 *  secondary, and the PWM distribution.
 */

#include <xc.h> 
#include <stdbool.h>
#include "pwm_hs/pwm.h"
#include "pwrctrl_typedef.h"
#include "pwrctrl_pwm.h"

PWM_DISTRIBUTION_t PhaseShiftDistribution;

/*******************************************************************************
 * @ingroup pwrctrl-pwm
 * @brief  PWM distribution for DAB converter
 * @param  pcInstance  Pointer to a power control data object of type POWER_CONTROL_t
 * @return void
 * 
 * @details This function distributes the calculated control phase for the 
 *  half bridges of the DAB converter. PWM1 and PWM3 drives the primary half 
 *  bridges and PWM2 and PWM4 drives the secondary half bridges.
 *********************************************************************************/
//void PwrCtrl_PWM_Update(POWER_CONTROL_t* pcInstance)
//{   
///*     // The PWM Period bits [2:0] needs to be mask when using cascaded PWM setup 
//    // (please refer to Section 4.1.3.3 in High Resolution PWM FRM)
//    uint16_t PeriodMask = 0x7; 
//    
//    // Mask the calculated frequency bits [2:0] to make the cascaded/synchronous
//    // PWM scheme reliable (please refer to Section 4.1.3.3 in High Resolution PWM FRM)
//    pcInstance->Pwm.ControlPeriod = pcInstance->Pwm.ControlPeriod & ~(PeriodMask);
//    
//    // Calculate Duty Cycle for 50%
//    pcInstance->Pwm.ControlDutyCycle = (pcInstance->Pwm.ControlPeriod >> 1);
//    
//    // Maximum Clamping for control phase
//    if(pcInstance->Pwm.ControlPhase > pcInstance->Pwm.ControlPeriod){
//        pcInstance->Pwm.ControlPhase = pcInstance->Pwm.ControlPeriod;
//    }
//
//    // Calculate primary to secondary phase as half of the control phase
//    uint16_t PrimarySecondaryPhase = (pcInstance->Pwm.ControlPhase >> 1);
//    
//    // Compensate the added Dead-time 
//    PrimarySecondaryPhase += pcInstance->Pwm.DeadTimeLow >> 1;
//    
//    // Calculate the Bridge Delay ((Frequency / 2) - Primary to Secondary Phase + Control Phase)
//    // Note that in the cascaded PWM, the reference phase of the client PWM, is its trigger source
//    uint16_t PrimaryPhaseDelay = (pcInstance->Pwm.ControlDutyCycle - PrimarySecondaryPhase) + 
//            pcInstance->Pwm.ControlPhase;
//    
//    // Set the PWM trigger with the calculated PWM phases
//    PWM_TriggerCCompareValueSet(PWM_PRI_1, PrimarySecondaryPhase);
//    PWM_TriggerCCompareValueSet(PWM_SEC_1, PrimaryPhaseDelay);
//    PWM_TriggerCCompareValueSet(PWM_PRI_2, PrimarySecondaryPhase);
//
//    // Set the PWM Duty Cycle at 50% with the given Frequency
//    PWM_DutyCycleSet(PWM_PRI_1, pcInstance->Pwm.ControlDutyCycle);
//    PWM_DutyCycleSet(PWM_SEC_1, pcInstance->Pwm.ControlDutyCycle);
//    PWM_DutyCycleSet(PWM_PRI_2, pcInstance->Pwm.ControlDutyCycle);
//    PWM_DutyCycleSet(PWM_SEC_2, pcInstance->Pwm.ControlDutyCycle);
//    
//    // Set the PWM Frequency
//    PWM_PeriodSet(PWM_PRI_1, pcInstance->Pwm.ControlPeriod);
//    PWM_PeriodSet(PWM_SEC_1, pcInstance->Pwm.ControlPeriod);
//    PWM_PeriodSet(PWM_PRI_2, pcInstance->Pwm.ControlPeriod);
//    PWM_PeriodSet(PWM_SEC_2, pcInstance->Pwm.ControlPeriod);
//
//    // Set the PWM Low DeadTime
//    PWM_DeadTimeLowSet(PWM_PRI_1, pcInstance->Pwm.DeadTimeLow);
//    PWM_DeadTimeLowSet(PWM_SEC_1, pcInstance->Pwm.DeadTimeLow);
//    PWM_DeadTimeLowSet(PWM_PRI_2, pcInstance->Pwm.DeadTimeLow);
//    PWM_DeadTimeLowSet(PWM_SEC_2, pcInstance->Pwm.DeadTimeLow);
//    
//    // Set the PWM High DeadTime
//    PWM_DeadTimeHighSet(PWM_PRI_1, pcInstance->Pwm.DeadTimeHigh);
//    PWM_DeadTimeHighSet(PWM_SEC_1, pcInstance->Pwm.DeadTimeHigh);
//    PWM_DeadTimeHighSet(PWM_PRI_2, pcInstance->Pwm.DeadTimeHigh);
//    PWM_DeadTimeHighSet(PWM_SEC_2, pcInstance->Pwm.DeadTimeHigh);
//    
//    // Set the Update bit of the last PWM in the cascaded approach to broadcast
//    // it to the other PWMs
//    PWM_SoftwareUpdateRequest(PWM_SEC_2); */
//    
//}

/*******************************************************************************
 * @ingroup pwrctrl-pwm
 * @brief  Enable the PWM output
 * @param  pcInstance  Pointer to a power control data object of type POWER_CONTROL_t 
 * @return void
 * 
 * @details This function turn on the physical PWM outputs by clearing the
 *  output override of the PWM module. 
 *********************************************************************************/
void PwrCtrl_PWM_Enable(void)
{      
    // Turn-On PWM outputs by disabling the output override 
    // on each high and low PWM output
//    PWM_OverrideHighDisable(PWM_PRI_1);
//    PWM_OverrideHighDisable(PWM_SEC_1);
//    PWM_OverrideHighDisable(PWM_PRI_2);
//    PWM_OverrideHighDisable(PWM_SEC_2);
//           
//    PWM_OverrideLowDisable(PWM_PRI_1);
//    PWM_OverrideLowDisable(PWM_SEC_1);
//    PWM_OverrideLowDisable(PWM_PRI_2);
//    PWM_OverrideLowDisable(PWM_SEC_2); 
    
    // Set update request of the last PWM in the cascade 
    // to update all PWM registers
    
    // temp
    
    PG1CONLbits.ON = 1;
    PG3CONLbits.ON = 1;
//    PWM_SoftwareUpdateRequest(PWM_SEC_2);

}


/*******************************************************************************
 * @ingroup pwrctrl-pwm
 * @brief  Disable the PWM output
 * @param  pcInstance  Pointer to a power control data object of type POWER_CONTROL_t
 * @return void
 * 
 * @details This function disable the physical PWM output by setting the override
 *  bits of the PWM module.
 *********************************************************************************/
void PwrCtrl_PWM_Disable(void)
{   
    // Turn-Off PWM outputs by enabling the output override 
    // on each high and low PWM output
//    PWM_OverrideHighEnable(PWM_PRI_1);
//    PWM_OverrideHighEnable(PWM_SEC_1);
//    PWM_OverrideHighEnable(PWM_PRI_2);
//    PWM_OverrideHighEnable(PWM_SEC_2);
//           
//    PWM_OverrideLowEnable(PWM_PRI_1);
//    PWM_OverrideLowEnable(PWM_SEC_1);
//    PWM_OverrideLowEnable(PWM_PRI_2);
//    PWM_OverrideLowEnable(PWM_SEC_2); 
    
    // Set update request of the last PWM in the cascade 
    // to update all pwm registers
    PG1CONLbits.ON = 0;
    PG3CONLbits.ON = 0;
    PG2CONLbits.ON = 0;
    PG4CONLbits.ON = 0;
//    PWM_SoftwareUpdateRequest(PWM_SEC_2);
    
}

void PwrCtrl_PWM_Initialize(void)
{
    /*
     PG1 is the leading PWM channel
     PG3 is triggered by PG1, where PG3 is synchronized to PG1 including updates
     PG4 is sync rectifier of PWM1H+PWM3L and is also synchronized to PG1
     PG2 is sync rectifier of PWM3H+PWM1L and is synchronized to PG3
     All updates are triggered by the PG1 broadcast
     All updates are performed at the cycle boundary (EOC)
     PG2 and PG4 only use PWMxL output
     PG2 and PG4 are swapped, hence, their respective HIGH signal is used
     Sync Rectifier edge placement is tuned by PGxDTH and PGxDC of PG2 and PG4,
          where the duty cycle = PER - PHASE-SHIFT +/- EDGEPLACEMENT 
     */
  
    // Set base register of PWM channels
    PG1CONL = 0x0088; // CLKSEL=1, HREN=1, ON=0
    PG2CONL = 0x0088; // CLKSEL=1, HREN=1, ON=0
    PG3CONL = 0x0088; // CLKSEL=1, HREN=1, ON=0
    PG4CONL = 0x0088; // CLKSEL=1, HREN=1, ON=0
    
    // Set synchronization of PWM start and register update
    PG1CONH = 0x0800; // SOCS=0, MSTEN=1
    PG2CONH = 0x0243; // SOCS=3, TRGMOD=1, UPDMOD=2
    PG3CONH = 0x0241; // SOCS=1, TRGMOD=1, UPDMOD=2
    PG4CONH = 0x0241; // SOCS=1, TRGMOD=1, UPDMOD=2

    // Set I/O Configuration
    PG1IOCONL = 0x3000; // OVRENH/L=1
    PG1IOCONH = 0;      // PENH/L=0
    PG2IOCONL = 0x7000; // SWAP=1, OVRENH=1=1, OVRENL=1
    PG2IOCONH = 0;      // PENH/L=0
    PG3IOCONL = 0x3000; // OVRENH/L=1
    PG3IOCONH = 0;      // PENH/L=0
    PG4IOCONL = 0x7000; // SWAP=1, OVRENH=1=1, OVRENL=1
    PG4IOCONH = 0;      // PENH/L=0
    
    // Set PG synchronization triggers
    PG1EVTL = 0; // clear register
    PG1EVTLbits.PGTRGSEL = 0b011; // use PGxTRIGC to set phase shift
    PG1EVTLbits.UPDTRG = 0; // requires UPDREQ bit to be set manually
    PG1EVTLbits.ADTR1EN1 = 1; // enable ADC trigger 1 => TRIGA
    PG1EVTH = 0; // disable all triggers
    PG1EVTHbits.ADTR2EN2 = 1; // enable ADC trigger 2 => TRIGB
    
    PG2EVTL = 0; // disable all triggers
    PG2EVTH = 0; // disable all triggers
    PG3EVTLbits.PGTRGSEL = 0b011; // use PGxTRIGC to set phase shift
    PG3EVTLbits.UPDTRG = 0; // requires UPDREQ bit to be set manually
    PG3EVTH = 0; // disable all triggers
    PG4EVTL = 0; // disable all triggers
    PG4EVTH = 0; // disable all triggers
    
    // Set Period
    PG1PER = 40000;
    PG2PER = 40000;
    PG3PER = 40000;
    PG4PER = 40000;

    // Set Duty Cycle
    PG1DC = 20000;
    PG3DC = 20000;
    
    // Set dead times
    PG1DTH = 640;   // set dead time to 160ns
    PG1DTL = 640;   // set dead time to 160ns
    PG2DTH = 0;     // no dead time on PWMxH
    PG2DTL = 640;  // set dead time to 160ns
    PG3DTH = 640;   // set dead time to 160ns
    PG3DTL = 640;   // set dead time to 160ns
    PG4DTH = 0;     // no dead time on PWMxH
    PG4DTL = 640;  // set dead time to 160ns

    // Set synchronization triggers
    PG1TRIGC = 0; // phase shift = 0
    PG3TRIGC = PG1DC; // phase shift = duty cycle of leading PWM channel
    PG2DC = (PG1PER - PG1TRIGC - PG2DTL);
    PG4DC = (PG1PER - PG1TRIGC - PG4DTL);
    
    // Configure the PWM value distribution data structure
    PhaseShiftDistribution.PhaseShift = 0;  // Clear output value of phase shift
    PhaseShiftDistribution.ptrPhaseShift = &PG1TRIGC;
    PhaseShiftDistribution.ptrPeriod = &PG1PER;
    PhaseShiftDistribution.ptrDCSRL = &PG4DC;
    PhaseShiftDistribution.ptrDCSRR = &PG2DC;
    PhaseShiftDistribution.ptrUpdateReg = &PG1STAT;
    
}



