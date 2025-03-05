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
 * @file      pwrctrl_sm.c
 * @ingroup   pwrctrl-sm   
 * @brief     Contains power control state machine functions that is 
 *  executed every 100us.
 */

#include <xc.h>
#include <stdint.h> // include standard integer data types
#include <stdbool.h> // include standard boolean data types
#include <stddef.h> // include standard definition data types
#include <stdlib.h> // for abs function, see how it works
                               

// PSFB header files
#include "pwrctrl.h"
#include "config/macros.h"
#include "device/device.h"
#include "fault/fault.h"
#include "config/config.h"
#include "pwrctrl_pwm.h"
#include "system/pins.h"


//PRIVATE FUNCTIONS
static void PCS_INIT_handler(POWER_CONTROL_t* pcInstance);
static void PCS_WAIT_IF_FAULT_ACTIVE_handler(POWER_CONTROL_t* pcInstance);
static void PCS_STANDBY_handler(POWER_CONTROL_t* pcInstance);
static void PCS_SOFT_START_handler(POWER_CONTROL_t* pcInstance);
static void PCS_UP_AND_RUNNING_handler(POWER_CONTROL_t* pcInstance);
static void PCS_START_CONTROL_handler(POWER_CONTROL_t* pcInstance);
static void PCS_PRECHARGE_handler(POWER_CONTROL_t* pcInstance);

/*******************************************************************************
 * @ingroup pwrctrl-sm
 * @brief  Manages the power control state machine
 * @param  pcInstance  Pointer to a power control data object of type POWER_CONTROL_t
 * @return void
 * 
 * @details This function manages the state machine of the converter. There are

 * 
 *********************************************************************************/
void PwrCtrl_StateMachine(POWER_CONTROL_t* pcInstance)
{ 
    switch (pcInstance->State)
    {
        case PWRCTRL_STATE_INITIALIZE:
            PCS_INIT_handler(pcInstance);
            break;    

        case PWRCTRL_STATE_FAULT_DETECTION:
            PCS_WAIT_IF_FAULT_ACTIVE_handler(pcInstance);
            break;

        case PWRCTRL_STATE_STANDBY:
            PCS_STANDBY_handler(pcInstance);
            break;

        case PWRCTRL_STATE_START_CONTROL:
            PCS_START_CONTROL_handler(pcInstance);
            break;

        case PWRCTRL_STATE_SOFT_START:
            PCS_SOFT_START_handler(pcInstance);
            break;

        case PWRCTRL_STATE_UP_AND_RUNNING:
            PCS_UP_AND_RUNNING_handler(pcInstance);
            break;
            
        case PWRCTRL_STATE_PRECHARGE:
            PCS_PRECHARGE_handler(pcInstance);
            break;

        default:
            pcInstance->State = PWRCTRL_STATE_INITIALIZE;
            break;      
    }
}

/*******************************************************************************
 * @ingroup pwrctrl-sm
 * @brief  Executes function for initialze state machine
 * @param  pcInstance  Pointer to a power control data object of type POWER_CONTROL_t
 * @return void
 * 
 * @details This function resets the conditional flag bits, ensures PWM output is
 *  disabled and run the initial current calibration offset. After this, the 
 *  state machine moves to checking the fault handler.
 *********************************************************************************/
