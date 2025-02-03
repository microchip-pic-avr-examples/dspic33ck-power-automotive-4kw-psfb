
/**
 * @file      fault.c
 * @ingroup   fault  
 * @brief     Contains fault functions including the fault handler, fault 
 *  object initialization and fault execution. 
 */

#include "xc.h"
#include <stdbool.h>
#include <stddef.h>

//#include "cmp/cmp1.h"
#include "cmp/cmp3.h"
#include "pwm_hs/pwm.h"
#include "system/pins.h"

//#include "config/macros.h"
#include "device/device.h"
#include "pwrctrl/pwrctrl.h"
#include "fault_common.h"
#include "fault.h"
#include "fault_comm_interface.h"

#include "pwrctrl/pwrctrl_pwm.h"

//defining macros here for now
#define VPRI_OV_THRES_TRIG_V            890
#define VPRI_OV_THRES_CLEAR_V           800
#define VPRI_UV_THRES_TRIG_V            250     // TODO::sys Default 400V
#define VPRI_UV_THRES_CLEAR_V           240     // TODO::sys Default 390V

#define VPRI_OV_THRES_TRIG_ADC          (((VPRI_OV_THRES_TRIG_V) * 4.329) + 205) 
#define VPRI_UV_THRES_TRIG_ADC          (((VPRI_UV_THRES_TRIG_V) * 4.329) + 205) 
#define VPRI_OV_THRES_CLEAR_ADC         (((VPRI_OV_THRES_CLEAR_V) * 4.329) + 205) 
#define VPRI_UV_THRES_CLEAR_ADC         (((VPRI_UV_THRES_CLEAR_V) * 4.329) + 205) 

#define VSEC_OV_THRES_TRIG_V            18
#define VSEC_OV_THRES_CLEAR_V           11
#define VSEC_UV_THRES_TRIG_V            8
#define VSEC_UV_THRES_CLEAR_V           10

#define VSEC_OV_THRES_TRIG_ADC          ((VSEC_OV_THRES_TRIG_V) * 191.131)
#define VSEC_OV_THRES_CLEAR_ADC         ((VPRI_OV_THRES_CLEAR_V) * 191.131)
#define VSEC_UV_THRES_TRIG_ADC          ((VSEC_UV_THRES_TRIG_V) * 191.131)  
#define VSEC_UV_THRES_CLEAR_ADC         ((VPRI_UV_THRES_CLEAR_V) * 191.131) 

#define ISEC_OC_THRES_TRIG_A            210
#define ISEC_OC_THRES_CLEAR_A           180

#define ISEC_OC_THRES_TRIG_ADC          ISEC_OC_THRES_TRIG_A + 0        //TODO: fix multipliers
#define ISEC_OC_THRES_CLEAR_ADC         ISEC_OC_THRES_CLEAR_A + 0       // placeholders

#define FAULT_VPRI_OV   true
#define FAULT_VPRI_UV   true
#define FAULT_VSEC_OV   true
#define FAULT_VSEC_UV   true

#define FAULT_ISEC_OC   false
#define FAULT_IPRI_OC   false
#define FAULT_PS_OTP    false
#define FAULT_SHORT_CCT false
#define FAULT_VRAIL_5V  false
#define LOAD_DISCONNECT false



// PRIVATE FUNCTIONS
static void Fault_EnableShortCircuitProtection(void);
static void Fault_PrimaryOverCurrent_EventHandler(void);
bool loadDisconnect = false;


/*******************************************************************************
 * @ingroup fault
 * @brief   Initialize the fault objects
 * @return void
 * 
 * @details This function initializes the fault objects following each respective
 *  fault thresholds. 
 *********************************************************************************/
