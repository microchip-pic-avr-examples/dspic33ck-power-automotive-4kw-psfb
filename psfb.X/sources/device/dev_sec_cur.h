/**
 * @file      dev_sec_cur.h
 * @ingroup   dev-sec_cur  
 * @brief     Contains secondary shunt current public functions and macros.
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef DEV_SEC_CUR_H
#define	DEV_SEC_CUR_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

/*********************************************************************************
 * @ingroup dev-sec-cur-abstraction-macros
 * @
 * @brief   
 **********************************************************************************/
#include "adc/adc1.h"   

/*********************************************************************************
 * @ingroup dev-sec-sys-spec-macros
 * @{
 * @brief   defines the secerature threshold raw
 **********************************************************************************/
#define MAX_NUM_SAMPLES_SEC_BUFFER                         64      ///< average calculated using 64 samples
#define MAX_NUM_SAMPLES_SEC_BUFFER_AS_EXP_OF_TWO            6      ///< for bit shifting
/** @} */ // end of dev-temp-sys-spec-macros

/***********************************************************************************
 * @ingroup dev-sec-data-type
 * @brief   A custom data type for temperature measurement
 **********************************************************************************/
struct SEC_SETTINGS_s {
    uint8_t     OverCurrentFlag;          ///< Over temperature flag
    uint8_t     SensorLost;                   ///< if sensor is present      
    uint16_t    AdcReading;                   ///< temperature value raw sampled from ADC
    uint16_t    AdcAverage;                   ///< temperature 
    uint16_t    SecBuffer[MAX_NUM_SAMPLES_SEC_BUFFER];   ///< buffer for calcualting average.
    uint16_t    BufferFull;                   ///< flag to monitor if full then allow averaging.
    uint16_t    ResetBuffer;                  ///< reset Temp_buffer
    uint16_t    BufferIndex;                  ///< buffer index
};
typedef struct SEC_SETTINGS_s SEC_SETTINGS_t;

/*********************************************************************************
 * @ingroup dev-temp-data-type
 * @brief   dev temp data object pointer external
 * @details pointer to the Object 'dev_Temp_data_t', providing access to all control
 *          and monitoring values. To access it, add the header file to the project, and 
 *          then the pointer can be used to access the dev_temp_data_type members.
 **********************************************************************************/
extern SEC_SETTINGS_t* devSecDataPtr;
extern SEC_SETTINGS_t secTempData;

/***********************************************************************************
 * Public Function Call Prototypes
 **********************************************************************************/
void Dev_Sec_Initialize(void);
void Dev_Sec_Task_10ms(void);
uint16_t Dev_Sec_AverageValue(void);
uint8_t Dev_Sec_Is_Over_Limit();
void Dev_Sec_Get_ADC_Sample(void); 
#endif	/* DEV_SEC_CUR_H */

