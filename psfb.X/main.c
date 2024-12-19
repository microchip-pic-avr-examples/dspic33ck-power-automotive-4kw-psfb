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

void custom_pwm(void);
void custom_pwm_n(void);

int main(void)
{
    
    SYSTEM_Initialize();
    TMR1_TimeoutCallbackRegister (TMR1_CallBack);  // scheduler timer 100us. statemachine
    
    //SCCP1_Timer_TimeoutCallbackRegister(ControlLoop_Interrupt_CallBack); // control + feedback (ADC) update, fault management       // turn this off, and hook everything to PWM interrupt.
    //PWM_GeneratorEOCEventCallbackRegister(ControlLoop_Interrupt_CallBack);

    PwrCtrl_Initialize();
    Fault_Initialize();

//    custom_pwm();
//    custom_pwm_n(); // AR-241126: Disabled, is now part of following function
    // AR-241126: Calling Custom PWM Daisy Chain Configuration 
    MCC_Custom_User_Config();

    _PWM1IP = 5;        // high priority
    _PWM1IF = 0;
    _PWM1IE = 1;        // enable interrupt
    
    //DACCTRL1Lbits.FCLKDIV = 0b101;
    OS_Init(); 
    App_PBV_psfb_Init();
    Dev_LED_Init();
//    Dev_ADC_init();



//    Dev_Temp_Init();
  
#if (X2CDEBUG_ENABLED == 1)
X2CScope_Init();
#endif
    
    OS_Scheduler_RunForever();
    
    while(1)
    {
        
    }    
}

// Instances of LOGCON A C E connect to xH pin  
// Instances of LOGCON B D F connect to xL pin

// implement ~ (1H . 3L ) = ~1H + ~3L
void custom_pwm(void){
    PWM_OverrideDataHighSet(2, 0);
    PWM_OverrideHighEnable(2);
    PWM_OverrideDataHighSet(4, 0);
    PWM_OverrideHighEnable(4);
    
//    LOGCONBbits.PWMS1B = 0b0000;    // PWM1H
//    LOGCONBbits.PWMS2B = 0b0101;    // PWM3L
//    LOGCONBbits.S1BPOL = 0b1;       // invert input 
//    LOGCONBbits.S2BPOL = 0b1;       // invert input 
//    LOGCONBbits.PWMLFB = 0b00;      // OR
//    LOGCONBbits.PWMLFBD = 0b001;    //output to pwm 2
//    
//    LOGCONDbits.PWMS1D = 0b0100;    // PWM3H
//    LOGCONDbits.PWMS2D = 0b0001;    // PWM1L
//    LOGCONDbits.S1DPOL = 0b1;       // invert input 
//    LOGCONDbits.S2DPOL = 0b1;       // invert input 
//    LOGCONDbits.PWMLFD = 0b00;      // OR
//    LOGCONDbits.PWMLFDD = 0b011;    // output to pwm 4
    
    
    LOGCONDbits.PWMS1D = 0b0000;    // PWM1H
    LOGCONDbits.PWMS2D = 0b0101;    // PWM3L
    LOGCONDbits.S1DPOL = 0b1;       // invert input 
    LOGCONDbits.S2DPOL = 0b1;       // invert input 
    LOGCONDbits.PWMLFD = 0b00;      // OR
    LOGCONDbits.PWMLFDD = 0b011;    //output to pwm 4
    
    LOGCONBbits.PWMS1B = 0b0100;    // PWM3H
    LOGCONBbits.PWMS2B = 0b0001;    // PWM1L
    LOGCONBbits.S1BPOL = 0b1;       // invert input 
    LOGCONBbits.S2BPOL = 0b1;       // invert input 
    LOGCONBbits.PWMLFB = 0b00;      // OR
    LOGCONBbits.PWMLFBD = 0b001;    // output to pwm 2

}

void custom_pwm_n(void){
    PG4IOCONLbits.SWAP = 1;
}

void __attribute__((__interrupt__, auto_psv))_PWM1Interrupt(void){
    ControlLoop_Interrupt_CallBack();
//    _PWM1IF = 0;
}