/*
    (c) 2024 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
 */

/**
 * @file      pwrctrl.c
 * @ingroup   pwrctrl   
 * @brief     Contains power control initialization including control loop 
 *  initialization and start-up initialization, and the power control execution.
 */

#include <xc.h>

//MCC header files
#include "pwm_hs/pwm_hs_types.h"
#include "pwm_hs/pwm.h"

//PSFB header files
#include "config/macros.h"
#include "pwrctrl.h"
//#include "fault/fault.h"


/******************************************************************************
 * @ingroup pwrctrl
 * @brief Global data object for a PSFB Converter
 * 
 * @details The 'psfb' data object holds all status, control and monitoring values 
 * of the POWER_CONTROL_t power controller. 
 ******************************************************************************/
POWER_CONTROL_t psfb;    // Declare psfb converter data structure
POWER_CONTROL_t * psfb_ptr = &psfb;

// PRIVATE FUNCTIONS
static void PwrCtrl_StartUpInitialize(void);
static void PwrCtrl_ControlLoopInitialize(void);

//// PUBLIC FUNCTIONS
extern void PwrCtrl_StateMachine(POWER_CONTROL_t* pcInstance);


void PwrCtrl_Initialize(void){

    // Clear references
    psfb.Properties.VPriReference = 0;
    psfb.Properties.VSecReference = 0;
    psfb.Properties.IReference = 0;
    psfb.Properties.PwrReference = 0;


    // Initialize Start-Up ramp settings
    PwrCtrl_StartUpInitialize(); // ramp stuff

    // Initialize Power Control Loop
    PwrCtrl_ControlLoopInitialize(); //coefficents and SMPS_Controller2P2ZInitialize

    //TODO: ??

}

void PwrCtrl_Execute(void) {
    // Execute the state machine
    PwrCtrl_StateMachine(psfb_ptr);
}

void PwrCtrl_Reset(void){
    //set the period to maximum  

/*  psfb.Pwm.ControlPeriod = MAX_PWM_PERIOD;
    psfb.Pwm.ControlPhase = psfb.Pwm.DeadTimeLow;
    psfb.Pwm.PBVPeriodTarget = MAX_PWM_PERIOD;
    psfb.Pwm.PBVControlPhaseTarget = psfb.Pwm.DeadTimeLow; */
    
    // Reset the power control references
    psfb.Properties.VPriReference = 0;
    psfb.Properties.VSecReference = 0;
    psfb.Properties.IReference = 0;
    psfb.Properties.PwrReference = 0;

    // Initialize current loop reference to 0, to be controlled externally
    psfb.ILoop.Reference = 0;
    // Initialize power loop reference to 0, to be controlled externally
    psfb.PLoop.Reference = 0;
    // Initialize voltage loop reference to current secondary voltage
    psfb.VLoop.Reference = 0;
    
    // Set the AGC to 1
    psfb.VLoop.AgcFactor = 0x7FFF;
    psfb.ILoop.AgcFactor = 0x7FFF;
    psfb.PLoop.AgcFactor = 0x7FFF;
    
    // Reset Control Loop Histories
    PwrCtrl_ResetControlLoopHistories();     
}


/*******************************************************************************
 * @ingroup pwrctrl
 * @brief  Initialize the StartUp Ramp configuration
 * @return void
 * 
 * @details This function initializes the Ramp Up configuration for voltage, current
 * and power. For each parameters that would be ramped up, pointer to the reference,
 * reference target pointer, increment/decrement step size, delay for each increment/decremnt
 * counter for each execution need to be defined. When the reference becomes equal to 
 * the reference targe, the rampComplete data member will be set. 
 *********************************************************************************/
