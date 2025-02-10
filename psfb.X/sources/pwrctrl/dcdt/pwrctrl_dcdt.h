/*
  @Company
    Microchip Technology Inc.

  @File Name
    dev_TPBLPFC_typedef.h

  @Summary
    This is the generated driver implementation file using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides Controller settings for average current mode and voltage mode controller.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.167.0
        Device            :  dsPIC33CH512MP506S1      
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.70
        MPLAB 	          :  MPLAB X v6.00
*/


/**
 * @file pwrctrl_dcdt.h     
 * @ingroup pwrctrl-dcdt   
 * @brief Contains public functions and data types relevant for DCDT file interface.
 */

#ifndef _DRV_CONTROLLER_PSFB_DCDC_H
#define _DRV_CONTROLLER_PSFB_DCDC_H
#include <stdio.h>
#include <xc.h>
#include <stdint.h>
#include "smps_control.h"


#include "iloop/dcdt_generated_code/iloop_dcdt.h"
#include "iloop33/dcdt_generated_code/iloop_t_dcdt.h"

extern SMPS_2P2Z_T IMC_2p2z;
extern SMPS_3P3Z_T IMC_3p3z;

void PwrCtrl_IComp_Initialize(void);
void PwrCtrl_ResetControlLoopHistories(void);
//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
#endif	/* XC_HEADER_TEMPLATE_H */