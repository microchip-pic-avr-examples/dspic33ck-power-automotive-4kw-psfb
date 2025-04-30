/**
 * PINS Generated Driver Source File 
 * 
 * @file      pins.c
 *            
 * @ingroup   pinsdriver
 *            
 * @brief     This is the generated driver source file for PINS driver.
 *
 * @skipline @version   Firmware Driver Version 1.0.2
 *
 * @skipline @version   PLIB Version 1.3.0
 *
 * @skipline  Device : dsPIC33CK256MP506
*/

/*
© [2025] Microchip Technology Inc. and its subsidiaries.

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

// Section: Includes
#include <xc.h>
#include <stddef.h>
#include "../pins.h"

// Section: File specific functions

// Section: Driver Interface Function Definitions
void PINS_Initialize(void)
{
    /****************************************************************************
     * Setting the Output Latch SFR(s)
     ***************************************************************************/
    LATA = 0x0000;
    LATB = 0x0000;
    LATC = 0x0400;
    LATD = 0x0000;

    /****************************************************************************
     * Setting the GPIO Direction SFR(s)
     ***************************************************************************/
    TRISA = 0x0017;
    TRISB = 0x03FF;
    TRISC = 0xF9FF;
    TRISD = 0x3C34;


    /****************************************************************************
     * Setting the Weak Pull Up and Weak Pull Down SFR(s)
     ***************************************************************************/
    CNPUA = 0x0000;
    CNPUB = 0x0000;
    CNPUC = 0x0000;
    CNPUD = 0x0000;
    CNPDA = 0x0000;
    CNPDB = 0x0000;
    CNPDC = 0x0000;
    CNPDD = 0x0000;


    /****************************************************************************
     * Setting the Open Drain SFR(s)
     ***************************************************************************/
    ODCA = 0x0000;
    ODCB = 0x0000;
    ODCC = 0x0000;
    ODCD = 0x0000;


    /****************************************************************************
     * Setting the Analog/Digital Configuration SFR(s)
     ***************************************************************************/
    ANSELA = 0x001F;
    ANSELB = 0x0386;
    ANSELC = 0x00CF;
    ANSELD = 0x2C00;

    /****************************************************************************
     * Set the PPS
     ***************************************************************************/
     __builtin_write_RPCON(0x0000); // unlock PPS

        RPINR18bits.U1RXR = 0x003B; //RC11->UART1:U1RX;
        RPINR26bits.CAN1RXR = 0x0042; //RD2->CAN1:CAN1RX;
        RPOR12bits.RP57R = 0x0019;  //RC9->CMP3:CMP3;
        RPOR13bits.RP58R = 0x0001;  //RC10->UART1:U1TX;
        RPOR17bits.RP67R = 0x0022;  //RD3->PWM:PWM4H;
        RPOR19bits.RP71R = 0x0015;  //RD7->CAN1:CAN1TX;

     __builtin_write_RPCON(0x0800); // lock PPS


}