void Fault_Initialize(void)
{
    // // Initialize Primary Over Current Protection
    // FAULT_Init(&psfb_ptr->Fault.Object.IPrimaryOCP, IPRI_OC_THRES_TRIG, 
    //         IPRI_OC_THRES_CLEAR, IPRI_OC_T_BLANK_TRIG, IPRI_OC_T_BLANK_CLEAR); 
    
    // Initialize Secondary Over Current Protection
    FAULT_Init(&psfb_ptr->Fault.Object.ISecondaryOCP, ISEC_OC_THRES_TRIG_ADC, 
            ISEC_OC_THRES_CLEAR_ADC, 1, 1);  
    
    // Initialize Primary Over Voltage Protection
    FAULT_Init(&psfb_ptr->Fault.Object.VPrimaryOVP, VPRI_OV_THRES_TRIG_ADC, 
            VPRI_OV_THRES_CLEAR_ADC, 1, 1);   
    
    // Initialize Primary Under Voltage Protection
    FAULT_Init(&psfb_ptr->Fault.Object.VPrimaryUVP, VPRI_UV_THRES_TRIG_ADC, 
            VPRI_UV_THRES_CLEAR_ADC, 1, 1);   
    
    // Initialize Secondary Over Voltage Protection
    FAULT_Init(&psfb_ptr->Fault.Object.VSecondaryOVP, VSEC_OV_THRES_TRIG_ADC, 
            VSEC_OV_THRES_CLEAR_ADC, 1, 1);
    
    // Initialize Secondary Under Voltage Protection
    FAULT_Init(&psfb_ptr->Fault.Object.VSecondaryUVP, VSEC_UV_THRES_TRIG_ADC, 
            VSEC_UV_THRES_CLEAR_ADC, 1, 1);
    
    // // Initialize 5V Rail instability Protection
    // FAULT_Init(&psfb_ptr->Fault.Object.VRail_5V, VRAIL_5V_UV_THRES_TRIG, 
    //         VRAIL_5V_UV_THRES_CLEAR, VRAIL_5V_UV_T_BLANK_TRIG, VRAIL_5V_UV_T_BLANK_CLEAR);
    
//    // Initialize Over Temperature Protection
//    FAULT_Init(&psfb_ptr->Fault.Object.PowerSupplyOTP, OTP_THRES_TRIG,         
//            OTP_THRES_CLEAR, FAULT_PERSISTENCE_COUNT_TEMP, FAULT_PERSISTENCE_COUNT_TEMP); 
//    
#if defined (FAULT_SHORT_CCT) && (FAULT_SHORT_CCT == true)
    // Initialize short circuit fault protection with comparators
    Fault_EnableShortCircuitProtection();
#endif 
    
    CMP3_EventCallbackRegister(Fault_PrimaryOverCurrent_EventHandler);
    
}

/*******************************************************************************
 * @ingroup fault
 * @brief   Executes the fault handlers
 * @return void
 * 
 * @details This function evaluates if any of the fault objects has been tripped. 
 *  When fault detection occurs, the power converter will shutdown thus turn-off
 *  the power converter. 
 *********************************************************************************/
