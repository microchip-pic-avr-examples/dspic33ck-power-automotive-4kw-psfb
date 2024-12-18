/*
    (c) 2022 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
 */

// MCC header files

// other header files
#include "App_PBV_psfb_frame_map.h"

#include "system/pins.h"
#include "device/dev_temp.h"
#include "device/dev_sec_cur.h"
#include "pwm_hs/pwm.h"

#include "pwrctrl/pwrctrl_comm_interface.h"
#include "fault/fault_comm_interface.h"
#include "pwrctrl/pwrctrl.h"
#include "config/version.h"

#include "pwrctrl/vcomp/VCOMP.h"
#include "pwrctrl/pwrctrl_pwm.h"

/*********************************************************************************
 * @ingroup pbv-protocol-ids
 * @
 * @brief   protocol ids defined. firmware id 0x1000 is fixed. the rest are arbitary
 **********************************************************************************/
#define FIRMWARE_PROTOCOL_ID              0x1000          ///< Firmware ID

#define PBV_GUI_BUTTONS                   0x202            ///< ID on which data is received Buttons.         
#define PBV_GUI_SLIDERS                   0x203            ///< ID on which data is received Sliders. 
#define PBV_SYSTEM_TO_GUI_ID              0x204             ///< ID on which data is sent
#define PBV_LOG_ID                        0x300            ///< ID on which log data is sent. 
/** @} */ // end of pbv-protocol-ids

// static because these are private.

static PBV_Datatype_TX_t App_PBV_psfb_TX;          ///< Application TX object
static PBV_Datatype_RX_t App_PBV_psfb_RX;          ///< Application RX object
static PBV_Datatype_TX_t App_PBV_psfb_ASCII;       ///< Application TX object for ascii

static PBV_Datatype_TX_t * App_PBV_psfb_TX_Ptr = &App_PBV_psfb_TX;        ///< Application TX object pointer
static PBV_Datatype_RX_t * App_PBV_psfb_RX_Ptr = &App_PBV_psfb_RX;        ///< Application RX object pointer
static PBV_Datatype_TX_t * App_PBV_psfb_ASCII_Ptr = &App_PBV_psfb_ASCII;  ///< Application TX object ascii pointer

uint8_t buffer_eight_rx[64];
uint16_t buffer_sixteen_rx[32];

uint8_t buffer_eight_tx[64];
uint16_t buffer_sixteen_tx[32];

static uint32_t tick_counter = 0;
static uint8_t transmit_firmware_id = 1;

//temporary variables
static uint16_t status_controller_state = 0 ;
static uint16_t status_input_state = 0 ;
static uint16_t status_output_state = 0 ;
static uint16_t safety_flags = 0 ;
static uint16_t slider_PS_PP = 0 ;
static uint16_t dead_time_right = 0;
static uint16_t voltage_ref = 0;
static uint16_t current_slider_ref = 0;

static uint8_t button_random_action = 0;
 static uint8_t button_start = 0;
static uint8_t button_start_sync = 0;
static int16_t charge_en = 0;

/***********************************************************************************
 * Private Functions Prototypes
 **********************************************************************************/

void App_PBV_psfb_Build_Frame(void);
void App_PBV_psfb_Process_Buttons(uint16_t * data);
void App_PBV_psfb_Process_Sliders(uint16_t * data);
void App_PBV_psfb_Frame_Parser(uint16_t protocol_ID, uint16_t length, uint8_t * data);
/***********************************************************************************
 * Public Functions Definitions
 **********************************************************************************/

/***********************************************************************************
 * @ingroup app-pbv-public-function
 * @fn      App_PBV_psfb_Init
 * @param   void
 * @return  nothing
 * @brief   this function initializes the local pbv objects. these objects are then
 *          passed on to the app_PBV_init function to initialize the CAN/UART objects
 * @details
 *          RX object just needs state change. 
 **********************************************************************************/
