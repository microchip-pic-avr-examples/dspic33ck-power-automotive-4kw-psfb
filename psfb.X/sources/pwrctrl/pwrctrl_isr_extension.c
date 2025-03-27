
/**
 * @file      pwrctrl_isr_extension.c
 * @ingroup   pwrctrl-isr   
 * @brief     Contains some of the functions used in the interrupt service routine
 *  of control loop.
 */

#include <xc.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

// MCC header files
#include "adc/adc_types.h"
#include "adc/adc1.h"
#include "system/pins.h"

// PSFB header files
//#include "config/macros.h"
//#include "config/config.h"
#include "pwrctrl.h"
#include "fault/fault.h"
#include "dcdt/pwrctrl_dcdt.h"


    
/*******************************************************************************
 * @ingroup pwrctrl-isr
 * @brief Data Object of primary voltage averaging
 * 
 * @details The 'VprimAveraging' data object holds the averaging parameter of the 
 *  Primary Voltage.
 *******************************************************************************/
AVERAGING_t vPrimAveraging;
/*******************************************************************************
 * @ingroup pwrctrl-isr
 * @brief Data Object of secondary voltage averaging
 * 
 * @details The 'vOutAveraging' data object holds the averaging parameter of the 
 *  Secondary Voltage.
 *******************************************************************************/
AVERAGING_t vOutAveraging;
/*******************************************************************************
 * @ingroup pwrctrl-isr
 * @brief Data Object of secondary current averaging
 * 
 * @details The 'IsecAveraging' data object holds the averaging parameter of the 
 *  Secondary Current.
 *******************************************************************************/
AVERAGING_t iSecAveraging;

/*******************************************************************************
 * @ingroup pwrctrl-isr
 * @brief Data Object of secondary voltage averaging
 * 
 * @details The 'VcapAveraging' data object holds the averaging parameter of the 
 *  Secondary Voltage.
 *******************************************************************************/
AVERAGING_t vCapAveraging;


// STATIC VARIABLES and FUNCTIONS
static void PwrCtrl_AdaptiveGainUpdate(void);
static bool VLoopInterleaveExec = true;

/*******************************************************************************
 * @ingroup pwrctrl-isr
 * @brief  This function updates the DAB data members with ADC raw values
 * @return void
 * 
 * @details This function is called every 100KHz and triggers the ADC module. This
 *  also handles the updating of DAB data members with its latest ADC raw values
 *  and collection of data for averaging.
 *********************************************************************************/
void PwrCtrl_UpdateADConverterData (void)
{        
    // Enable the ADC sampling
    // ADC1_SoftwareTriggerEnable();
    
/*     if(ADC1_IsConversionComplete(I_SEC_AVG_FILT))
        psfb_ptr->Data.ISenseSecondary = ADC1_ConversionResultGet(I_SEC_AVG_FILT);  */
    
/*     if(ADC1_IsConversionComplete(IPRI_CT))
        dab.Data.ISensePrimary = ADC1_ConversionResultGet(IPRI_CT);    */
    
    if(ADC1_IsConversionComplete(FB_5V)){
        psfb_ptr->Data.VRail_5V = ADC1_ConversionResultGet(FB_5V); 
    }
    
    if(ADC1_IsConversionComplete(FB_TEMP))
        psfb_ptr->Data.Temperature = ADC1_ConversionResultGet(FB_TEMP);

    if(ADC1_IsConversionComplete(FB_VCAP))
        psfb_ptr->Data.VCapVoltage = ADC1_ConversionResultGet(FB_VCAP);
    
    if(ADC1_IsConversionComplete(VIN_INT_AN)){
        psfb_ptr->Data.VInVoltage = ADC1_ConversionResultGet(VIN_INT_AN);
        // vPrimAveraging.Accumulator += psfb_ptr->Data.VInVoltage;
        // vPrimAveraging.Counter = vPrimAveraging.Counter + 1;   
    }
    
    if(ADC1_IsConversionComplete(FB_VOUT)){
        psfb_ptr->Data.VOutVoltage = ADC1_ConversionResultGet(FB_VOUT);
        // vOutAveraging.Accumulator += psfb_ptr->Data.VOutVoltage;
        // vOutAveraging.Counter = vOutAveraging.Counter + 1; 
    }
    
//    if(ADC1_IsConversionComplete(I_SEC_AVG_FILT)){
//        psfb_ptr->Data.ISenseSecondary = ADC1_ConversionResultGet(I_SEC_AVG_FILT); 
//        psfb_ptr->Data.ISecAverageRectified = abs(psfb_ptr->Data.ISenseSecondary - psfb_ptr->Data.ISecSensorOffset);
//        iSecAveraging.Accumulator += psfb_ptr->Data.ISecAverageRectified;
//        iSecAveraging.Counter = iSecAveraging.Counter + 1;
//    }
}