void Fault_Execute(void)
{
    uint16_t faultCheck = 0;
   // secondary over current fault handler
   #if defined(FAULT_ISEC_OC) && (FAULT_ISEC_OC ==  true)      
   faultCheck = FAULT_CheckMax(&psfb_ptr->Fault.Object.ISecondaryOCP, psfb_ptr->Data.ISenseSecondary, &Fault_Handler);
   #endif 
//    
//    // secondary over voltage fault handler
////    #if defined(FAULT_VSEC_OV) && (FAULT_VSEC_OV ==  true)            
////    faultCheck &= FAULT_CheckMax(&psfb_ptr->Fault.Object.VSecondaryOVP, psfb_ptr->Data.VSecVoltage, &Fault_Handler);
////    #endif    
//    
//    // primary over current fault handler
//    #if defined (FAULT_IPRI_OC) && (FAULT_IPRI_OC ==  true)
//    faultCheck &= FAULT_CheckMax(&psfb_ptr->Fault.Object.IPrimaryOCP, psfb_ptr->Data.ISensePrimary, &Fault_Handler);
//    #endif 
//    
   // primary over voltage fault handler
   #if defined(FAULT_VPRI_OV) && (FAULT_VPRI_OV ==  true)      
   faultCheck &= FAULT_CheckMax(&psfb_ptr->Fault.Object.VPrimaryOVP, psfb_ptr->Data.VInVoltage, &Fault_Handler);
   #endif  

   // primary over voltage fault handler
   #if defined(FAULT_VPRI_UV) && (FAULT_VPRI_UV ==  true)      
   faultCheck &= FAULT_CheckMin(&psfb_ptr->Fault.Object.VPrimaryUVP, psfb_ptr->Data.VInVoltage, &Fault_Handler);
   #endif  


    // secondary over voltage fault handler
   #if defined(FAULT_VSEC_OV) && (FAULT_VSEC_OV ==  true)      
    faultCheck &= FAULT_CheckMax(&psfb_ptr->Fault.Object.VSecondaryOVP, psfb_ptr->Data.VCapVoltage, &Fault_Handler);
   #endif  

   // secondary over voltage fault handler
   #if defined(FAULT_VSEC_UV) && (FAULT_VSEC_UV ==  true)  
   if (psfb_ptr->State > 1 )   // after precharge    
        faultCheck &= FAULT_CheckMin(&psfb_ptr->Fault.Object.VSecondaryUVP, psfb_ptr->Data.VCapVoltage, &Fault_Handler);
   #endif  
   
//
//    // primary over voltage fault handler
//    #if defined(FAULT_VRAIL_5V) && (FAULT_VRAIL_5V ==  true)                
//    faultCheck &= FAULT_CheckMin(&psfb_ptr->Fault.Object.VRail_5V, psfb_ptr->Data.VRail_5V, &Fault_Handler);
//    #endif  
//    
    
// #if defined(FAULT_IPRI_OC) && (FAULT_IPRI_OC ==  true)  
//     // Hardware short circuit
//     if( CMP3_StatusGet()){
        
//         //Fault_Handler();
        
//         // Set fault bits
//         psfb_ptr->Fault.Object.IPrimaryOCP.FaultActive = 1;
//         psfb_ptr->Fault.Object.IPrimaryOCP.FaultLatch = 1;

//         //faultCheck &= psfb_ptr->Fault.Object.IPrimaryOCP.FaultActive;
        
//         PwrCtrl_PWM_Disable();
//         psfb_ptr->State = PWRCTRL_STATE_FAULT_DETECTION;
//         psfb_ptr->Fault.FaultDetected = 1;
// //        FAULT_EN_SetLow();
// //        Fault_Handler();
//     }    
    
// #endif
//    
//    #if defined (LOAD_DISCONNECT) && (LOAD_DISCONNECT ==  true)
//    // Protection when Load is removed by accident. 
//    //psfb does not sink power in this modulation. Voltage builds up on output.
//    if(psfb_ptr->PowerDirection==PWR_CTRL_CHARGING)    
//    {    
//        if((iSecAveraging.AverageValue <=  (ISEC_LOAD_STEP_CLAMP>>2)) && 
//           (vOutAveraging.AverageValue > (psfb_ptr->VLoop.Reference + VSEC_LOAD_STEP_CLAMP)) && 
//           (psfb_ptr->Properties.IReference >= 1) )
//        {
//            BatteryDisconnect = true;
//            Fault_Handler();
//        }   
//    }
//    if(psfb_ptr->PowerDirection==PWR_CTRL_DISCHARGING)    
//    {
//        if((iSecAveraging.AverageValue <=  (ISEC_LOAD_STEP_CLAMP>>2)) && 
//           (vPrimAveraging.AverageValue > (psfb_ptr->VLoop.Reference + VPRIM_LOAD_STEP_CLAMP)) && 
//           (psfb_ptr->Properties.IReference >= 1) )
//        {
//            loadDisconnect = true;
//            Fault_Handler();
//        } 
//    }   
//    #endif

    psfb_ptr->Status.bits.FaultActive = faultCheck;
    
    // Identify the fault that trips
    psfb_ptr->Fault.FaultDetected = Fault_GetFlags();
}


/*******************************************************************************
 * @ingroup fault
 * @brief   Clears the fault object flag bits
 * @return void
 * 
 * @details This function clears the fault status bits for Fault Active and 
 *  Fault Latched. A fault pin is also drive to high to allow PWM signal drive. 
 *********************************************************************************/
void Fault_Reset(void)
{
    // Drive the fault pin to high to allow PWM signal drive
    //FAULT_EN_SetHigh();
    
    // Clear fault Objects FaultActive bit
    psfb_ptr->Fault.Object.IPrimaryOCP.FaultActive = 0;
    psfb_ptr->Fault.Object.ISecondaryOCP.FaultActive = 0;
    psfb_ptr->Fault.Object.ISenseSCP.FaultActive = 0;
    psfb_ptr->Fault.Object.VPrimaryOVP.FaultActive = 0;
    psfb_ptr->Fault.Object.VPrimaryUVP.FaultActive = 0;
    psfb_ptr->Fault.Object.VSecondaryOVP.FaultActive = 0;
    psfb_ptr->Fault.Object.VSecondaryUVP.FaultActive = 0;
    psfb_ptr->Fault.Object.PowerSupplyOTP.FaultActive = 0;
    psfb_ptr->Fault.Object.VRail_5V.FaultActive= 0;

    
    // Clear fault Objects FaultLatch bit

    psfb_ptr->Fault.Object.IPrimaryOCP.FaultLatch = 0;
    psfb_ptr->Fault.Object.ISecondaryOCP.FaultLatch = 0;
    psfb_ptr->Fault.Object.ISenseSCP.FaultLatch = 0;
    psfb_ptr->Fault.Object.VPrimaryOVP.FaultLatch = 0;
    psfb_ptr->Fault.Object.VPrimaryUVP.FaultLatch = 0;
    psfb_ptr->Fault.Object.VSecondaryOVP.FaultLatch = 0;
    psfb_ptr->Fault.Object.VSecondaryUVP.FaultLatch = 0;
    psfb_ptr->Fault.Object.PowerSupplyOTP.FaultLatch = 0;
    psfb_ptr->Fault.Object.VRail_5V.FaultLatch= 0;
    
    loadDisconnect = false;

    psfb_ptr->Fault.FaultDetected = 0;
    psfb_ptr->Status.bits.FaultActive = 0;
}