void App_PBV_psfb_Init()
{
    App_PBV_psfb_TX_Ptr->PBV_Protcol_ID        = PBV_SYSTEM_TO_GUI_ID;
    App_PBV_psfb_TX_Ptr->PBV_Signal_Ascii      = PBV_SIGNAL_MODE;
    App_PBV_psfb_TX_Ptr->PBV_Message_State     = PBV_MESSAGE_INIT;
    App_PBV_psfb_TX_Ptr->Length                = 64;

    App_PBV_psfb_RX_Ptr->PBV_Message_State     = PBV_MESSAGE_READY_TO_RECEIVE;

    App_PBV_psfb_ASCII_Ptr->PBV_Protcol_ID     = FIRMWARE_PROTOCOL_ID;
    App_PBV_psfb_ASCII_Ptr->PBV_Signal_Ascii   = PBV_ASCII_MODE;
    App_PBV_psfb_ASCII_Ptr->PBV_Message_State  = PBV_MESSAGE_INIT;
    App_PBV_psfb_ASCII_Ptr->Length             = 64;

    app_PBV_Init(App_PBV_psfb_TX_Ptr, App_PBV_psfb_ASCII_Ptr, App_PBV_psfb_RX_Ptr);
    
    
    ///
    
    dead_time_right = PG1DTH;
    
    current_slider_ref = DAC3DATH;
}

/***********************************************************************************
 * @ingroup app-pbv-public-function
 * @fn      App_PBV_psfb_Task_10ms
 * @param   void
 * @return  nothing
 * @brief   this is high frequency task to simulate sending of high frequency numeric data.
 *          Also to check if there are any messages received, must be done at a high enough
 *          rate to ensure that the messages are not dropped.
 * @details
 *   
 **********************************************************************************/

void App_PBV_psfb_Task_10ms(void)
{
    // Reading received message
    if (App_PBV_psfb_RX_Ptr->PBV_Message_State == PBV_MESSAGE_RECEIVED)
    {        
        app_Read_Received_From_PBV(App_PBV_psfb_RX_Ptr);       
        App_PBV_psfb_Frame_Parser(App_PBV_psfb_RX_Ptr->PBV_Protcol_ID, App_PBV_psfb_RX_Ptr->Length, App_PBV_psfb_RX_Ptr->Data_Buffer);
        
        // msg read. Read another
        app_Receive_From_PBV(App_PBV_psfb_RX_Ptr); 
    } 
    ///< 110ms sending 
    if (++tick_counter > 11)
    {
        App_PBV_psfb_Build_Frame();
        app_Send_To_PBV(App_PBV_psfb_TX_Ptr);  
        tick_counter = 0;
    }    
}

/***********************************************************************************
 * @ingroup app-pbv-public-function
 * @fn      App_PBV_psfb_Task_1s
 * @param   void
 * @return  nothing
 * @brief   this is a slow task simulating the low speed sending of ascii data
 *           it sends the firmware id, and the switches to the log id.
 * @details
 *   
 **********************************************************************************/
void App_PBV_psfb_Task_1s(void)
{
    if (App_PBV_psfb_ASCII_Ptr->PBV_Protcol_ID == FIRMWARE_PROTOCOL_ID)
    {
        App_PBV_psfb_ASCII_Ptr->Data_Buffer = "Firmware Version 1.0.0.0.......................................!";
        app_Send_To_PBV(App_PBV_psfb_ASCII_Ptr);
        App_PBV_psfb_ASCII_Ptr->PBV_Protcol_ID = PBV_LOG_ID;
        transmit_firmware_id = 1;
        return;
    }
    if (App_PBV_psfb_ASCII_Ptr->PBV_Protcol_ID == PBV_LOG_ID)
    {
        if (transmit_firmware_id) app_PBV_Re_Init(App_PBV_psfb_ASCII_Ptr);     ///< reinit to new id
        transmit_firmware_id = 0; 
    }
    App_PBV_psfb_ASCII_Ptr->Data_Buffer = "Log Message From Protocol ID 0x300.............................\r";
    app_Send_To_PBV(App_PBV_psfb_ASCII_Ptr);
   
    //average = Dev_Temp_Get_Temperature_Celcius();
}

/***********************************************************************************
 * @ingroup app-pbv-public-function
 * @fn      App_PB_psfb_Get_TX_ASCII_ptr
 * @param   void
 * @return  PBV_Datatype_TX_t *
 * @brief   this function can be used to send log messages from other files.
 * @details
 *   
 **********************************************************************************/


PBV_Datatype_TX_t * App_PB_psfb_Get_TX_ASCII_ptr(void){
    if (App_PBV_psfb_ASCII_Ptr ->PBV_Message_State == PBV_MESSAGE_TRANSMITTING)
        return 0;
    return App_PBV_psfb_ASCII_Ptr;
}

/***********************************************************************************
 * Private Functions Definitions
 **********************************************************************************/

