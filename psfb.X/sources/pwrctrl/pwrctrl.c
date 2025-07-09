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

/*******************************************************************************
 * @ingroup pwrctrl
 * @brief  Initialize the power control parameters
 * @return void
 * 
 * @details This function initialize the power control settings, 
 * start-up configuration and control loop configuration. 
 *******************************************************************************/
void PwrCtrl_Initialize(void){

    // Clear references
    psfb.Properties.VPriReference = 0;
    psfb.Properties.VSecReference = 0;
    psfb.Properties.IReference = 0;
    psfb.Properties.PwrReference = 0;


    // Initialize Start-Up ramp settings
    PwrCtrl_StartUpInitialize(); // ramp initialize

    // Initialize Power Control Loop
    PwrCtrl_ControlLoopInitialize(); //coefficents and SMPS_Controller2P2ZInitialize

    //TODO: ??

}
/*******************************************************************************
 * @ingroup pwrctrl
 * @brief  Executes the power control state machine
 * @return void
 * 
 * @details After initialization this function has to be called on a deterministic, 
 *  constant time base. Each execution step, this function will call the power control
 *  state machine.
 *********************************************************************************/
void PwrCtrl_Execute(void) {
    // Execute the state machine
    PwrCtrl_StateMachine(psfb_ptr);
}

/*******************************************************************************
 * @ingroup pwrctrl
 * @brief  Resets the power control properties
 * @return void
 * 
 * @details This function resets the power control properties and control loop references.
 *********************************************************************************/
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
        // 10 volts
        // for 10 volts
        // divided by 8 to keep it within 16bits
    psfb.Precharge.scaler = 10 * 2 * 17 * 54;
    psfb.Precharge.primaryVoltage = Dev_PwrCtrl_GetAdc_Vpri() - 205;
    //  divided by 8 as scaler is also divided by 8
    psfb.Precharge.primaryVoltage = psfb.Precharge.primaryVoltage >> 3; 

    psfb.Precharge.maxDutyCycle  = __builtin_divud(psfb.Precharge.scaler, psfb.Precharge.primaryVoltage);

    if (psfb.Precharge.maxDutyCycle > 80) 
        psfb.Precharge.maxDutyCycle  = 80;   // limit precharge to 80 percent dutycycle
}

/*******************************************************************************
 * @ingroup pwrctrl
 * @brief  reset offset data types
 * @return void
 * 
 * @details This function resets current sensor offsets
 *********************************************************************************/
void PwrCtrl_OffsetDatatypesInitalize(void){
    
    psfb.PrimaryCT_Offset.Counter = 0;
    psfb.PrimaryCT_Offset.AveragingCount = 63;
    psfb.PrimaryCT_Offset.AverageValue = 0;
    psfb.PrimaryCT_Offset.Counter = 0;
    
    psfb.SecondarySh_Offset.Counter = 0;
    psfb.SecondarySh_Offset.AveragingCount = 63;
    psfb.SecondarySh_Offset.AverageValue = 0;
    psfb.SecondarySh_Offset.Counter = 0;
    
    psfb.VoutCalibratingAveraging.Counter = 0;
    psfb.VoutCalibratingAveraging.AveragingCount = 63;
    psfb.VoutCalibratingAveraging.AverageValue = 0;
    psfb.VoutCalibratingAveraging.Counter = 0;
}