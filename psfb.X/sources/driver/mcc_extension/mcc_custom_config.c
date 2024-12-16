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
 * @file    mcc_custom_config.h     
 * @ingroup driver-layer 
 * @brief   Contain the MCC Custom peripheral configuration that is not supported
 *  by MCC Melody tool. 
 */
#include <xc.h>
#include "driver/mcc_extension/drv_mcc_extension.h"
//#include "driver/mcc_extension/drv_custom_config_pwm.h"
#include "driver/mcc_extension/drv_custom_config_adc.h"
#include "pwrctrl/pwrctrl.h"

/*******************************************************************************
 * @ingroup driver-layer 
 * @brief  Contains custom peripheral initialization 
 * @return void
 * 
 * @details This function initializes peripheral settings that are needed in the
 *  application but is not supported by the generated code through MCC Melody tool. 
 * 
 *********************************************************************************/

void MCC_Custom_User_Config (void)
{
    Nop(); // Place breakpoint here
    Nop();
    Nop();
    Nop();
    
    /*
     PG1 is the leading PWM channel
     PG3 is triggered by PG1, where PG3 is synchronized to PG1 including updates
     PG4 is sync rectifier of PWM1H+PWM3L and is also synchronized to PG1
     PG2 is sync rectifier of PWM3H+PWM1L and is synchronized to PG3
     All updates are triggered by the PG1 broadcast
     All updates are performed at the cycle boundary (EOC)
     PG2 and PG4 only use PWMxL output
     PG2 and PG4 are swapped, hence, their respective HIGH signal is used
     Sync Rectifier edge placement is tuned by PGxDTH and PGxDC of PG2 and PG4,
          where the duty cycle = PER - PHASE-SHIFT +/- EDGEPLACEMENT 
     */
  
    // Set base register of PWM channels
    PG1CONL = 0x0088; // CLKSEL=1, HREN=1, ON=0
    PG2CONL = 0x0088; // CLKSEL=1, HREN=1, ON=0
    PG3CONL = 0x0088; // CLKSEL=1, HREN=1, ON=0
    PG4CONL = 0x0088; // CLKSEL=1, HREN=1, ON=0
    
    // Set synchronization of PWM start and register update
    PG1CONH = 0x0800; // SOCS=0, MSTEN=1
    PG3CONH = 0x0241; // SOCS=1, TRGMOD=1, UPDMOD=2
    
    PG4CONH = 0x0241; // SOCS=1, TRGMOD=1, UPDMOD=2
    PG2CONH = 0x0243; // SOCS=3, TRGMOD=1, UPDMOD=2



    // Set I/O Configuration
    PG1IOCONL = 0x3000; // OVRENH/L=1
    PG1IOCONH = 0;      // PENH/L=0
    PG2IOCONL = 0x7000; // SWAP=1, OVRENH=1=1, OVRENL=1
    PG2IOCONH = 0;      // PENH/L=0
    PG3IOCONL = 0x3000; // OVRENH/L=1
    PG3IOCONH = 0;      // PENH/L=0
    PG4IOCONL = 0x7000; // SWAP=1, OVRENH=1=1, OVRENL=1
    PG4IOCONH = 0;      // PENH/L=0
    
    // Set PG synchronization triggers
    PG1EVTL = 0; // clear register
    PG1EVTLbits.PGTRGSEL = 0b011; // use PGxTRIGC to set phase shift
    PG1EVTLbits.UPDTRG = 0; // requires UPDREQ bit to be set manually
    PG1EVTLbits.ADTR1EN1 = 1; // enable ADC trigger 1 => TRIGA
    PG1EVTH = 0; // disable all triggers
    PG1EVTHbits.ADTR2EN2 = 1; // enable ADC trigger 2 => TRIGB
    
    PG2EVTL = 0; // disable all triggers
    PG2EVTH = 0; // disable all triggers
    PG3EVTLbits.PGTRGSEL = 0b011; // use PGxTRIGC to set phase shift
    PG3EVTLbits.UPDTRG = 0; // requires UPDREQ bit to be set manually
    PG3EVTH = 0; // disable all triggers
    PG4EVTL = 0; // disable all triggers
    PG4EVTH = 0; // disable all triggers
    
    // Set Period
    PG1PER = 40000;
    PG2PER = 40000;
    PG3PER = 40000;
    PG4PER = 40000;

    // Set Duty Cycle
    PG1DC = 20000;
    PG3DC = 20000;
    
    // Set dead times
    PG1DTH = 1000;   // set dead time to 150ns
    PG1DTL = 1000;   // set dead time to 150ns
    PG2DTH = 0;     // no dead time on PWMxH
    PG2DTL = 1000;  // set dead time to 250ns
    PG3DTH = 1000;   // set dead time to 150ns
    PG3DTL = 1000;   // set dead time to 150ns
    PG4DTH = 0;     // no dead time on PWMxH
    PG4DTL = 1000;  // set dead time to 250ns

    // Set synchronization triggers
    PG1TRIGC = 0; // phase shift = 0
    PG3TRIGC = PG1DC; // phase shift = duty cycle of leading PWM channel
    PG2DC = (PG1PER - PG1TRIGC - PG4DTL);
    PG4DC = (PG1PER - PG1TRIGC - PG4DTL);
    
    // Start PWM
    PG4CONLbits.ON = 1;
    PG2CONLbits.ON = 1;
    PG3CONLbits.ON = 1;
    PG1CONLbits.ON = 1;
    
    PCLKCONbits.MCLKSEL = 1;
    PCLKCONbits.DIVSEL = 0;
    
    int timeout = 0;
    
    while ((0 == PCLKCONbits.HRRDY) && (++timeout < 5000));
    if (timeout < 5000)
    {
        PG1IOCONHbits.PENH = 1;
        PG1IOCONHbits.PENL = 1;
        PG2IOCONHbits.PENH = 0; // not used
        PG2IOCONHbits.PENL = 1;
        PG3IOCONHbits.PENH = 1;
        PG3IOCONHbits.PENL = 1;
        PG4IOCONHbits.PENH = 0; // not used
        PG4IOCONHbits.PENL = 1;
        
        for (int i=1000; i>0; i--); // short delay
        
        PG1IOCONLbits.OVRENH = 0;
        PG1IOCONLbits.OVRENL = 0;
        PG2IOCONLbits.OVRENH = 1; // not used
        PG2IOCONLbits.OVRENL = 0;
        PG3IOCONLbits.OVRENH = 0;
        PG3IOCONLbits.OVRENL = 0;
        PG4IOCONLbits.OVRENH = 1; // not used
        PG4IOCONLbits.OVRENL = 0;
    }       
}
