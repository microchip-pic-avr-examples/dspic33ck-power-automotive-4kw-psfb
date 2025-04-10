/*
� [2023] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/
#include "system/system.h"
#include "timer/tmr1.h"
#include "os/os_scheduler.h"
#include "device/dev_led.h"
#include "app/app_PBV_psfb_frame_map.h"
#include "config/comms_config.h"
#include "pwm_hs/pwm.h"
#include "fault/fault.h"
#include "pwrctrl/pwrctrl_isr.h"
#include "pwrctrl/pwrctrl.h"
#include "system/pins.h"
#include "adc/adc1.h"
#include "sources/config/macros.h"

// header file for calling custom peripheral configuration after MCC config
#include "sources/driver/mcc_extension/mcc_custom_config.h"
uint16_t counter = 65500;

uint16_t macro_debug = 0;

/*
    Main application
*/

/*******************************************************************************
 * @ingroup 
 * @brief  Application main function executed after device comes out of RESET
 * @details 
 * This function is the starting point of the firmware. It is called after the
 * device is coming out of RESET, starting to execute code. 
 * 
 *********************************************************************************/
int main(void)
{
    
    SYSTEM_Initialize();
    PwrCtrl_Initialize();
    Fault_Initialize();


    //disabling the interrupt
    IEC4bits.CMP3IE = 0;
    IPC19bits.CMP3IP = 6;       // Primary OCP higher priority then all. 7 is highest, rest of the interrupts are at 1.
    // Clearing IF flag before enabling the interrupt.
    IFS4bits.CMP3IF = 0;
    // Enabling CMP3 interrupt.
    IEC4bits.CMP3IE = 1;

    // Calling Custom PWM Daisy Chain Configuration 
    MCC_Custom_User_Config();   // pwm initialize and enable.
    
    App_PBV_psfb_Init();
    Dev_LED_Init();
    psfb_ptr->SecRec.SR_Enabled = 0;    // enabled after start power transfer
    psfb_ptr->SecRec.SR_Flag = 0;   // make sure it is off


    ADC1_SoftwareTriggerEnable(); // add it here so that the very first time the values are available
    dev_MeasureOffsets_Initialize();
    
    while (counter--> 0) Nop(); // implementing delay for values to settle
    
    OS_Init(); 
    TMR1_TimeoutCallbackRegister (TMR1_CallBack);  // scheduler timer 100us. statemachine
    
    // ISEC_OC_THRES_TRIG 3350
    // VSEC_OV_THRES_TRIG 764
    // VPRI_OV_THRES_TRIG 4048
    // VPRI_UV_THRES_TRIG 1932 
    // VSEC_UV_THRES_TRIG 764
    // VRAIL_5V_UV_THRES_TRIG 2321
    // primary voltage equation to be revisted
    macro_debug = IPRI_SC_THRES_TRIG;
    OS_Scheduler_RunForever();
    
    while(1)
    {
        
    }    
}

/*******************************************************************************
 * @ingroup pwrctrl-isr
 * @brief  Executes the power converter control loop
 * @return void
 * 
 * @details This interrupt function is a ADC Core 0 interrupt executed every 100KHz
 * This ADC is triggered using trigger b of PG1. This trigger is half of the phase 
 * difference between fixed and phase shifted leg
 *********************************************************************************/
void __attribute__ ( ( __interrupt__ , auto_psv ) ) _ADCAN0Interrupt ( void )
{
    GPIO_debug_SetHigh();   // to keep track of the interrupt latency per cycle
    //Read all the ADC value from the ADCBUF
    psfb_ptr->Data.ISensePrimary = ADCBUF0;
    psfb_ptr->Data.ISenseSecondary = ADCBUF1;
    
    PwrCtrl_UpdateADConverterData();
    
    ADC1_SoftwareTriggerEnable();
    
    // Execute the fault detection
    Fault_Execute();    // moving it here, so that all other faults are checked other than VCAP undervoltage

    if (psfb_ptr->State > 1) {             // if state is greater than precharge, give the phase shift control over to controller
        ControlLoop_Interrupt_CallBack();   //update software based ADC, execute Faults
    } 

    PwrCtrl_PWM_UpdateSecondaryRectifiers(); // check Ishunt within range
    
    GPIO_debug_SetLow();
    //clear the FB_P_CT_FILT interrupt flag
    IFS5bits.ADCAN0IF = 0;
}