static void PCS_INIT_handler(POWER_CONTROL_t* pcInstance)
{
    // Current sensor calibration is complete. Update the offset of the current sensor
    // pcInstance->Data.ISecSensorOffset = Dev_CurrentSensor_Get_Offset();
    
    // Ensure PWM output is disabled
    PwrCtrl_PWM_Disable();
    
    //Disable short circuit for now  -> Temporary should go in fault set/reset
    FAULT_EN_SetLow();
    
    // Reset fault objects status bits
    Fault_Reset();
    
    // Clear power control fault active bit
    pcInstance->Status.bits.FaultActive = 0;
    // Clear power control running bit
    pcInstance->Status.bits.Running = 0;
    // Clear power control enable bit
    pcInstance->Properties.Enable = 0;
    
    // place holders
//    pcInstance->IRamp.RampComplete = true;
//    pcInstance->VRamp.RampComplete = true;
//    pcInstance->PRamp.RampComplete = true;
//    
    
    Dev_LED_Off(LED_BOARD_GREEN);
    Dev_LED_Off(LED_BOARD_RED);

    //add delay of some ms to for values to be stable

    if (dev_AreOffsetsCalculated() == 1){
        pcInstance->Data.ISecSensorOffset = dev_Get_SecondaryShuntOffset();
        pcInstance->Data.IPriSensorOffset = dev_Get_PrimaryCTOffset();
        
        //34 amps
        pcInstance->SecRec.Threshold_high = pcInstance->Data.ISecSensorOffset + 421;
        //30amps
        pcInstance->SecRec.Threshold_low = pcInstance->Data.ISecSensorOffset + 372;

        pcInstance->State = PWRCTRL_STATE_PRECHARGE;

        //  float temp = (float)39100 / (float)(Dev_PwrCtrl_GetAdc_Vpri() - 205);
        //  pcInstance->Precharge.maxDutyCycle = (uint16_t)temp >> 4 ; 

        FAULT_EN_SetHigh();
    }
    else {
        dev_MeasureOffsets();
    }

    //pcInstance->State = PWRCTRL_STATE_PRECHARGE;
    
}

/*******************************************************************************
 * @ingroup pwrctrl-sm
 * @brief  Executes the fault handler state machine
 * @param  pcInstance  Pointer to a power control data object of type POWER_CONTROL_t
 * @return void
 * 
 * @details This function checks if there is fault event that occurred. When
 *  there is no fault event, the state machine moves to StandBy state. 
 * 
 *********************************************************************************/
static void PCS_WAIT_IF_FAULT_ACTIVE_handler(POWER_CONTROL_t* pcInstance)
{   
    if (pcInstance->Fault.FaultDetected == 0)
    {
        pcInstance->Status.bits.FaultActive = 0;
        pcInstance->State = PWRCTRL_STATE_PRECHARGE; // next state
    }
    
        // Check for fault event 
    if (pcInstance->Fault.FaultDetected)
    {
        // Clear power control enable bit
        pcInstance->Properties.Enable = 0;
        
        // State back to STATE_FAULT_DETECTION
        pcInstance->State = PWRCTRL_STATE_FAULT_DETECTION;
    }
    
    Dev_LED_Blink(LED_BOARD_RED);
    Dev_LED_Off(LED_BOARD_GREEN);
}

/*******************************************************************************
 * @ingroup pwrctrl-sm
 * @brief  Executes Standby State machine
 * @param  pcInstance  Pointer to a power control data object of type POWER_CONTROL_t
 * @return void
 * 
 * @details This function waits until there is no fault event that has occurred 
 *  and when the power control enable bit is set. When Enable bit is set,  
 *  reset the fault objects status bits, reset PWM control settings, enable
 *  the power control running bit, enable PWM physical output, initialize 
 *  control loop references and then move to the next state STATE_SOFT_START. 
 * 
 * @note    In this application the power control enable bit is controlled 
 *  externally by Power Board Visualizer.  
 * 
 *********************************************************************************/
