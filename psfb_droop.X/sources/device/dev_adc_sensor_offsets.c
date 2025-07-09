#include "dev_adc_sensor_offsets.h"

dev_Offsets_t primary_CT_offset;
dev_Offsets_t secondary_shunt_offset;


// Private Function Call Prototypes

void dev_ReadADCSample();
void dev_AverageSamples(dev_Offsets_t * dev_off);
static uint16_t dev_CalculateAverage(uint16_t * buffer, uint16_t length);

/*********************************************************************************
 * @ingroup dev-current-sensor
 * @details acquire one sample of primary and secondary current sensor
 **********************************************************************************/
void dev_MeasureOffsets(void){
    dev_ReadADCSample();
}

/*********************************************************************************
 * @ingroup dev-current-sensor
 * @details returns averaged offset
 * @return returns averaged offset
 **********************************************************************************/
uint16_t dev_Get_PrimaryCTOffset(void){
    dev_AverageSamples(&primary_CT_offset);
    return(primary_CT_offset.Offset);
}

/*********************************************************************************
 * @ingroup dev-current-sensor
 * @details returns averaged offset
 * @return returns averaged offset
 **********************************************************************************/
uint16_t dev_Get_SecondaryShuntOffset(void){
    dev_AverageSamples(&secondary_shunt_offset);
    return(secondary_shunt_offset.Offset);
}

/*********************************************************************************
 * @ingroup dev-current-sensor
 * @details check if enough samples are acquired
 * @return 1 if buffers are full 0 if not
 **********************************************************************************/
uint8_t dev_AreOffsetsCalculated(void){
    return ((primary_CT_offset.BufferFull) & (secondary_shunt_offset.BufferFull));
}

/*********************************************************************************
 * @ingroup dev-current-sensor
 * @details intializes the offset datatypes
 **********************************************************************************/
void dev_MeasureOffsets_Initialize(void){
    primary_CT_offset.BufferFull = 0;
    primary_CT_offset.BufferIndex = 0;
    primary_CT_offset.Offset = 0;

    secondary_shunt_offset.BufferFull = 0;
    secondary_shunt_offset.BufferIndex = 0;
    secondary_shunt_offset.Offset = 0;
}

/*********************************************************************************
 * @ingroup dev-current-sensor
 * @details This function reads samples
 **********************************************************************************/
void dev_ReadADCSample(void){
    if (primary_CT_offset.BufferIndex >= (MAX_NUM_SAMPLES_OFFSET_BUFFER)) {
        primary_CT_offset.BufferFull = 1;
        primary_CT_offset.BufferIndex = 0;
    } else {
        primary_CT_offset.Buffer[primary_CT_offset.BufferIndex++] = PwrCtrl_GetAdc_Ipri_ct();
    }

    if (secondary_shunt_offset.BufferIndex >= (MAX_NUM_SAMPLES_OFFSET_BUFFER )) {
        secondary_shunt_offset.BufferFull = 1;
        secondary_shunt_offset.BufferIndex = 0;
    } else {
        secondary_shunt_offset.Buffer[secondary_shunt_offset.BufferIndex++] = PwrCtrl_GetAdc_Isec_shunt();
    } 
}

/*********************************************************************************
 * @ingroup dev-current-sensor
 * @details This function averages samples once buffer is full
 **********************************************************************************/
void dev_AverageSamples(dev_Offsets_t * dev_off) {
    if (dev_off->BufferFull){
        dev_off->Offset = dev_CalculateAverage(dev_off->Buffer, MAX_NUM_SAMPLES_OFFSET_BUFFER);
    }
} 

/*******************************************************************************
 * @ingroup dev-current-sensor
 * @brief  Averages the ADC samples
 * @return void
 * 
 * @details This function averages the  ADC samples.
 *********************************************************************************/
static uint16_t dev_CalculateAverage(uint16_t * buffer, uint16_t length) {
    uint16_t index = 0;
    uint32_t sum = 0;
    for (index = 0; index <length; index++)
        sum +=buffer[index];
    return (sum >> MAX_NUM_SAMPLES_OFFSET_BUFFER_AS_EXP_OF_TWO);
}