/***********************************************************************************
 * @ingroup pbv-private-function
 * @fn      App_PBV_psfb_Build_Frame
 * @param   void
 * @return  nothing
 * @brief   this builds frame
 * @details
 * @note
 **********************************************************************************/

void App_PBV_psfb_Build_Frame()
{
    // send back one "flag word" which combines fault and status and enable control flag
    uint16_t enabled = Dev_PwrCtrl_Get_EnableFlag();
    uint16_t fault_flags = Fault_GetFlags();
    uint16_t status_flags = Dev_PwrCtrl_Get_Status();
    uint16_t flag_word = enabled + ((status_flags & 0x0003)<<1) + (fault_flags<<3);
    
    
    buffer_sixteen_tx[0] = (1<<Dev_PwrCtrl_Get_State());
    buffer_sixteen_tx[1] = flag_word;
    buffer_sixteen_tx[2] = (Dev_PwrCtrl_GetAdc_Vpri() - 202);
    buffer_sixteen_tx[3] = Dev_PwrCtrl_GetAdc_Vsec();
    buffer_sixteen_tx[4] = (uint16_t)(PwrCtrl_GetAdc_Isec_shunt() - 610);
    buffer_sixteen_tx[5] = Dev_PwrCtrl_GetVoltage_Vcap();
    buffer_sixteen_tx[6] = (uint16_t)(PwrCtrl_GetAdc_Temperature() - 497 );
    buffer_sixteen_tx[7] = PwrCtrl_GetAdc_Vrail_5V();
    buffer_sixteen_tx[8] = PG1TRIGC;
    buffer_sixteen_tx[9] =  PwrCtrl_GetAdc_Ipri_ct();
    buffer_sixteen_tx[10] = PwrCtrl_GetAdc_Isec_shunt();
    buffer_sixteen_tx[11] = Dev_PwrCtrl_GetControl_Phase();
    buffer_sixteen_tx[12] = FAULT_EN_GetValue() + (button_start_sync<<1) + (VCOMP.status.bits.enabled<<2);
    buffer_sixteen_tx[13] = dead_time_right;
    buffer_sixteen_tx[14] = psfb_ptr->Properties.VSecReference;
    buffer_sixteen_tx[15] = current_slider_ref - 621;
    buffer_sixteen_tx[16] = DAC3DATH;
    buffer_sixteen_tx[17] = psfb_ptr->VLoop.Reference;
    buffer_sixteen_tx[18] = DAC1DATH;
    buffer_sixteen_tx[19] = psfb_ptr->controller_error;
    
    
    PBV_Change_from_Sixteen_to_Eight(buffer_sixteen_tx, buffer_eight_tx, 20);
    
    App_PBV_psfb_TX_Ptr->Data_Buffer = buffer_eight_tx;
    App_PBV_psfb_TX_Ptr->Length = 20 * 2 ;
}


/***********************************************************************************
 * @ingroup pbv-private-function
 * @fn      App_PBV_psfb_Process_Buttons
 * @param   void
 * @return  nothing
 * @brief   this processes buttons
 * @details
 * @note
 **********************************************************************************/
void App_PBV_psfb_Process_Buttons(uint16_t * data) {
    uint16_t switchcase = data[0];
    switch (switchcase) {
        case 0x0001:
            PwrCtrl_SetEnable(true);
            break;
        case 0x0000:
            PwrCtrl_SetEnable(false);
            FAULT_EN_SetLow();
            break;
        case 0x0101:
            button_start_sync = 1;
//            PG2CONLbits.ON = 1;
//            PG4CONLbits.ON = 1;
            PG4IOCONLbits.OVRENL = 0;
            PG2IOCONLbits.OVRENL = 0;
            break;
        case 0x0100:
            button_start_sync = 0;
            PG4IOCONLbits.OVRENL = 1;
            PG2IOCONLbits.OVRENL = 1;
//            PG2CONLbits.ON = 0;
//            PG4CONLbits.ON = 0;
            break;
        case 0xaa00:
//            Dev_Reset_Average_Buffer();
            break;
//        case 0xFFFF:
//            button_random_action ^= 1;
////            dev_fan_data_ptr->override_flag  ^= 1;
////            break;
        case 0x00aa:
            FAULT_EN_SetHigh();
            charge_en = 1;
            break;
        case 0x00bb:
            FAULT_EN_SetLow();
            charge_en = 0;
            break;
            
        case 0xFFFF:
            VCOMP.status.bits.enabled = 1;
            break;
        
        case 0xFF00:
             VCOMP.status.bits.enabled = 0;
            break;
        default:
            break;
    }
//    status_controller_state = button_start | (button_start_sync << 1) | (button_random_action << 2)| (dev_fan_data_ptr->override_flag << 3) ;
}

