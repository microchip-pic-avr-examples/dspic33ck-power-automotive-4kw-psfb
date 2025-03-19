/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */
/**
 * @file pwrctrl_typedefs.h     
 * @ingroup pwrctrl-data-types   
 * @brief Contains the power control data structure.    
 */

#ifndef DEV_PWRCTRL_TYPEDEF_H
#define	DEV_PWRCTRL_TYPEDEF_H

#include "fault/fault_typedef.h"

/***********************************************************************************
 * @ingroup pwrctrl-data-types
 * @brief   Enumeration of power controller state machine states
 * @details
 * These are all of the states for the power controller state machine
 ***********************************************************************************/
enum  PWR_CTRL_STATES_e
{
  PWRCTRL_STATE_INITIALIZE = 0,       ///< power converter control state #0:  initializes object properties and clears status bits
  PWRCTRL_STATE_PRECHARGE,            ///< power converter control state #1:  charging output caps
  PWRCTRL_STATE_STANDBY,              ///< power converter control state #2:  waiting for status bits allowing power supply to run
  PWRCTRL_STATE_START_CONTROL,        ///< power converter control state #3:  wait for some time to 
  PWRCTRL_STATE_SOFT_START,           ///< power converter control state #4:  performs soft-start sequence 
  PWRCTRL_STATE_UP_AND_RUNNING,       ///< power converter control state #5:  output is in regulation and power is OK (normal continuous operation)
  PWRCTRL_STATE_FAULT_DETECTION,      ///< power converter control state #6:  checks if a fault occurs
};
typedef enum PWR_CTRL_STATES_e PWR_CTRL_STATE_t;

/***********************************************************************************
 * @ingroup pwrctrl-data-types
 * @brief   Power converter switch-node specifications
 * @details
 *   This data structure is used to set the converter switch-node specifications declaring which
 *  PWM channel is used as well as its switching period, duty cycle and phase-shift.
 ***********************************************************************************/
struct SWITCH_NODE_s
{
    uint16_t ControlPeriod;                 ///< Control period value from control loop
    uint16_t ControlDutyCycle;              ///< Control Duty Cycle calculation based on Control Period
    uint16_t ControlPhase;                  ///< Control phase value from control loop
    uint16_t ControlPhase_P2S_Degreex10;    ///< Control phase value from control loop in 10 x degree, integer
    uint16_t ControlPhase_P2S_Target;       ///< Control phase value from control loop in degrees
    uint16_t DeadTimeHigh;                  ///< Deadtime High settings for PWM
    uint16_t DeadTimeLow;                   ///< Deadtime High settings for PWM
    uint16_t PBVPeriodTarget;               ///< Power Board Visualizer Set Control target 
    uint16_t PBVControlPhaseTarget;         ///< Power Board Visualizer Set Control phase
    uint16_t LowPowerSlowMode;              ///< future development
};
typedef struct SWITCH_NODE_s SWITCH_NODE_t;

/***********************************************************************************
 * @ingroup pwrctrl-data-types
 * @brief   Publicly accessible data buffer of most recent runtime data values
 * @details
 *   This data structure is used to store the feedback values of the most 
 * recent runtime data values.
 ***********************************************************************************/
struct FEEDBACK_SETTINGS_s
{
    uint16_t VInVoltage;            ///< Data value for input voltage 
    uint16_t VOutVoltage;           ///< Data value for output voltage
    uint16_t VCapVoltage;           ///< Data value for capacitor
    
    uint16_t ISensePrimary;         ///< Data value for primary current as measured with CT
    uint16_t IPriSensorOffset;      ///< Offset of the primary current sensor

    uint16_t ISenseSecondary;       ///< Data value for secondary current as measured with shunt
    uint16_t ISecAverage;           ///< Data value for average secondary current 
    uint16_t ISecAverageRectified;  ///< Data value for average secondary current rectified
    uint16_t ISecSensorOffset;      ///< Offset of the secondary current sensor

    int16_t  SecPower;              ///< Data value for Secondary power (Watt)
    
