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
//#include "timer/sccp1.h"
#include "os/os_scheduler.h"
#include "device/dev_led.h"
//#include "device/dev_fan.h"
#include "app/app_PBV_psfb_frame_map.h"
//#include "device/dev_temp.h"
#include "x2cScope/X2CScope.h"
#include "config/comms_config.h"
#include "pwm_hs/pwm.h"
#include "device/dev_adc_temporary.h"
#include "fault/fault.h"
#include "pwrctrl/pwrctrl_isr.h"

// AR-241126: header file for calling custom peripheral configuration after MCC config
#include "sources/driver/mcc_extension/mcc_custom_config.h"
/*
    Main application
*/


int main(void)
{
    
    SYSTEM_Initialize();
    PwrCtrl_Initialize();
    Fault_Initialize();

    // AR-241126: Calling Custom PWM Daisy Chain Configuration 
    MCC_Custom_User_Config();   // pwm initialize and enable.
    
    App_PBV_psfb_Init();
    Dev_LED_Init();
    OS_Init(); 
    TMR1_TimeoutCallbackRegister (TMR1_CallBack);  // scheduler timer 100us. statemachine

  
#if (X2CDEBUG_ENABLED == 1)
X2CScope_Init();
#endif
    
    OS_Scheduler_RunForever();
    
    while(1)
    {
        
    }    
}

void __attribute__ ( ( __interrupt__ , auto_psv ) ) _ADCAN0Interrupt ( void )
{
    //Read the ADC value from the ADCBUF
    psfb_ptr->Data.ISensePrimary = ADCBUF0;
    psfb_ptr->Data.ISenseSecondary = ADCBUF1;
    
    ControlLoop_Interrupt_CallBack();   //update software based ADC, execute Faults
    
    //clear the FB_P_CT_FILT interrupt flag
    IFS5bits.ADCAN0IF = 0;
}