/**
 * @file    dev_adc_sensor_offsets.h
 * @brief   measure current sensor offsets
 * @author  M70027
  
 */

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer data types


//#include "driver/mcc_extension/drv_custom_config_adc.h"
#include "pwrctrl/pwrctrl.h"
#include "adc/adc1.h"

/*********************************************************************************
 * @ingroup dev-sec-sys-spec-macros
 * @{
 * @brief   defines the secerature threshold raw
 **********************************************************************************/
#define MAX_NUM_SAMPLES_OFFSET_BUFFER                         64      ///< average calculated using 64 samples
#define MAX_NUM_SAMPLES_OFFSET_BUFFER_AS_EXP_OF_TWO            6      ///< for bit shifting
/** @} */ // end of dev-temp-sys-spec-macros

void dev_MeasureOffsets_Initialize(void);
void dev_MeasureOffsets(void);
uint16_t dev_Get_PrimaryCTOffset(void);
uint16_t dev_Get_SecondaryShuntOffset(void);
uint8_t dev_AreOffsetsCalculated(void);


typedef struct dev_Offsets {
    uint16_t    Offset;                        ///< temperature 
    uint16_t    Buffer[MAX_NUM_SAMPLES_OFFSET_BUFFER];   ///< buffer for calcualting average.
    uint8_t     BufferFull;                   ///< flag to monitor if full then allow averaging.
    uint16_t    ResetBuffer;                  ///< reset Temp_buffer
    uint16_t    BufferIndex;                  ///< buffer index
} dev_Offsets_t;
