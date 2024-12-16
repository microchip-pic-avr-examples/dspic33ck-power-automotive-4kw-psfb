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

//// PUBLIC FUNCTIONS
extern void PwrCtrl_StateMachine(POWER_CONTROL_t* pcInstance);


void PwrCtrl_Initialize(void){

}

void PwrCtrl_Execute(void) {
    // Execute the state machine
    PwrCtrl_StateMachine(psfb_ptr);
}

void Dev_PwrCtrl_Suspend(void){

}

void Dev_PwrCtrl_ControlLoopInitialize(void){

}

void PwrCtrl_Reset(void){

}