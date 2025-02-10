/**
  Summary:
  This header file was created using Microchip Digital Compensator Design Tool (DCDT)
  and includes system summary and digital compensator coefficients. This file
  should be included in the MPLAB X project directory.

  File Name:     vloop_dcdt.h
  Project Name:  vloop
  Date:          2/10/2025
  Time:          10:57.33

  Software License Agreement

  Copyright © 2025 Microchip Technology Inc.  All rights reserved.
  Microchip licenses to you the right to use, modify, copy and distribute
  Software only when embedded on a Microchip microcontroller or digital
  signal controller, which is integrated into your product or third party
  product (pursuant to the sublicense terms in the accompanying license
  agreement).

  You should refer to the license agreement accompanying this Software
  for additional information regarding your rights and obligations.

  SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY
  KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
  OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
  PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
  OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,
  BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT
  DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
  INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,
  COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY
  CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
  OR OTHER SIMILAR COSTS.
 **/
#ifndef _VLOOP_DCDT_DEFINES_H
#define _VLOOP_DCDT_DEFINES_H

/**
  Compensator Type:  2P2Z
      Entry                Value  
    ---------            ---------
  Pole 0                 1.0000e+02 Hz
  Pole 2                 5.0000e+03 Hz
  Zero 1                 5.0000e+02 Hz
  Gain(Kdc)              1.000
  Warp                   false
  PWM Frequency          1.0000e+05
  PWM Sampling Ratio     1
  Sampling Frequency     1.0000e+05
  PWM Max Resolution     1.0600e-09
  Computational Delay    1.3500e-06
  Gate Drive Delay       1.5000e-07
  Control Output Min.    650
  Control Output Max.    1642
  Kuc Gain               7.6025e+00
  Use Kuc Gain           false


  PWM Calculations
      Name                Value  
    ---------           ---------
  Bits of Resolution    13.204
  Gain                  1.060e-04


  s-domain transfer function

               Wp0   Wp2(Wz1 + s)
  H(s) = Kdc X --- X ------------
                s    Wz1(Wp2 + s)

                  6.28e+02   3.14e+04(3.14e+03 + s)
  H(s) = 1.000 X -------- X ----------------------
                     s       3.14e+03(3.14e+04 + s)



  Digital Compensator Coefficients

  Name    Value     Normalized    Q15      Hex
  ----    -----     ----------    ---      ---
  a1      1.728     1.000         32764    0x7FFC
  a2      -0.728    -0.421        -13809   0xCA0F
  b0      0.028     0.016         522      0x020A
  b1      0.001     0.000         16       0x0010
  b2      -0.027    -0.015        -506     0xFE06


  z-domain transfer function

         u(z)  B0 + B1z^(-1) + B2z^(-2)
  H(z) = --- = ------------------------
         e(z)  A0 - A1z^(-1) - A2z^(-2)

          (0.028) + (0.001)z^(-1) + (-0.027)z^(-2)
  H(z) = ---------------------------------------------
          1 - (1.728)z^(-1) - (-0.728)z^(-2)

**/


// Compensator Coefficient Defines
#define VLOOP_COMP_2P2Z_COEFF_A1      0x7FFC
#define VLOOP_COMP_2P2Z_COEFF_A2      0xCA0F
#define VLOOP_COMP_2P2Z_COEFF_B0      0x020A
#define VLOOP_COMP_2P2Z_COEFF_B1      0x0010
#define VLOOP_COMP_2P2Z_COEFF_B2      0xFE06
#define VLOOP_COMP_2P2Z_POSTSCALER    0x6EA2
#define VLOOP_COMP_2P2Z_POSTSHIFT     0xFFFF
#define VLOOP_COMP_2P2Z_PRESHIFT      0x0000


// Compensator Clamp Limits
#define VLOOP_COMP_2P2Z_MIN_CLAMP    0x028A
#define VLOOP_COMP_2P2Z_MAX_CLAMP    0x066A


#endif