    uint16_t Temperature;           ///< Data value for temperature
    uint16_t VRail_5V;              ///< Data value for 5V auxiliary rail

    uint16_t Channel_AN15;          ///< Channel 15
};
typedef struct FEEDBACK_SETTINGS_s FEEDBACK_SETTINGS_t;

/***********************************************************************************
 * @ingroup pwrctrl-data-types
 * @brief Power converter status flags
 * @details
 *      This data structure is used to indicate when the there is a fault or
 * running power supply.
 **********************************************************************************/
struct STATUS_FLAGS_s {
    union {
        struct {      
            unsigned Running         : 1; ///< Bit 0: Power converter is running
            unsigned FaultActive     : 1; ///< Bit 1: Power converter fault is active
            unsigned notUsed         : 13;///< Bit 2 - 15: Not used 
        } __attribute__((packed)) bits; 
        uint16_t value;
    };
};
typedef struct STATUS_FLAGS_s STATUS_FLAGS_t;

/***********************************************************************************
 * @ingroup pwrctrl-data-types
 * @brief Stores data related to the ramping up/down of the reference
 * @details This data structure manages the ramping up/down of the reference depending 
 *  on the target. "reference" is the actual reference currently being used while
 *  "targetReference" can be different that reference during soft-start.
 **********************************************************************************/
struct START_UP_RAMP_s 
{
    uint16_t Counter;               ///< Soft-Start Execution Counter. This setting is set/cleared by the device driver and is 'read only'.
    uint16_t Delay;             	///< Soft-Start Period (POD, RAMP PERIOD, PGD, etc.)
    uint16_t StepSize;              ///< Size/value of one reference increment/decrement or this period
    uint16_t* ptrReference;         ///< pointer the reference variable
    uint16_t* ptrReferenceTarget;   ///< pointer to the ramp-up reference target
    bool RampComplete;              ///< indicates when ramp-up is complete
};
typedef struct START_UP_RAMP_s START_UP_RAMP_t; 


/***********************************************************************************
 * @ingroup pwrctrl-data-types
 * @brief Stores data related to the control loop properties
 * @details This data structure manages the control loop properties. The 
 * "output" is the output of the controller, "reference" is the actual reference 
 * currently being used, and "targetReference" is the desirable reference that
 * the control loop should achieved.
 **********************************************************************************/
struct CONTROLLER_s
{
  int16_t Reference;        ///< actual reference
  uint16_t Feedback;        ///< coming 
  uint16_t Output;          ///< controller output
  int16_t AgcFactor;        ///< Adaptive gain control
  bool Enable;              ///< Enable control loop
};
typedef struct CONTROLLER_s CONTROLLER_t;

/***********************************************************************************
 * @ingroup pwrctrl-data-types
 * @brief   Enumeration of power controller charging state
 * @details
 * These is the enumeration of the psfb's charging state
 ***********************************************************************************/
enum  PWR_CTRL_CHARGE_STATE_e
{
    PWR_CTRL_CHARGING,      ///< power converter is in charging mode
    PWR_CTRL_DISCHARGING    ///< power converter is in discharging mode
};
typedef enum PWR_CTRL_CHARGE_STATE_e PWR_CTRL_CHARGE_STATE_t;

/***********************************************************************************
 * @ingroup pwrctrl-data-types
 * @brief   Collection of power control properties
 * @details This data structure can be use to initialize the desired output voltage,
 *  current and power. THe bits like charging state and if the power supply 
 *  is enabled can be monitored in this structure. 
 ***********************************************************************************/
struct PWR_CTRL_PROPERTIES_s 
{
    uint16_t VSecReference;    ///< User secondary-voltage port reference setting used to control the power converter output voltage in buck mode
    uint16_t VPriReference;    ///< User primary-voltage port reference setting used to control the power converter output voltage in boost mode
    int16_t  IReference;       ///< User current reference setting used to control the converter controller
    uint16_t PwrReference;     ///< User power reference setting used to control the  converter controller
    
