
/**
 * @file      fault_comm_interface.c
 * @ingroup   fault 
 * @brief     Contains API functions for fault protection.
 */

#include "fault_common.h"
#include "pwrctrl/pwrctrl.h"

enum FAULTLOG_FLAGS_e {
    FLT_IPRI_OCP,
    FLT_ISEC_OCP,
    FLT_ISNS_SCP,
    FLT_VPRI_OVP,
    FLT_VPRI_UVP,
    FLT_VSEC_OVP,
    FLT_VSEC_UVP,
    FLT_psfb_TEMP,
    FLT_RAIL_5V,
    FLT_LOAD_DISCONNECT,
    RESET
};
typedef enum FAULTLOG_FLAGS_e FAULTLOG_FLAGS_t;

extern bool loadDisconnect;

uint16_t Fault_GetFlags(void)
{
    uint16_t fault = 0;
    
    // Concatenate the Fault status in one for PBV 
    fault = (psfb_ptr->Fault.Object.IPrimaryOCP.FaultLatch << FLT_IPRI_OCP) +
            (psfb_ptr->Fault.Object.ISecondaryOCP.FaultLatch << FLT_ISEC_OCP) +
            (psfb_ptr->Fault.Object.ISenseSCP.FaultLatch << FLT_ISNS_SCP) +
            (psfb_ptr->Fault.Object.VPrimaryOVP.FaultLatch << FLT_VPRI_OVP) +
            (psfb_ptr->Fault.Object.VPrimaryUVP.FaultLatch << FLT_VPRI_UVP) +
            (psfb_ptr->Fault.Object.VSecondaryOVP.FaultLatch << FLT_VSEC_OVP) +
            (psfb_ptr->Fault.Object.VSecondaryUVP.FaultLatch << FLT_VSEC_UVP) +
            (psfb_ptr->Fault.Object.PowerSupplyOTP.FaultLatch << FLT_psfb_TEMP) +
            (psfb_ptr->Fault.Object.VRail_5V.FaultLatch << FLT_RAIL_5V) +
            (loadDisconnect << FLT_LOAD_DISCONNECT) + 
            (psfb_ptr->Fault.Object.ISenseSCP.FaultLatch << RESET);
            
    return(fault);        
}
