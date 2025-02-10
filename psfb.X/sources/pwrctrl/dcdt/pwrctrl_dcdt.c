/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    dev_TPBLPFC_typedef.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.170.0
        Device            :  dsPIC33CH512MP506S1
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.70
        MPLAB 	          :  MPLAB X v6.00
 */


/**
 * @file      pwrctrl_dcdt.c
 * @ingroup   pwrctrl-dcdt   
 * @brief     Contains individual DCDT control loop initialization functions. 
 */

#include <xc.h>

#include "pwrctrl_dcdt.h"


/*******************************************************************************
 * @ingroup pwrctrl-dcdt
 * @brief Data Object of current mode control
 * 
 * @details The 'IMC_2p2z' data object holds the control loop parameter of the 
 *  Current loop.
 *******************************************************************************/
SMPS_2P2Z_T IMC_2p2z;
/*******************************************************************************

//==============================================================================
/*******************************************************************************
 * @ingroup pwrctrl-dcdt
 * @brief Current Loop A-coefficient array
 * 
 * @details This array contains the A-coefficients of the z-domain compensation
 *  filter for current mode control.
 *******************************************************************************/
int16_t   IMC_2p2zACoefficients[2]__attribute__((space(xmemory)));
/*******************************************************************************
 * @ingroup pwrctrl-dcdt
 * @brief Current Loop B-coefficient array
 * 
 * @details This array contains the B-coefficients of the z-domain compensation
 *  filter for current mode control.
 *******************************************************************************/
int16_t   IMC_2p2zBCoefficients[3] __attribute__((space(xmemory)));
/*******************************************************************************
 * @ingroup pwrctrl-dcdt
 * @brief Current Loop Control history
 * 
 * @details This array contains the current loop control output history.
 *******************************************************************************/
int16_t   IMC_2p2zControlHistory[2] __attribute__((space(ymemory), far));
/*******************************************************************************
 * @ingroup pwrctrl-dcdt
 * @brief Current Loop Error history
 * 
 * @details This array contains the error history of the current loop control.
 *******************************************************************************/
int16_t   IMC_2p2zErrorHistory[3] __attribute__((space(ymemory), far));

//==============================================================================


/*******************************************************************************
 * @ingroup pwrctrl-dcdt
 * @brief  Initialize the Current Loop Control
 * @return void
 * 
 * @details This function initializes the filter coefficients of current loop
 *  from the frequencies selected in the DCDT tool. The control and error
 *  histories of the current loop is also cleared.
 *********************************************************************************/
void PwrCtrl_IComp_Initialize(void)
{
   //charger
   IMC_2p2z.aCoefficients =  &IMC_2p2zACoefficients[0]; // Set up pointer to derived coefficients
   IMC_2p2z.bCoefficients =  &IMC_2p2zBCoefficients[0]; // Set up pointer to derived coefficients
   IMC_2p2z.controlHistory = &IMC_2p2zControlHistory[0]; // Set up pointer to controller history
   IMC_2p2z.errorHistory =   &IMC_2p2zErrorHistory[0]; // Set up pointer to error history

   IMC_2p2z.preShift =   ILOOP_COMP_2P2Z_PRESHIFT;
   IMC_2p2z.postScaler = ILOOP_COMP_2P2Z_POSTSCALER;
   IMC_2p2z.postShift =  ILOOP_COMP_2P2Z_POSTSHIFT;
   IMC_2p2z.minOutput =  ILOOP_COMP_2P2Z_MIN_CLAMP;
   IMC_2p2z.maxOutput =  ILOOP_COMP_2P2Z_MAX_CLAMP;
   
   
   IMC_2p2zACoefficients[0] = ILOOP_COMP_2P2Z_COEFF_A1;
   IMC_2p2zACoefficients[1] = ILOOP_COMP_2P2Z_COEFF_A2;
   IMC_2p2zBCoefficients[0] = ILOOP_COMP_2P2Z_COEFF_B0;
   IMC_2p2zBCoefficients[1] = ILOOP_COMP_2P2Z_COEFF_B1;
   IMC_2p2zBCoefficients[2] = ILOOP_COMP_2P2Z_COEFF_B2;
  
  //explicit clear to 0
   IMC_2p2zControlHistory[0] =0;
   IMC_2p2zControlHistory[1] =0;
   IMC_2p2zErrorHistory[0] =0;
   IMC_2p2zErrorHistory[1] =0;
   IMC_2p2zErrorHistory[2] =0;
   
   SMPS_Controller2P2ZInitialize(&IMC_2p2z);
}


/*******************************************************************************
 * @ingroup pwrctrl-dcdt
 * @brief Data Object of current mode control
 * 
 * @details The 'IMC_2p2z' data object holds the control loop parameter of the 
 *  Current loop.
 *******************************************************************************/