    //bool ChargingState;        ///< Control Flag: When set, enables PWM output while keeping control loop disabled; Skips closed loop ramp up states and directly jumps to state Online 
    bool Precharged;            ///< placeholder for precharge. false when not precharged. true when charged
    bool Enable;               ///< Control Flag: When set, enables the converter triggering a startup sequence; When cleared, forces theS power supply to reset and turn off

};  
typedef struct PWR_CTRL_PROPERTIES_s PWR_CTRL_PROPERTIES_t;  




struct PWM_DISTRIBUTION_s {
    uint16_t PhaseShift;        // target buffer of the control output value to be distributed across PWM registers
    uint16_t* ptrPeriod;        // Pointer to the leading PWM channel period register
    uint16_t* ptrPhaseShift;    // Pointer to leading PWM channel trigger register setting the phase shift between right- and left-leg
    uint16_t* ptrDCSRR;         // Pointer to Duty Cycle register of the right-leg synchronous rectifier
    uint16_t* ptrDCSRL;         // Pointer to Duty Cycle register of the left-leg synchronous rectifier
    uint16_t* ptrUpdateReg;     // Pointer to Status register to set the UPDATE_REQUEST bit
}__attribute__((aligned));
typedef struct PWM_DISTRIBUTION_s PWM_DISTRIBUTION_t;



struct PRECHARGE_s {
    uint8_t PrechargeEnabled; // if precharge is enabled.
    uint16_t DutyCycle;     // current open loop dutycycle in percentage
    uint8_t  precharged;    // is the system precharged
    uint16_t maxDutyCycle;  // max dutycycle in percentage
    uint16_t delayCounter;   // to further slow down the rate of change of Duty Cycle
    uint16_t scaler;     // temp scaler
    uint16_t primaryVoltage;   // denominator
};
typedef struct PRECHARGE_s PRECHARGE_t;

struct SEC_REC_s {
    uint8_t SR_Enabled;         // if SR is enabled or not. enabled after start power transfer
    uint8_t SR_Flag;            // SR flag o
    uint16_t Threshold_high;     // Threhshold high
    uint16_t Threshold_low;      // Threshold Low

};
typedef struct SEC_REC_s SEC_REC_t;

/***********************************************************************************
 * @ingroup pwrctrl-data-types
 * @brief   Power control API structure
 * @details This data structure contains all the properties of the power control.  
 ***********************************************************************************/
struct POWER_CONTROL_s
{
    STATUS_FLAGS_t      Status; ///< Power Supply status flags
    PWR_CTRL_STATE_t    State;  ///< Power Control State ID
    PWR_CTRL_PROPERTIES_t Properties;    ///< Power Control properties    : clean this (no bidirectional, there will be only one reference)
    SWITCH_NODE_t       Pwm;    ///< Switch node settings
    FEEDBACK_SETTINGS_t Data;   ///< Feedback channel settings
    FAULT_SETTINGS_t    Fault;  ///< Fault flags and settings 
    START_UP_RAMP_t     IRamp;  ///< Current ramp-up settings
    START_UP_RAMP_t     VRamp;  ///< Voltage ramp-up settings
    START_UP_RAMP_t     PRamp;  ///< Power ramp-up settings
    START_UP_RAMP_t     PhRamp;  ///< Phase ramp-up settings (for open loop)
    CONTROLLER_t        ILoop;  ///< structure for current controller data
    CONTROLLER_t        VLoop;  ///< structure for voltage controller data
    CONTROLLER_t        PLoop;  ///< structure for power controller data
    PWR_CTRL_CHARGE_STATE_t    PowerDirection;  ///< defines if the power converter is in charging or discharging mode   
    uint16_t            vloop_output;
    uint16_t            iloop_output;  ///< iloop output
    uint16_t            vloop_delay;   ///< slowing down voltage loop
    PRECHARGE_t         Precharge;
    SEC_REC_t           SecRec;
    PWM_DISTRIBUTION_t PhaseShiftDistribution;  // phase shift distribution TODO: tie this uo with switch node data struct
};
typedef struct POWER_CONTROL_s POWER_CONTROL_t;

#endif	/* DEV_PWRCTRL_TYPEDEF_H */

