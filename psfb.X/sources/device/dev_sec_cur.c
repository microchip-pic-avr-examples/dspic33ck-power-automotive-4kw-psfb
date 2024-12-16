
/*
 * File:   dev_sec_cur.c
 * Author: M70027
 *
 * Created on Jan 17, 2024, 13:30 PM
 */


#include "dev_sec_cur.h"
#include <stdlib.h>

#include "config/hardware.h"
#include "config/macros.h"
#include "pwrctrl/pwrctrl_comm_interface.h"
#include "pwrctrl/pwrctrl.h"

/*******************************************************************************
 * @ingroup dev-temp
 * @brief Data Object of temperature settings
 * 
 * @details The 'devSecData' data object holds the settings for temperature
 *  sensor.
 *******************************************************************************/
SEC_SETTINGS_t devSecData;
SEC_SETTINGS_t* devSecDataPtr = &devSecData;

// Private Function Call Prototypes
static uint16_t Average_Sec_ADC_Samples(void);
static uint16_t Sec_Calculate_Average(uint16_t * buffer, uint16_t size);


/*******************************************************************************
 * @ingroup dev-temp
 * @brief  Initializes the temperature data structure
 * @return void
 * 
 * @details This function initializes the temperature data structure.
 *********************************************************************************/
void Dev_Sec_Initialize(void){
    devSecData.OverCurrentFlag = 0;        ///< Over temperature flag
    devSecData.SensorLost = 0;                 ///< if Sensor is present  
    devSecData.AdcReading = 0;                 ///< temperature raw
    devSecData.AdcAverage = 0;                 ///< temperature raw
}

/*******************************************************************************
 * @ingroup dev-temp
 * @brief  Executes the Temperature reading every 10ms
 * @return void
 * 
 * @details This function executes temperature reading every 10ms
 *********************************************************************************/
void Dev_Sec_Task_10ms(void){
    
    Dev_Sec_Get_ADC_Sample();
    devSecData.AdcAverage = Average_Sec_ADC_Samples();
}

/*******************************************************************************
 * @ingroup dev-temp
 * @brief  Returns the average adc temperature reading
 * @return void
 * 
 * @details This function returns the average ADC temperature reading. 
 *********************************************************************************/
uint16_t Dev_Sec_AverageValue(void){
    return (devSecData.AdcAverage);
}


/*******************************************************************************
 * @ingroup dev-temp
 * @brief  Returns the status of the Over Temperature flag
 * @return true Over temperature is detected
 * @return false Over temperature is not detected
 * 
 * @details This function returns the status of the Over Temperature flag.
 *********************************************************************************/
uint8_t Dev_Temp_Is_Over_Limit(void) {
    return devSecData.OverCurrentFlag;
}

/*******************************************************************************
 * @ingroup dev-temp
 * @brief  Gets the temperature ADC sample
 * @return void
 * 
 * @details This function gets the temperature ADC sample and adds it to the 
 *  temperature buffer for averaging use. 
 *********************************************************************************/
void Dev_Sec_Get_ADC_Sample(void) 
{
    // making sure that array does not go out of bounds. 
    if (devSecData.BufferIndex >= (MAX_NUM_SAMPLES_SEC_BUFFER - 1))   ///< store last 128 values and then average
    {      
        devSecData.BufferIndex = 0;
        devSecData.BufferFull = 1;    ///< flag to monitor if full then do averaging.
    }
        
    // use API from power controller to read temperature
    devSecData.AdcReading = PwrCtrl_GetAdc_Isec_shunt();            
    devSecData.SecBuffer[devSecData.BufferIndex++] = devSecData.AdcReading;
    
}

/*******************************************************************************
 * @ingroup dev-temp
 * @brief  Returns the temperature average value
 * @return void
 * 
 * @details This function returns the average result of the temperature ADC samples. 
 *********************************************************************************/
static uint16_t Average_Sec_ADC_Samples(void) {
    if (devSecData.BufferFull)
        return Sec_Calculate_Average(devSecData.SecBuffer, MAX_NUM_SAMPLES_SEC_BUFFER);
    else 
        return devSecData.AdcAverage;
}

/*******************************************************************************
 * @ingroup dev-temp
 * @brief  Averages the temperature ADC samples
 * @return void
 * 
 * @details This function averages the temperature ADC samples.
 *********************************************************************************/
static uint16_t Sec_Calculate_Average(uint16_t * buffer, uint16_t length) {
    uint16_t index = 0;
    uint32_t sum = 0;
    for (index = 0; index <length; index++)
        sum +=buffer[index];
    return (sum >> MAX_NUM_SAMPLES_SEC_BUFFER_AS_EXP_OF_TWO);
}

/*******************************************************************************
 * end of file
 *******************************************************************************/