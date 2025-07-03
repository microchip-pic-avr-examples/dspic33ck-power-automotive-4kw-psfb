
/**
 * @file      pwrctrl_pwm.c
 * @ingroup   pwrctrl-pwm   
 * @brief     Contains PSFB control phase calculation between primary and 
 *  secondary, and the PWM distribution.
 */

#include <xc.h> 
#include <stdbool.h>
#include "pwm_hs/pwm.h"
#include "pwrctrl_typedef.h"
#include "pwrctrl_pwm.h"

#define DEAD_TIME_ADJ   240

/*******************************************************************************
 * @ingroup pwrctrl-pwm
 * @brief  PWM distribution for PSFB converter
 * @return void
 * 
 * @details This function distributes the calculated control phase for the 
 *  half bridges of the PSFB converter. PWM1 and PWM3 drives the primary half 
 *  bridges and PWM2 and PWM4 drives the secondary rectifiers
 *********************************************************************************/
void PwrCtrl_PWM_Update()
{   
    if (psfb_ptr->ILoop.Enable == 0){
        psfb_ptr->Pwm.ControlPhase = psfb_ptr->Pwm.PBVControlPhaseTarget * 112;      // could add a ramp here as well :/
    } else {
        psfb_ptr->Pwm.ControlPhase = psfb_ptr->Pwm.PBVControlPhaseTarget; 
    }
      
    PWM_TriggerCCompareValueSet(PWM_PRI_1, psfb_ptr->Pwm.ControlPhase);
    
    PWM_TriggerBCompareValueSet(PWM_PRI_1, (psfb_ptr->Pwm.ControlPhase>>1) );        // ADC primary trigger
    
    PWM_DutyCycleSet(PWM_SEC_2, PG4PER - psfb_ptr->Pwm.ControlPhase);
    PWM_DutyCycleSet(PWM_SEC_1, PG2PER - psfb_ptr->Pwm.ControlPhase);
    
    PWM_SoftwareUpdateRequest(PWM_PRI_1);     
}

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
    // Start PWM
    PG4CONLbits.ON = 1;
    PG2CONLbits.ON = 1;
    PG3CONLbits.ON = 1;
    PG1CONLbits.ON = 1;
    
    PCLKCONbits.MCLKSEL = 1;
    PCLKCONbits.DIVSEL = 0;
    
    int timeout = 0;
    
    while ((0 == PCLKCONbits.HRRDY) && (++timeout < 5000));
    if (timeout < 5000)
    {
        PG1STATbits.UPDREQ = 1; // make sure all PWM channels are updated before enabling outputs
    
        // Hand over pin control to PWM module
        PG1IOCONHbits.PENH = 1;
        PG1IOCONHbits.PENL = 1;
        PG2IOCONHbits.PENH = 0; // not used
        PG2IOCONHbits.PENL = 1;
        PG3IOCONHbits.PENH = 1;
        PG3IOCONHbits.PENL = 1;
        PG4IOCONHbits.PENH = 0; // not used
        PG4IOCONHbits.PENL = 1;
        
        for (int i=1000; i>0; i--); // short delay
        
        // enable PWM channel outputs by clearing override bits
        PG1IOCONLbits.OVRENH = 0;
        PG1IOCONLbits.OVRENL = 0;
        
        PG2IOCONLbits.OVRENH = 1; // not used
        //PG2IOCONLbits.OVRENL = 0;
        PG2IOCONLbits.OVRENL = 1; // temporary.
        
        PG3IOCONLbits.OVRENH = 0;
        PG3IOCONLbits.OVRENL = 0;
        
        PG4IOCONLbits.OVRENH = 1; // not used
        //PG4IOCONLbits.OVRENL = 0;
        PG4IOCONLbits.OVRENL = 1;
    }    
    
}


