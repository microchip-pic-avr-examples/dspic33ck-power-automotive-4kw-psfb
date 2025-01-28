/**
  Summary:
  This header file was created using Microchip Digital Compensator Design Tool (DCDT)
  and includes system summary and digital compensator coefficients. This file
  should be included in the MPLAB X project directory.

  File Name:     iloop_dcdt.h
  Project Name:  iloop
  Date:          1/23/2025
  Time:          10:39.07

  Software License Agreement

  Copyright � 2025 Microchip Technology Inc.  All rights reserved.
  Microchip licenses to you the right to use, modify, copy and distribute
  Software only when embedded on a Microchip microcontroller or digital
  signal controller, which is integrated into your product or third party
  product (pursuant to the sublicense terms in the accompanying license
  agreement).

  You should refer to the license agreement accompanying this Software
  for additional information regarding your rights and obligations.

  SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS� WITHOUT WARRANTY OF ANY
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
#ifndef _ILOOP_DCDT_DEFINES_H
#define _ILOOP_DCDT_DEFINES_H

/**
  Compensator Type:  2P2Z
      Entry                Value  
    ---------            ---------
  Pole 0                 2.0000e+02 Hz
  Pole 2                 5.0000e+04 Hz
  Zero 1                 1.0000e+03 Hz
  Gain(Kdc)              1.000
  Warp                   false
  PWM Frequency          1.0000e+05
  PWM Sampling Ratio     1
  Sampling Frequency     1.0000e+05
  PWM Max Resolution     1.0600e-09
  Computational Delay    1.3500e-06
  Gate Drive Delay       1.5000e-07
  Control Output Min.    0
  Control Output Max.    19200
  Kuc Gain               3.0432e+01
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

                  1.26e+03   3.14e+05(6.28e+03 + s)
  H(s) = 1.000 X -------- X ----------------------
                     s       6.28e+03(3.14e+05 + s)



  Digital Compensator Coefficients

  Name    Value     Normalized    Q15     Hex
  ----    -----     ----------    ---     ---
  a1      0.778     0.778         25492   0x6394
  a2      0.222     0.222         7275    0x1C6B
  b0      0.126     0.126         4130    0x1022
  b1      0.008     0.008         251     0x00FB
  b2      -0.118    -0.118        -3878   0xF0DA


  z-domain transfer function

         u(z)  B0 + B1z^(-1) + B2z^(-2)
  H(z) = --- = ------------------------
         e(z)  A0 - A1z^(-1) - A2z^(-2)

          (0.126) + (0.008)z^(-1) + (-0.118)z^(-2)
  H(z) = ---------------------------------------------
          1 - (0.778)z^(-1) - (0.222)z^(-2)

**/


// Compensator Coefficient Defines
#define ILOOP_COMP_2P2Z_COEFF_A1      0x6394
#define ILOOP_COMP_2P2Z_COEFF_A2      0x1C6B
#define ILOOP_COMP_2P2Z_COEFF_B0      0x1022
#define ILOOP_COMP_2P2Z_COEFF_B1      0x00FB
#define ILOOP_COMP_2P2Z_COEFF_B2      0xF0DA
#define ILOOP_COMP_2P2Z_POSTSCALER    0x7FFF
#define ILOOP_COMP_2P2Z_POSTSHIFT     0x0000
#define ILOOP_COMP_2P2Z_PRESHIFT      0x0000


// Compensator Clamp Limits
#define ILOOP_COMP_2P2Z_MIN_CLAMP    0x0000
#define ILOOP_COMP_2P2Z_MAX_CLAMP    19000  // clamp values added manually here. TODO: change DCDT project file.


#endif
