/**
 * @file    dev_adc_temporary.h
 * @brief   fan device driver
 * @author  M70027
 * @date    1/10/2024
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef DEV_ADC_TEM_H
#define	DEV_ADC_TEM_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // include standard integer data types


#include "driver/mcc_extension/drv_custom_config_adc.h"
#include "pwrctrl/pwrctrl.h"

void Dev_ADC_Sampling_Task_1ms(void);

void Dev_ADC_init(void);

#endif	