static void PCS_PRECHARGE_handler(POWER_CONTROL_t* pcInstance)
{
    // Check for fault event 
    if (pcInstance->Fault.FaultDetected)
    {
        // Clear power control enable bit
        pcInstance->Properties.Enable = 0;
        
        // State back to STATE_FAULT_DETECTION
        pcInstance->State = PWRCTRL_STATE_FAULT_DETECTION;
        return;     //TODO: fix state machine and fault jumps
    }
    //    // NOTE: Power control enable is controlled externally 
   else if (pcInstance->Properties.Enable)
   {
       // Reset fault objects status bits
       Fault_Reset();
           
       // Reset the power control properties and control loop histories
       //PwrCtrl_Reset();

       // Update PWM distribution
//        PwrCtrl_PWM_Update(&psfb);

       // Enable PWM physical output
       //PwrCtrl_PWM_Enable();

       // Enable power control running bit
       pcInstance->Status.bits.Running = 1;

       // override precharge 
       pcInstance->Precharge.PrechargeEnabled = 0 ;

       // Next State assigned to STATE_SOFT_START
       pcInstance->State = PWRCTRL_STATE_STANDBY;
       
   }

   else if (pcInstance->Precharge.PrechargeEnabled == 1) {
        pcInstance->Precharge.maxDutyCycle = 42;        //TODO: fix maths. and state machine 
        if (pcInstance->Precharge.DutyCycle < pcInstance->Precharge.maxDutyCycle) {
            pcInstance->Precharge.delayCounter = pcInstance->Precharge.delayCounter + 1;
            if (pcInstance->Precharge.delayCounter > 499){                       //5ms each step increment
                Nop();
                pcInstance->Precharge.delayCounter = 0;
                pcInstance->Precharge.DutyCycle = pcInstance->Precharge.DutyCycle + 1;
                }
            } 
            PwrCtrl_PWM_SetDutyCyclePrimary(pcInstance->Precharge.DutyCycle);
            pcInstance->State = PWRCTRL_STATE_PRECHARGE;
   }
    // else if (pcInstance->Precharge.PrechargeEnabled == 1) {
    //     //Disable Iloop here - if we go back to precharge state 
    //     //psfb_ptr->ILoop.Enable = 0;
    //     pcInstance->Precharge.maxDutyCycle = 63;        //TODO: fix maths. and state machine 
    //     if (pcInstance->Precharge.precharged == 1) {
    //         pcInstance->State = PWRCTRL_STATE_STANDBY;
    //     } else {
    //         if (pcInstance->Precharge.DutyCycle < pcInstance->Precharge.maxDutyCycle) {
    //             pcInstance->Precharge.delayCounter = pcInstance->Precharge.delayCounter + 1;
    //             if (pcInstance->Precharge.delayCounter > 499){                       //5ms each step increment
    //                 Nop();
    //                 pcInstance->Precharge.delayCounter = 0;
    //                 pcInstance->Precharge.DutyCycle = pcInstance->Precharge.DutyCycle + 1;
    //             }
    //         } 
    //         PwrCtrl_PWM_SetDutyCyclePrimary(pcInstance->Precharge.DutyCycle);
    //         pcInstance->State = PWRCTRL_STATE_PRECHARGE;
    //     }
    //     // at 12v Vcap is around 11.5 * 191.131 = 2196
    //     // at 7.5v Vcap is around 7.5 * 191.131 = 1433.4825
    //     if (Dev_PwrCtrl_GetVoltage_Vcap() > 1433) {
    //         pcInstance->Precharge.precharged = 1;
    //         pcInstance->Precharge.DutyCycle = 0;
    //         PwrCtrl_PWM_SetDutyCyclePrimary(pcInstance->Precharge.DutyCycle);
    //         pcInstance->State = PWRCTRL_STATE_STANDBY;
    //         PwrCtrl_PWM_Stop_Switching();
    //     }
    // }
}
//    float temp = (float)39100 / (float)(Dev_PwrCtrl_GetAdc_Vpri() - 205);
//    pcInstance->Precharge.maxDutyCycle = (uint16_t)temp >> 4 ; 
//}


/*******************************************************************************
 * @ingroup pwrctrl-sm
 * @brief  Executes Standby State machine
 * @param  pcInstance  Pointer to a power control data object of type POWER_CONTROL_t
 * @return void
 * 
 * @details This function waits until there is no fault event that has occurred 
 *  and when the power control enable bit is set. When Enable bit is set,  
 *  reset the fault objects status bits, reset PWM control settings, enable
 *  the power control running bit, enable PWM physical output, initialize 
 *  control loop references and then move to the next state STATE_SOFT_START. 
 * 
 * @note    In this application the power control enable bit is controlled 
 *  externally by Power Board Visualizer.  
 * 
 *********************************************************************************/
