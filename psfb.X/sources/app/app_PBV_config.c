/* 
 * @file    app_PBV_config.c   
 * @author  M70027
 * @group    app-pbv-config
 * @brief   Power Board Visualizer application configuration 
 */
#include "config/comms_config.h"

#include "app_PBV_config.h"

#if (PBV_UART == 1)
#include "app_PBV_UART.h"
#endif

#if (PBV_CANFD == 1)
#include "app_PBV_CAN.h"
#endif

/***********************************************************************************
 * @ingroup app-pbv-config
 * @brief   structure of API function pointers. 
 **********************************************************************************/
const APP_PBV_INTF_API_t APP_PBV_FUNCS = {

#if (PBV_UART == 1)
    PBV_UART_Init,
    PBV_UART_Receive_from_GUI,
    PBV_UART_Transmit_to_GUI,
    PBV_UART_Transmit_Ascii_to_GUI,
    PBV_UART_Reinit,
    PBV_UART_Link_Data_TX,
    PBV_UART_Link_Data_RX
#endif

#if (PBV_CANFD == 1)
    PBV_CAN_Init,
    PBV_CAN_Receive_from_GUI,
    PBV_CAN_Transmit_to_GUI,
    PBV_CAN_Transmit_Ascii_to_GUI,
    PBV_CAN_Reinit,
    PBV_CAN_Link_Data_TX,
    PBV_CAN_Link_Data_RX
#endif
};
