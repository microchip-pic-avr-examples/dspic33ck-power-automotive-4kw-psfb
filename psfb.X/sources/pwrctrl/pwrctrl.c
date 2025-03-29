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
//#include "config/macros.h"
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
    psfb.VRamp.Delay = 4;
    psfb.VRamp.Counter = 0;
    psfb.VRamp.RampComplete = 0;
    
    //Initialize Current ramp-up settings
    psfb.IRamp.ptrReference = (uint16_t*)&psfb.ILoop.Reference;
    psfb.IRamp.ptrReferenceTarget = (uint16_t*)&psfb.Properties.IReference;
    psfb.IRamp.StepSize = 1;
    psfb.IRamp.Delay = 60;
    psfb.IRamp.Counter = 0;
    psfb.IRamp.RampComplete = 0;
       
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

/*******************************************************************************
 * @ingroup pwrctrl
 * @brief  Calculates the max duty cycle for precharge 
 * @return void
 * 
 * @details This function calculates the max duty cycle for precharge
 * based on input voltage
 *********************************************************************************/
void PwrCtrl_MaxDutyCycle(void){
               //10 volts
        // for 10 volts
    // divided by 8 to keep it within 16bits
    psfb_ptr->Precharge.scaler = 10 * 2 * 17 * 54;
    psfb_ptr->Precharge.primaryVoltage = Dev_PwrCtrl_GetAdc_Vpri() - 205;
    //divided by 8 as scaler is also divided by 8
    psfb_ptr->Precharge.primaryVoltage = psfb_ptr->Precharge.primaryVoltage >> 3; 

    psfb_ptr->Precharge.maxDutyCycle  = __builtin_divud(psfb_ptr->Precharge.scaler, psfb_ptr->Precharge.primaryVoltage);

    if (psfb_ptr->Precharge.maxDutyCycle > 80) 
        psfb_ptr->Precharge.maxDutyCycle  = 80;   // limit precharge to 80 percent dutycycle
}