static void PCS_STANDBY_handler(POWER_CONTROL_t* pcInstance)
{
    // Check for fault event 
    if (pcInstance->Fault.FaultDetected)
    {
        // Clear power control enable bit
        pcInstance->Properties.Enable = 0;
        
        // State back to STATE_FAULT_DETECTION
        pcInstance->State = PWRCTRL_STATE_FAULT_DETECTION;
    }
    
    // NOTE: Power control enable is controlled externally 
    else if (pcInstance->Properties.Enable)
    {
        // Reset fault objects status bits
        // Fault_Reset();
            
        // Reset the power control properties and control loop histories
        // PwrCtrl_Reset();

        // Update PWM distribution
//        PwrCtrl_PWM_Update(&psfb);



        // Enable power control running bit
        pcInstance->Status.bits.Running = 1;

        // Next State assigned to STATE_SOFT_START
        pcInstance->State = PWRCTRL_STATE_SOFT_START;
        
        // Enable PWM physical output
        // PwrCtrl_PWM_Enable();

        // enable SR flag to be controlled by current
        psfb_ptr->SecRec.SR_Enabled = 1;
        //Enable Iloop here
        psfb_ptr->ILoop.Enable = 1;
        //Enable Vloop here
        psfb_ptr->VLoop.Enable = 1;

        //reference set to precharged caps on startup
        psfb_ptr->VLoop.Reference =             psfb_ptr->Data.VOutVoltage;
        psfb_ptr->Properties.VSecReference =    psfb_ptr->Data.VOutVoltage;
    }
}

/*******************************************************************************
 * @ingroup pwrctrl-sm
 * @brief  Executes the power control soft start state machine
 * @param  pcInstance  Pointer to a power control data object of type POWER_CONTROL_t
 * @return void
 * 
 * @details This function gradually ramps up the references of the power control.
 *  The control loop references are gradually incremented until in reached the 
 *  desired power control reference. When this is achieved, the next state will  
 *  be assigned to STATE_ONLINE. 
 * 
 *********************************************************************************/
static void PCS_SOFT_START_handler(POWER_CONTROL_t* pcInstance)
{
    // Check for fault event 
    if (pcInstance->Fault.FaultDetected)
    {
        // Clear power control enable bit
        pcInstance->Properties.Enable = 0;
        
        // State back to STATE_FAULT_DETECTION
        pcInstance->State = PWRCTRL_STATE_FAULT_DETECTION;
    }
  
    // Check if Enable bit has been cleared
    else if (!pcInstance->Properties.Enable) 
    {
        // Disable PWM physical output
        //PwrCtrl_PWM_Disable();
        PwrCtrl_PWM_Stop_Switching();
        
        // Clear power control running bit
        pcInstance->Status.bits.Running = 0;
        
        // State back to STATE_STANDBY
        pcInstance->State = PWRCTRL_STATE_STANDBY; 
    }

     else
    {   
        // Ramp Up the Voltage, Current and Power reference
        uint16_t rampComplete = PwrCtrl_RampReference(&pcInstance->VRamp);  
        

//        uint16_t rampComplete = PwrCtrl_RampReference(&pcInstance->IRamp);
//        rampComplete &= PwrCtrl_RampReference(&pcInstance->IRamp);
//        rampComplete &= PwrCtrl_RampReference(&pcInstance->PRamp);
//        rampComplete &= PwrCtrl_RampReference(&pcInstance->PhRamp);
        
        //removing softstart for open loop
        
        //uint16_t rampComplete = PwrCtrl_RampReference(&pcInstance->PhRamp);
        // Check if ramp up is complete
        //uint16_t rampComplete  = true;
        if (rampComplete)
            // Next State assigned to STATE_ONLINE
            pcInstance->State = PWRCTRL_STATE_UP_AND_RUNNING; 
    }
}

/*******************************************************************************
 * @ingroup pwrctrl-sm
 * @brief  Executes the Online state
 * @param  pcInstance  Pointer to a power control data object of type POWER_CONTROL_t
 * @return void
 * 
 * @details This function keeps checks if there is fault event that occurred,
 *  if power control Enable has been disabled and if there is changes in the 
 *  power control references. 
 *********************************************************************************/