SMPS_3P3Z_T IMC_3p3z;
/*******************************************************************************

//==============================================================================
/*******************************************************************************
 * @ingroup pwrctrl-dcdt
 * @brief Current Loop A-coefficient array
 * 
 * @details This array contains the A-coefficients of the z-domain compensation
 *  filter for current mode control.
 *******************************************************************************/
int16_t   IMC_3p3zACoefficients[3]__attribute__((space(xmemory)));
/*******************************************************************************
 * @ingroup pwrctrl-dcdt
 * @brief Current Loop B-coefficient array
 * 
 * @details This array contains the B-coefficients of the z-domain compensation
 *  filter for current mode control.
 *******************************************************************************/
int16_t   IMC_3p3zBCoefficients[4] __attribute__((space(xmemory)));
/*******************************************************************************
 * @ingroup pwrctrl-dcdt
 * @brief Current Loop Control history
 * 
 * @details This array contains the current loop control output history.
 *******************************************************************************/
int16_t   IMC_3p3zControlHistory[3] __attribute__((space(ymemory), far));
/*******************************************************************************
 * @ingroup pwrctrl-dcdt
 * @brief Current Loop Error history
 * 
 * @details This array contains the error history of the current loop control.
 *******************************************************************************/
int16_t   IMC_3p3zErrorHistory[4] __attribute__((space(ymemory), far));

//==============================================================================


/*******************************************************************************
 * @ingroup pwrctrl-dcdt
 * @brief  Initialize the Current Loop Control
 * @return void
 * 
 * @details This function initializes the filter coefficients of current loop
 *  from the frequencies selected in the DCDT tool. The control and error
 *  histories of the current loop is also cleared.
 *********************************************************************************/
void PwrCtrl_IComp_3p3z_Initialize(void)
{
   //charger
   IMC_3p3z.aCoefficients =  &IMC_3p3zACoefficients[0]; // Set up pointer to derived coefficients
   IMC_3p3z.bCoefficients =  &IMC_3p3zBCoefficients[0]; // Set up pointer to derived coefficients
   IMC_3p3z.controlHistory = &IMC_3p3zControlHistory[0]; // Set up pointer to controller history
   IMC_3p3z.errorHistory =   &IMC_3p3zErrorHistory[0]; // Set up pointer to error history

   IMC_3p3z.preShift =   ILOOP_T_COMP_3P3Z_PRESHIFT;
   IMC_3p3z.postScaler = ILOOP_T_COMP_3P3Z_POSTSCALER;
   IMC_3p3z.postShift =  ILOOP_T_COMP_3P3Z_POSTSHIFT;
   IMC_3p3z.minOutput =  ILOOP_T_COMP_3P3Z_MIN_CLAMP;
   IMC_3p3z.maxOutput =  ILOOP_T_COMP_3P3Z_MAX_CLAMP;
   

   IMC_3p3zACoefficients[0] = ILOOP_T_COMP_3P3Z_COEFF_A1;
   IMC_3p3zACoefficients[1] = ILOOP_T_COMP_3P3Z_COEFF_A2;
   IMC_3p3zACoefficients[2] = ILOOP_T_COMP_3P3Z_COEFF_A3;
   IMC_3p3zBCoefficients[0] = ILOOP_T_COMP_3P3Z_COEFF_B0;
   IMC_3p3zBCoefficients[1] = ILOOP_T_COMP_3P3Z_COEFF_B1;
   IMC_3p3zBCoefficients[2] = ILOOP_T_COMP_3P3Z_COEFF_B2;
   IMC_3p3zBCoefficients[3] = ILOOP_T_COMP_3P3Z_COEFF_B3;
  
  //explicit clear to 0
   IMC_3p3zControlHistory[0] = 0;
   IMC_3p3zControlHistory[1] = 0;
   IMC_3p3zControlHistory[2] = 0;
   IMC_3p3zErrorHistory[0] = 0;
   IMC_3p3zErrorHistory[1] = 0;
   IMC_3p3zErrorHistory[2] = 0;
   IMC_3p3zErrorHistory[3] = 0;
   
  // SMPS_Controller2P2ZInitialize(&IMC_2p2z);
  SMPS_Controller3P3ZInitialize(&IMC_3p3z);
}



/*******************************************************************************
 * @ingroup pwrctrl-dcdt
 * @brief Data Object of voltage mode control generated by DCDT
 * 
 * @details The 'VMC_2p2z' data object holds the control loop parameter of the 
 *  Voltage loop.
 *******************************************************************************/
SMPS_2P2Z_T VMC_2p2z;

//==============================================================================

/*******************************************************************************
 * @ingroup pwrctrl-dcdt
 * @brief Voltage Loop A-coefficient array
 * 
 * @details This array contains the A-coefficients of the z-domain compensation
 *  filter for voltage mode control.
 *******************************************************************************/