/*******************************************************************************
 * @ingroup pwrctrl-isr
 * @brief  This function prepares the data for control loop and selects which 
 *  control loop will be executed. 
 * @return void
 * 
 * @details This function prepares the data for Voltage loop control and Power
 *  loop control.
 *********************************************************************************/
void PwrCtrl_PrepareData(void)
{
//    vPrimAveraging.AverageValue = (uint16_t)(__builtin_divud(vPrimAveraging.Accumulator, vPrimAveraging.Counter));
//    vPrimAveraging.Accumulator = 0;
//    vPrimAveraging.Counter = 0;
//
//    iSecAveraging.AverageValue = (uint16_t)(__builtin_divud(iSecAveraging.Accumulator, iSecAveraging.Counter));
//    iSecAveraging.Accumulator = 0;
//    iSecAveraging.Counter = 0;
//
//    vOutAveraging.AverageValue = (uint16_t)(__builtin_divud(vOutAveraging.Accumulator, vOutAveraging.Counter));
//    vOutAveraging.Accumulator = 0;
//    vOutAveraging.Counter = 0;         
}   

/*******************************************************************************
 * @ingroup pwrctrl-isr
 * @brief Executes the power converter control loop 
 * @return void
 * 
 * @details This function is called to execute the control loop of the power
 *  converter. It comprise of Voltage Loop control (VLoop), Power Loop control (PLoop) 
 *  and Current Loop Control (ILoop). Vloop and PLoop is ten times slower than
 *  the current loop with interleaved execution while Iloop is executed every time 
 *  this function is called.  
 *********************************************************************************/
void PwrCtrl_ControlLoopExecute(void)
{   
    // control loop execute 
    uint16_t IloopReference = 0;
    
    if (psfb_ptr->VLoop.Enable == 1) {
        // psfb_ptr->VLoop.Feedback = psfb_ptr->Data.VCapVoltage;  
        psfb_ptr->VLoop.Feedback = psfb_ptr->Data.VOutVoltage;
        
            SMPS_Controller2P2ZUpdate(
                &VMC_2p2z,                      // SPMPS_2P2Z_T pointer type structure
                &(psfb_ptr->VLoop.Feedback),    // pointer to the input source register or variable being tracked by 2P2Z
                psfb_ptr->VLoop.Reference,      // VLoopReference from Vramp
                &(psfb_ptr->VLoop.Output)       // pointer to the control loop target register of the calculated result 
            );
    }     
    // Vloop output 
    IloopReference = psfb_ptr->VLoop.Output; 
    
    if (psfb_ptr->ILoop.Enable == 1) {
        psfb_ptr->ILoop.Feedback = psfb_ptr->Data.ISenseSecondary;

        SMPS_Controller3P3ZUpdate(
            &IMC_3p3z,                      // SPMPS_2P2Z_T pointer type structure
            &(psfb_ptr->ILoop.Feedback),    // pointer to the input source register or variable being tracked by 2P2Z
            //psfb_ptr->ILoop.Reference,      // ILoopReference from Iramp
            psfb_ptr->VLoop.Output,                 // IloopReference from Vloop
            &(psfb_ptr->ILoop.Output)          // pointer to the control loop target register of the calculated result 
        );   

        // temporary
        // psfb_ptr->iloop_output =    psfb_ptr->ILoop.Output;
        // psfb_ptr->vloop_output =    psfb_ptr->VLoop.Output;
        
        
        PwrCtrl_SetPhaseTarget(psfb_ptr->ILoop.Output);
        PwrCtrl_PWM_Update();
    }
}

/*******************************************************************************
 * @ingroup pwrctrl-isr
 * @brief Updates the Adaptive gain for the power converter control loop 
 * @return void
 * 
 * @details AGC is a specific method for tuning the overall feedback loop gain 
 * during runtime. This function handles the calculation of the gain at a particular 
 * input voltage. AGC is active when it is above the minimum AGC input voltage.
 *********************************************************************************/
static void PwrCtrl_AdaptiveGainUpdate(void)
{

}

/*******************************************************************************
 * @ingroup pwrctrl-isr
 * @brief  This function updates the DAB data members with phase values normalized 
 *  in degree
 * @return void
 * 
 * @details This function updates the DAB data members with phase values normalized 
 * in degree. The calculated phase is scaled by 10x to have better phase resolution.
 *********************************************************************************/
void PwrCtrl_PrimToSecPHDegree(void)
{

}

/*******************************************************************************
 * @ingroup pwrctrl-isr
 * @brief  This function updates the DAB data members dead time based on load. 
 * @return void
 * 
 * @details This function updates the DAB data members dead time based on load. 
 * The phase and dead-time in this function is based on the actual board test.
 *********************************************************************************/
void PwrCtrl_DeadTimeAdjust(void)
{

}