static void PCS_UP_AND_RUNNING_handler(POWER_CONTROL_t* pcInstance)
{    
    // Check for fault event 
    if (pcInstance->Fault.FaultDetected)
    {
        // Clear power control enable bit
        pcInstance->Properties.Enable = 0;
        
        // State back to STATE_FAULT_DETECTION
        pcInstance->State = PWRCTRL_STATE_FAULT_DETECTION;
    }
    
    else if (!pcInstance->Properties.Enable) 
        // Check if Enable bit has been cleared
        {
            // Disable PWM physical output
            PwrCtrl_PWM_Disable();

            // Clear power control running bit
            pcInstance->Status.bits.Running = 0;

            // State back to STATE_INITIALIZE
            pcInstance->State = PWRCTRL_STATE_INITIALIZE; 
            Dev_LED_Off(LED_BOARD_GREEN);
        }      
        //removing it for now
        
//    #if defined (OPEN_LOOP_PBV) && (OPEN_LOOP_PBV == true)
//        else if (pcInstance->Pwm.ControlPhase != pcInstance->Pwm.PBVControlPhaseTarget)
//            pcInstance->State = PWRCTRL_STATE_SOFT_START;
//    #endif    
        
    // Check if there is change in power control references    
        
    else if (pcInstance->VLoop.Reference != pcInstance->Properties.VSecReference)
//     else if ((pcInstance->ILoop.Reference != pcInstance->Properties.IReference) ||
//                (pcInstance->VLoop.Reference != pcInstance->Properties.VSecReference) ||
//                (pcInstance->PLoop.Reference != pcInstance->Properties.PwrReference))
     {    
            // State back to STATE_SOFT_START
            pcInstance->State = PWRCTRL_STATE_SOFT_START;
     }

} 


/*******************************************************************************
 * @ingroup pwrctrl-sm
 * @brief  Executes Standby State machine
 * @param  pcInstance  Pointer to a power control data object of type POWER_CONTROL_t
 * @return void
 * 
 * @details This function waits until there is no fault event that has occurred 
 *  and when the power control enable bit is set. When Enable bit is set,  
 *  reset the fault objects status bits, reset PWM control settings, enable
 *  the power control running bit, enable PWM physical output, initialize 
 *  control loop references and then move to the next state STATE_SOFT_START. 
 * 
 * @note    In this application the power control enable bit is controlled 
 *  externally by Power Board Visualizer.  
 * 
 *********************************************************************************/
static void PCS_START_CONTROL_handler(POWER_CONTROL_t* pcInstance)
{
    // Check for fault event 
    if (pcInstance->Fault.FaultDetected)
    {
        // Clear power control enable bit
        pcInstance->Properties.Enable = 0;
        
        // State back to STATE_FAULT_DETECTION
        pcInstance->State = PWRCTRL_STATE_FAULT_DETECTION;
    }
    
    // NOTE: Power control enable is controlled externally 
    else if (pcInstance->Properties.Enable)
    {
        // Reset fault objects status bits
        // Fault_Reset();
            
        // Reset the power control properties and control loop histories
        // PwrCtrl_Reset();

        // Update PWM distribution
//        PwrCtrl_PWM_Update(&psfb);

        // Enable PWM physical output
        //PwrCtrl_PWM_Enable();

        // Enable power control running bit
        pcInstance->Status.bits.Running = 1;

        // Next State assigned to STATE_SOFT_START
        pcInstance->State = PWRCTRL_STATE_SOFT_START;
        
    }     // Check if Enable bit has been cleared
    else if (!pcInstance->Properties.Enable) 
    {
        // Disable PWM physical output
        //PwrCtrl_PWM_Disable();
        PwrCtrl_PWM_Stop_Switching();
        
        // Clear power control running bit
        pcInstance->Status.bits.Running = 0;
        
        // State back to STATE_STANDBY
        pcInstance->State = PWRCTRL_STATE_STANDBY; 
    }
}