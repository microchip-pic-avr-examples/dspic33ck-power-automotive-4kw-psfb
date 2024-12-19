/**
 * PINS Generated Driver Header File 
 * 
 * @file      pins.h
 *            
 * @defgroup  pinsdriver Pins Driver
 *            
 * @brief     The Pin Driver directs the operation and function of 
 *            the selected device pins using dsPIC MCUs.
 *
 * @skipline @version   Firmware Driver Version 1.0.2
 *
 * @skipline @version   PLIB Version 1.3.0
 *
 * @skipline  Device : dsPIC33CK256MP506
*/

/*
© [2024] Microchip Technology Inc. and its subsidiaries.

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

#ifndef PINS_H
#define PINS_H
// Section: Includes
#include <xc.h>

// Section: Device Pin Macros

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD7 GPIO Pin which has a custom name of LED_GREEN to High
 * @pre      The RD7 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define LED_GREEN_SetHigh()          (_LATD7 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD7 GPIO Pin which has a custom name of LED_GREEN to Low
 * @pre      The RD7 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED_GREEN_SetLow()           (_LATD7 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RD7 GPIO Pin which has a custom name of LED_GREEN
 * @pre      The RD7 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED_GREEN_Toggle()           (_LATD7 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RD7 GPIO Pin which has a custom name of LED_GREEN
 * @param    none
 * @return   none  
 */
#define LED_GREEN_GetValue()         _RD7

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD7 GPIO Pin which has a custom name of LED_GREEN as Input
 * @param    none
 * @return   none  
 */
#define LED_GREEN_SetDigitalInput()  (_TRISD7 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD7 GPIO Pin which has a custom name of LED_GREEN as Output
 * @param    none
 * @return   none  
 */
#define LED_GREEN_SetDigitalOutput() (_TRISD7 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD8 GPIO Pin which has a custom name of LED_RED to High
 * @pre      The RD8 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define LED_RED_SetHigh()          (_LATD8 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD8 GPIO Pin which has a custom name of LED_RED to Low
 * @pre      The RD8 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED_RED_SetLow()           (_LATD8 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RD8 GPIO Pin which has a custom name of LED_RED
 * @pre      The RD8 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED_RED_Toggle()           (_LATD8 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RD8 GPIO Pin which has a custom name of LED_RED
 * @param    none
 * @return   none  
 */
#define LED_RED_GetValue()         _RD8

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD8 GPIO Pin which has a custom name of LED_RED as Input
 * @param    none
 * @return   none  
 */
#define LED_RED_SetDigitalInput()  (_TRISD8 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD8 GPIO Pin which has a custom name of LED_RED as Output
 * @param    none
 * @return   none  
 */
#define LED_RED_SetDigitalOutput() (_TRISD8 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD9 GPIO Pin which has a custom name of FAULT_EN to High
 * @pre      The RD9 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define FAULT_EN_SetHigh()          (_LATD9 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD9 GPIO Pin which has a custom name of FAULT_EN to Low
 * @pre      The RD9 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define FAULT_EN_SetLow()           (_LATD9 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RD9 GPIO Pin which has a custom name of FAULT_EN
 * @pre      The RD9 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define FAULT_EN_Toggle()           (_LATD9 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RD9 GPIO Pin which has a custom name of FAULT_EN
 * @param    none
 * @return   none  
 */
#define FAULT_EN_GetValue()         _RD9

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD9 GPIO Pin which has a custom name of FAULT_EN as Input
 * @param    none
 * @return   none  
 */
#define FAULT_EN_SetDigitalInput()  (_TRISD9 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD9 GPIO Pin which has a custom name of FAULT_EN as Output
 * @param    none
 * @return   none  
 */
#define FAULT_EN_SetDigitalOutput() (_TRISD9 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD14 GPIO Pin which has a custom name of GPIO_debug to High
 * @pre      The RD14 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define GPIO_debug_SetHigh()          (_LATD14 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD14 GPIO Pin which has a custom name of GPIO_debug to Low
 * @pre      The RD14 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define GPIO_debug_SetLow()           (_LATD14 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RD14 GPIO Pin which has a custom name of GPIO_debug
 * @pre      The RD14 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define GPIO_debug_Toggle()           (_LATD14 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RD14 GPIO Pin which has a custom name of GPIO_debug
 * @param    none
 * @return   none  
 */
#define GPIO_debug_GetValue()         _RD14

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD14 GPIO Pin which has a custom name of GPIO_debug as Input
 * @param    none
 * @return   none  
 */
#define GPIO_debug_SetDigitalInput()  (_TRISD14 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD14 GPIO Pin which has a custom name of GPIO_debug as Output
 * @param    none
 * @return   none  
 */
#define GPIO_debug_SetDigitalOutput() (_TRISD14 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD15 GPIO Pin which has a custom name of LED_DP_PIM to High
 * @pre      The RD15 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define LED_DP_PIM_SetHigh()          (_LATD15 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD15 GPIO Pin which has a custom name of LED_DP_PIM to Low
 * @pre      The RD15 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED_DP_PIM_SetLow()           (_LATD15 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RD15 GPIO Pin which has a custom name of LED_DP_PIM
 * @pre      The RD15 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED_DP_PIM_Toggle()           (_LATD15 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RD15 GPIO Pin which has a custom name of LED_DP_PIM
 * @param    none
 * @return   none  
 */
#define LED_DP_PIM_GetValue()         _RD15

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD15 GPIO Pin which has a custom name of LED_DP_PIM as Input
 * @param    none
 * @return   none  
 */
#define LED_DP_PIM_SetDigitalInput()  (_TRISD15 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD15 GPIO Pin which has a custom name of LED_DP_PIM as Output
 * @param    none
 * @return   none  
 */
#define LED_DP_PIM_SetDigitalOutput() (_TRISD15 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Initializes the PINS module
 * @param    none
 * @return   none  
 */
void PINS_Initialize(void);



#endif