/*******************************************************************************
 * @ingroup pwrctrl-pwm
 * @brief  Disable the PWM output
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
    PG1IOCONLbits.OVRENH = 1;
    PG1IOCONLbits.OVRENL = 1;
    PG2IOCONLbits.OVRENH = 1;
    PG2IOCONLbits.OVRENL = 1;
    PG3IOCONLbits.OVRENH = 1;
    PG3IOCONLbits.OVRENL = 1;
    PG4IOCONLbits.OVRENH = 1;
    PG4IOCONLbits.OVRENL = 1;
//    PWM_SoftwareUpdateRequest(PWM_SEC_2);
    
}


/*******************************************************************************
 * @ingroup pwrctrl-pwm
 * @brief  Initialize the PWM peripheral
 * @return void
 * 
 * @details This function configures additional parameters that are not configurable
 * using MCC graphical interface
 *********************************************************************************/
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
    PG2DTL = 640 + DEAD_TIME_ADJ;  // set dead time to 160ns
    PG3DTH = 640;   // set dead time to 160ns
    PG3DTL = 640;   // set dead time to 160ns
    PG4DTH = 0;     // no dead time on PWMxH
    PG4DTL = 640 + DEAD_TIME_ADJ;  // set dead time to 160ns

    // Set synchronization triggers
    PG1TRIGC = 0; // phase shift = 0
    PG3TRIGC = PG1DC; // phase shift = duty cycle of leading PWM channel
    PG2DC = (PG1PER - PG1TRIGC - PG2DTL);
    PG4DC = (PG1PER - PG1TRIGC - PG4DTL);
    
}


/*******************************************************************************
 * @ingroup pwrctrl-pwm
 * @brief  Disable the PWM output
 * @return void
 * 
 * @details This function disables the physical PWM output at the primary side by 
 * setting the override bits of the PWM modules PG1 and PG3.
 *********************************************************************************/

void PwrCtrl_PWM_Stop_Switching(void){
    PG1IOCONLbits.OVRENH = 1;
    PG1IOCONLbits.OVRENL = 1;
    PG3IOCONLbits.OVRENH = 1;
    PG3IOCONLbits.OVRENL = 1;
}


/*******************************************************************************
 * @ingroup pwrctrl-pwm
 * @brief  set the PWM output to particular duty cycle. 
 * @param duty cycle in percent
 * @return void
 * 
 * @details This function disables the physical PWM output at the primary side by 
 * setting the override bits of the PWM modules PG1 and PG3.
 *********************************************************************************/

void PwrCtrl_PWM_SetDutyCyclePrimary(uint16_t dutycycle){
    // max phase shift 20000; 
    // 1 percent = 20000 / 200; 
    uint16_t phaseValue = dutycycle * 200;    
    PWM_TriggerCCompareValueSet(PWM_PRI_1, phaseValue);
    PWM_TriggerBCompareValueSet(PWM_PRI_1, phaseValue>>1);
    PWM_SoftwareUpdateRequest(PWM_PRI_1);   
}


/*******************************************************************************
 * @ingroup pwrctrl-pwm
 * @brief  set the PWM output to particular duty cycle. 
 * @param void
 * @return void
 * 
 * @details This function independently controls the Secondary rectifiers
 * by checking agains the CCM DCM conditions
 *********************************************************************************/

void PwrCtrl_PWM_UpdateSecondaryRectifiers (void) {
    if (psfb_ptr->SecRec.SR_Enabled == 0){
        return;
    }

    if (psfb_ptr->SecRec.SR_Flag == 0) 
    {
        //threshhold = 970;
        
        if (psfb_ptr->Data.ISenseSecondary > psfb_ptr->SecRec.Threshold_high) {       //approx 34 amps Iout
            //debug_SetHigh();
            psfb_ptr->SecRec.SR_Flag = 1;
             PG4IOCONLbits.OVRENL = 0;       //turning on SR
             PG2IOCONLbits.OVRENL = 0;
        } 
    } else {
        //threshhold = 950;
        if (psfb_ptr->Data.ISenseSecondary < psfb_ptr->SecRec.Threshold_low) {       // approx 26 amps Iout
            //debug_SetLow();
            psfb_ptr->SecRec.SR_Flag  = 0;
             PG4IOCONLbits.OVRENL = 1;       //turning off SR
             PG2IOCONLbits.OVRENL = 1;
        }
    }
}