/***********************************************************************************
 * @ingroup pbv-private-function
 * @fn      App_PBV_psfb_Process_Buttons
 * @param   void
 * @return  nothing
 * @brief   this processes sliders
 * @details
 * @note
 **********************************************************************************/
void App_PBV_psfb_Process_Sliders(uint16_t * data) {
    uint16_t switchcase = data[0];
    uint16_t temp = 0;
    
    Nop();
    Nop();
    Nop();
    Nop();
    
    switch (switchcase) {
        case 0xaa:
            if (VCOMP.status.bits.enabled) 
                break;
            else {
                slider_PS_PP = data[1];
//            PWM_DutyCycleSet(4, PG4PER - (data[1] * 112));
//            PWM_TriggerACompareValueSet(1, data[1] * 112);
//            PwrCtrl_SetPhaseTarget(data[1] * 112);

            // AR-241126: Extended above timing computation and update in 
            //            accordance with new PWM daisy chain configuration
            /*
            PWM_TriggerCCompareValueSet(1, data[1] * 112);
            PWM_DutyCycleSet(4, PG4PER - (data[1] * 112));
            PWM_DutyCycleSet(2, PG2PER - (data[1] * 112));
            PwrCtrl_SetPhaseTarget(data[1] * 112);
            */
            Nop();
            Nop();
            Nop();
            
            PhaseShiftDistribution.PhaseShift = (data[1] * 112);
            
            PwrCtrl_PWM_Update(&PhaseShiftDistribution);
            
            PWM_TriggerBCompareValueSet(1, data[1] * 56);
            
            // PG1STATbits.UPDREQ = 1; // Set manual update (can be automated later)
            }
            break;

        case 0xbb:
            //current slider ref -- DACDATH update here.
            dead_time_right = data[1];
//            temp = 0.5 + (data[1] * 0.18);
//            temp = ( temp * 4095 )/3.3 ; 
//            current_dac_value = temp;
//            DAC1DATH = temp;
                PG1DTH = data[1];   // set dead time to 150ns
                PG1DTL = data[1];   // set dead time to 150ns

                PG3DTH = data[1];   // set dead time to 150ns
                PG3DTL = data[1];   // set dead time to 150ns
//                PG2DTH = 0;     // no dead time on PWMxH
//                PG2DTL = 1000;  // set dead time to 250ns
//                PG3DTH = data[1];   // set dead time to 150ns
//                PG3DTL = data[1];   // set dead time to 150ns
//                PG4DTH = 0;     // no dead time on PWMxH
//                PG4DTL = 1000;  // set dead time to 250ns
                PG1STATbits.UPDREQ = 1; // Set manual update (can be automated later)

            break;
            
        case 0xcc:
            voltage_ref = data[1];
            PwrCtrl_SetVSecReference(voltage_ref);

            break;
        
        case 0xdd:
            //current slider ref -- DACDATH update here.
            temp = data[1];
//            temp = temp * 0.18;
//            temp = 0.5 + temp;
//            temp = ( temp * 4095 )/3.3 ; 
//            temp = data[1];
//            temp = temp * 1241;
            current_slider_ref = temp;
            DAC3DATH = temp;
            break;
//            dev_fan_data_ptr->target_speed_percent = data[1];
        default:
            break;
    }
}
/***********************************************************************************
 * @ingroup pbv-private-function
 * @fn      App_PBV_psfb_Frame_Parser
 * @param   void
 * @return  nothing
 * @brief   default callback
 * @details
 * @note
 **********************************************************************************/
void App_PBV_psfb_Frame_Parser(uint16_t protocol_ID, uint16_t length, uint8_t * data){
    PBV_Change_from_Eight_to_Sixteen(data, buffer_sixteen_rx, length);
    switch (protocol_ID){
        case PBV_GUI_BUTTONS:
            App_PBV_psfb_Process_Buttons(buffer_sixteen_rx);
            break;
        case PBV_GUI_SLIDERS:
            App_PBV_psfb_Process_Sliders(buffer_sixteen_rx);
            break;
    }
}


/*******************************************************************************
 * end of file
 *******************************************************************************/