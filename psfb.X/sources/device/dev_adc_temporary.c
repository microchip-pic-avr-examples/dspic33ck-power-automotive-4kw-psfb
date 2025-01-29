#include "dev_adc_temporary.h"

void callback(enum ADC_CHANNEL channel, uint16_t adcVal);

void Dev_ADC_Sampling_Task_1ms (void){
    // Enable the ADC sampling
    ADC1_SoftwareTriggerEnable();
    
//    if(ADC1_IsConversionComplete(FB_P_CT_FILT))
//        psfb_ptr->.Data.ISensePrimary = ADC1_ConversionResultGet(FB_P_CT_FILT); 
//    
//    if(ADC1_IsConversionComplete(I_SEC_AVG_FILT))
//        psfb_ptr->Data.ISenseSecondary = ADC1_ConversionResultGet(I_SEC_AVG_FILT); 
    
    if(ADC1_IsConversionComplete(FB_VOUT))
        psfb_ptr->Data.VOutVoltage = ADC1_ConversionResultGet(FB_VOUT); 
    
    if(ADC1_IsConversionComplete(FB_VCAP))
        psfb_ptr->Data.VCapVoltage = ADC1_ConversionResultGet(FB_VCAP);   
    
    if(ADC1_IsConversionComplete(VIN_INT_AN))
        psfb_ptr->Data.VInVoltage = ADC1_ConversionResultGet(VIN_INT_AN);
    
    if(ADC1_IsConversionComplete(FB_TEMP))
        psfb_ptr->Data.Temperature = ADC1_ConversionResultGet(FB_TEMP);
    
    if(ADC1_IsConversionComplete(Channel_AN15)){
        psfb_ptr->Data.Channel_AN15 = ADC1_ConversionResultGet(Channel_AN15);
    }
    
    if(ADC1_IsConversionComplete(FB_5V)){
        psfb_ptr->Data.VRail_5V = ADC1_ConversionResultGet(FB_5V); 
    }
    
   // PG6DC = ADC1_ConversionResultGet(FB_VOUT) * 5;
}

//void callback(enum ADC_CHANNEL channel, uint16_t adcVal){
//    if (channel == I_SEC_AVG_FILT)
//       psfb_ptr->Data.ISenseSecondary = adcVal;
//    }
//
////void __attribute__ ( ( __interrupt__ , auto_psv ) ) _ADCAN1Interrupt ( void )
////{
////    uint16_t valI_SEC_AVG_FILT;
////    //Read the ADC value from the ADCBUF
////    valI_SEC_AVG_FILT = ADCBUF1;
////
////    psfb_ptr->Data.ISenseSecondary = valI_SEC_AVG_FILT;
////    
////    //clear the I_SEC_AVG_FILT interrupt flag
////
////    IFS5bits.ADCAN1IF = 0;
////}
//
//
//void Dev_ADC_init(){
//    ADC1_ChannelCallbackRegister(callback);
//}