static void PwrCtrl_StartUpInitialize(void)
{
    // Initialize Voltage ramp-up settings
    psfb.VRamp.ptrReference = (uint16_t*)&psfb.VLoop.Reference;
    psfb.VRamp.ptrReferenceTarget = &psfb.Properties.VSecReference;
    psfb.VRamp.StepSize = 1;
    psfb.VRamp.Delay = 60;
    psfb.VRamp.Counter = 0;
    psfb.VRamp.RampComplete = 0;
    
    //Initialize Current ramp-up settings
    psfb.IRamp.ptrReference = (uint16_t*)&psfb.ILoop.Reference;
    psfb.IRamp.ptrReferenceTarget = (uint16_t*)&psfb.Properties.IReference;
    psfb.IRamp.StepSize = 1;
    psfb.IRamp.Delay = 60;
    psfb.IRamp.Counter = 0;
    psfb.IRamp.RampComplete = 0;
    
    // //Initialize Power ramp-up settings
    // psfb.PRamp.ptrReference = (uint16_t*)&psfb.PLoop.Reference;
    // psfb.PRamp.ptrReferenceTarget = &psfb.Properties.PwrReference;
    // psfb.PRamp.StepSize = 1;
    // psfb.PRamp.Delay = 4;
    // psfb.PRamp.Counter = 0;
    // psfb.PRamp.RampComplete = 0;
    
    #if defined (OPEN_LOOP_PBV) && (OPEN_LOOP_PBV == true)
    // The PWM Period bits [2:0] needs to be mask when using cascaded PWM setup 
    // (please refer to Section 4.1.3.3 in High Resolution PWM FRM)
    uint16_t PeriodMask = 0x7; 
    
    // Initialize Voltage ramp-up settings for Period control
    psfb.Pwm.ControlPeriod = psfb.Pwm.ControlPeriod & ~(PeriodMask);
    psfb.Pwm.PBVPeriodTarget = psfb.Pwm.PBVPeriodTarget & ~(PeriodMask);
    
    psfb.VRamp.ptrReference = &psfb.Pwm.ControlPeriod;
    psfb.VRamp.ptrReferenceTarget = &psfb.Pwm.PBVPeriodTarget;
    psfb.VRamp.StepSize = 0xE;
    psfb.VRamp.Delay = 0;
    
    //Initialize Current ramp-up settings for Phase control
    psfb.Pwm.ControlPhase = psfb.Pwm.ControlPhase & ~(PeriodMask);
    psfb.Pwm.PBVControlPhaseTarget = psfb.Pwm.PBVControlPhaseTarget & ~(PeriodMask);
    
    psfb.IRamp.ptrReference = &psfb.Pwm.ControlPhase;
    psfb.IRamp.ptrReferenceTarget = &psfb.Pwm.PBVControlPhaseTarget;
    psfb.IRamp.StepSize = 0xE;
    psfb.IRamp.Delay = 0;
    
#endif
}


/*******************************************************************************
 * @ingroup pwrctrl
 * @brief  Initializes the control loop
 * @return void
 * 
 * @details This function initializes the control loop necessary to run the close loop
 * operation of the converter. 
 *********************************************************************************/
void PwrCtrl_ControlLoopInitialize(void)
{
    //VCOMP_ControlObject_Initialize();

    // Initialize current loop compensator
    //PwrCtrl_IComp_Initialize();

    // Initialize voltage loop compensator
    PwrCtrl_VComp_Initialize();

    //Initialize the current loop 3p3z
    PwrCtrl_IComp_3p3z_Initialize();

    // Current loop properties initialize
    psfb.ILoop.Enable = false;
    psfb.ILoop.AgcFactor = 0x7FFF;
    psfb.ILoop.Feedback = 0;
    psfb.ILoop.Output = 0;
    psfb.ILoop.Reference = 0;

    // Voltage loop properties initialize 
    psfb.VLoop.Enable = false;
    psfb.VLoop.AgcFactor = 0x7FFF;
    psfb.VLoop.Feedback = 0;
    psfb.VLoop.Output = 0;
    psfb.VLoop.Reference = 0;
}