/*******************************************************************************
 * @ingroup fault
 * @brief   Enable Short circuit hardware protection
 * @return void
 * 
 * @details This function setup the short circuit protection threshold and
 *  turns on the DAC (Digital-to-Analog) module. This hardware protection use
 *  Comparator DACs to detect short circuit.   
 *********************************************************************************/
static void Fault_EnableShortCircuitProtection(void)
{
//  // on dsPIC33CK DP-PIM:
//  // CMP1B used for short circuit protection on the secondary side 
//  // CMP3B used for short circuit protection on the primary side
//
//  // initialize thresholds of comparators used for short circuit protection
//  CMP_ISEC_SC_DACDataWrite(ISEC_SC_THRES_TRIG);   
//  CMP_IPRI_SC_DACDataWrite(IPRI_SC_THRES_TRIG);   
//  
//  // turn on comparator DACs 
//  CMP_IPRI_SC_DACEnable();
//  CMP_ISEC_SC_DACEnable();
  
}

/*******************************************************************************
 * @ingroup fault
 * @brief   Fault evaluation for Temperature and other slow fault detection 
 *  executed every 100ms
 * @return void
 * 
 * @details This function checks if the board temperature is within the nominal
 *  operating range. When value exceeds the limit, the power control
 *  can trip or perform temperature derating for temperature fault. 
 *********************************************************************************/
void Fault_Execute_100ms(void) 
{
    #if defined (FAULT_PS_OTP) && (FAULT_PS_OTP ==  true)
    if(FAULT_CheckMin(&psfb_ptr->Fault.Object.PowerSupplyOTP, psfb_ptr->Data.Temperature, &Fault_Handler))
    {
       devTempData.OverTemperatureFlag = 1; //for over temperature
       psfb_ptr->Status.bits.FaultActive = 1;
    }
    else
    {
        devTempData.OverTemperatureFlag = 0; //for over temperature
        psfb_ptr->Status.bits.FaultActive = 0;
    }
    #endif
}


void Fault_PrimaryOverCurrent_EventHandler(){
            //Fault_Handler();
        
        // Set fault bits
        GPIO_debug_SetHigh();
        psfb_ptr->Fault.Object.IPrimaryOCP.FaultActive = 1;
        psfb_ptr->Fault.Object.IPrimaryOCP.FaultLatch = 1;

        //faultCheck &= psfb_ptr->Fault.Object.IPrimaryOCP.FaultActive;
        
        PwrCtrl_PWM_Stop_Switching();
        psfb_ptr->State = PWRCTRL_STATE_FAULT_DETECTION;
        psfb_ptr->Fault.FaultDetected = 1;
//        FAULT_EN_SetLow();
//        Fault_Handler();
        GPIO_debug_SetLow();
}


/*******************************************************************************
 * @ingroup fault
 * @brief  Handles the fault trip by turning off the power control switching
 * @return void
 * 
 * @details This function handles the occurence of fault when one of the fault
 *  condition trips. It shuts down the operation of the power control, set the 
 *  FaultActive bit and clear the Running bit indicating that power converter
 *  has been turned-off. A fault pin is also set to low to blocked the PWM signal
 *  as a hardware protection.  
 *********************************************************************************/
void Fault_Handler(void)
{
    // Drive the fault pin to Low when Fault trips
    //FAULT_EN_SetLow();
    
    // Turn off PWM output
    PwrCtrl_PWM_Stop_Switching();
     
    //set the fault active bit
    psfb_ptr->Status.bits.FaultActive = 1;
    
    psfb_ptr->Fault.FaultDetected = 1;
    
    // clear the running bit
    psfb_ptr->Status.bits.Running = 0; 
    
}