int16_t   VMC_2p2zACoefficients[2]__attribute__((space(xmemory)));
/*******************************************************************************
 * @ingroup pwrctrl-dcdt
 * @brief Voltage Loop B-coefficient array
 * 
 * @details This array contains the B-coefficients of the z-domain compensation
 *  filter for voltage mode control.
 *******************************************************************************/
int16_t   VMC_2p2zBCoefficients[3] __attribute__((space(xmemory)));
/*******************************************************************************
 * @ingroup pwrctrl-dcdt
 * @brief Voltage Loop Control history
 * 
 * @details This array contains the voltage loop control output history.
 *******************************************************************************/
int16_t   VMC_2p2zControlHistory[2] __attribute__((space(ymemory), far));
/*******************************************************************************
 * @ingroup pwrctrl-dcdt
 * @brief Voltage Loop Error history
 * 
 * @details This array contains the error history of the voltage loop control.
 *******************************************************************************/
int16_t   VMC_2p2zErrorHistory[3] __attribute__((space(ymemory), far));

//==============================================================================
/*******************************************************************************
 * @ingroup pwrctrl-dcdt
 * @brief  Initialize the Voltage Loop Control
 * @return void
 * 
 * @details This function initializes the filter coefficients of voltage loop
 *  from the frequencies selected in the DCDT tool. The control and error
 *  histories of the voltage loop is also cleared.
 *********************************************************************************/
void PwrCtrl_VComp_Initialize(void)
{
  //charger direction  
  VMC_2p2z.aCoefficients =  &VMC_2p2zACoefficients[0]; // Set up pointer to derived coefficients
  VMC_2p2z.bCoefficients =  &VMC_2p2zBCoefficients[0]; // Set up pointer to derived coefficients
  VMC_2p2z.controlHistory = &VMC_2p2zControlHistory[0]; // Set up pointer to controller history
  VMC_2p2z.errorHistory =   &VMC_2p2zErrorHistory[0]; // Set up pointer to error history

  VMC_2p2z.preShift =   VLOOP_COMP_2P2Z_PRESHIFT;  
  VMC_2p2z.postScaler = VLOOP_COMP_2P2Z_POSTSCALER;
  VMC_2p2z.postShift =  VLOOP_COMP_2P2Z_POSTSHIFT;
  VMC_2p2z.minOutput =  VLOOP_COMP_2P2Z_MIN_CLAMP;
  VMC_2p2z.maxOutput =  VLOOP_COMP_2P2Z_MAX_CLAMP; 
  
  VMC_2p2zACoefficients[0] = VLOOP_COMP_2P2Z_COEFF_A1;
  VMC_2p2zACoefficients[1] = VLOOP_COMP_2P2Z_COEFF_A2;
  VMC_2p2zBCoefficients[0] = VLOOP_COMP_2P2Z_COEFF_B0;
  VMC_2p2zBCoefficients[1] = VLOOP_COMP_2P2Z_COEFF_B1;
  VMC_2p2zBCoefficients[2] = VLOOP_COMP_2P2Z_COEFF_B2; 
  
   //explicit clear to 0
  VMC_2p2zControlHistory[0] = 0;
  VMC_2p2zControlHistory[1] = 0;
  VMC_2p2zErrorHistory[0] = 0;
  VMC_2p2zErrorHistory[1] = 0;
  VMC_2p2zErrorHistory[2] = 0;
   
  SMPS_Controller2P2ZInitialize(&VMC_2p2z);  
}

/*******************************************************************************
 * @ingroup pwrctrl-dcdt
 * @brief  Clear the control and error histories of the control loops
 * @return void
 * 
 * @details This function clear the control and error histories of the control 
 *  loops. it is essential to call this function during restart condition.
 *********************************************************************************/
void PwrCtrl_ResetControlLoopHistories(void)
{

   //explicit clear to 0
  //  IMC_2p2zControlHistory[0] = 0;
  //  IMC_2p2zControlHistory[1] = 0;
  //  IMC_2p2zErrorHistory[0] = 0;
  //  IMC_2p2zErrorHistory[1] = 0;
  //  IMC_2p2zErrorHistory[2] = 0;

   IMC_3p3zControlHistory[0] = 0;
   IMC_3p3zControlHistory[1] = 0;
   IMC_3p3zControlHistory[2] = 0;
   IMC_3p3zErrorHistory[0] = 0;
   IMC_3p3zErrorHistory[1] = 0;
   IMC_3p3zErrorHistory[2] = 0;
   IMC_3p3zErrorHistory[3] = 0;
   // 
        //explicit clear to 0
  VMC_2p2zControlHistory[0] = 0;
  VMC_2p2zControlHistory[1] = 0;
  VMC_2p2zErrorHistory[0] = 0;
  VMC_2p2zErrorHistory[1] = 0;
  VMC_2p2zErrorHistory[2] = 0;
}