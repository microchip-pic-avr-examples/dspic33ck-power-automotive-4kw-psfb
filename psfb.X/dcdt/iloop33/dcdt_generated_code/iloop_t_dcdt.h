/**
  Summary:
  This header file was created using Microchip Digital Compensator Design Tool (DCDT)
  and includes system summary and digital compensator coefficients. This file
  should be included in the MPLAB X project directory.

  File Name:     iloop_t_dcdt.h
  Project Name:  iloop33
  Date:          2/7/2025
  Time:          18:03.10

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
#ifndef _ILOOP_T_DCDT_DEFINES_H
#define _ILOOP_T_DCDT_DEFINES_H

/**
  Compensator Type:  3P3Z
      Entry                Value  
    ---------            ---------
  Pole 0                 1.1000e+03 Hz
  Pole 2                 4.0000e+03 Hz
  Pole 3                 4.5000e+04 Hz
  Zero 1                 1.5000e+03 Hz
  Zero 2                 5.0000e+03 Hz
  Gain(Kdc)              4.000
  Warp                   false
  PWM Frequency          1.0000e+05
  PWM Sampling Ratio     1
  Sampling Frequency     1.0000e+05
  PWM Max Resolution     1.0600e-09
  Computational Delay    1.5000e-06
  Gate Drive Delay       1.5000e-07
  Control Output Min.    0
  Control Output Max.    19000
  Kuc Gain               7.6025e+00
  Use Kuc Gain           false


  PWM Calculations
      Name                Value  
    ---------           ---------
  Bits of Resolution    13.204
  Gain                  1.060e-04


  s-domain transfer function

               Wp0   Wp2(Wp3)(Wz1 + s)(Wz2 + s)
  H(s) = Kdc X --- X --------------------------
                s    Wz1(Wz2)(Wp2 + s)(Wp3 + s)

                 6.91e+03   2.51e+04(2.83e+05)(9.42e+03 + s)(3.14e+04 + s)
  H(s) = 4.000 X -------- X ----------------------------------------------
                    s       9.42e+03(3.14e+04)(2.51e+04 + s)(2.83e+05 + s)



  Digital Compensator Coefficients

  Name    Value     Normalized    Q15      Hex
  ----    -----     ----------    ---      ---
  a1      1.605     1.000         32764    0x7FFC
  a2      -0.472    -0.294        -9637    0xDA5B
  a3      -0.133    -0.083        -2717    0xF563
  b0      1.479     0.921         30194    0x75F2
  b1      -0.945    -0.588        -19278   0xB4B2
  b2      -1.443    -0.899        -29456   0x8CF0
  b3      0.981     0.611         20016    0x4E30


  z-domain transfer function

         u(z)  B0 + B1z^(-1) + B2z^(-2) + B3z^(-3)
  H(z) = --- = -----------------------------------
         e(z)  A0 - A1z^(-1) - A2z^(-2) - A3z^(-3)

          (1.479) + (-0.945)z^(-1) + (-1.443)z^(-2) + (0.981)z^(-3)
  H(z) = -----------------------------------------------------------
          1 - (1.605)z^(-1) - (-0.472)z^(-2) - (-0.133)z^(-3)

**/


// Compensator Coefficient Defines
#define ILOOP_T_COMP_3P3Z_COEFF_A1      0x7FFC
#define ILOOP_T_COMP_3P3Z_COEFF_A2      0xDA5B
#define ILOOP_T_COMP_3P3Z_COEFF_A3      0xF563
#define ILOOP_T_COMP_3P3Z_COEFF_B0      0x75F2
#define ILOOP_T_COMP_3P3Z_COEFF_B1      0xB4B2
#define ILOOP_T_COMP_3P3Z_COEFF_B2      0x8CF0
#define ILOOP_T_COMP_3P3Z_COEFF_B3      0x4E30
#define ILOOP_T_COMP_3P3Z_POSTSCALER    0x66C0
#define ILOOP_T_COMP_3P3Z_POSTSHIFT     0xFFFF
#define ILOOP_T_COMP_3P3Z_PRESHIFT      0x0000


// Compensator Clamp Limits
#define ILOOP_T_COMP_3P3Z_MIN_CLAMP    0x0000
#define ILOOP_T_COMP_3P3Z_MAX_CLAMP    0x4A38


#endif
