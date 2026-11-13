/* Copyright (C) 2015 Autotalks Ltd. */
#ifndef _ATLK_VCAD_H
#define _ATLK_VCAD_H

/**
   Note: This header is not final and will change in future SDK versions.
*/

#include <atlk/sdk.h>

#ifdef __cplusplus
extern "C" {
#endif

/* send VCA frames non stop */
#define VCA_CONTINUOUS_SEND 0xFFFFFFFF

/**
   Send specific amount of VCA frames over WLAN interface
   @param[in] if_index MAC interface index
   @param[in] frame_len Frame length
   @param[in] amount Amount of VCA packets to be sent
   @param[in] interval Interval between packets in milli seconds
 */
atlk_rc_t atlk_must_check
vca_send_frames(int32_t if_index,
                uint32_t frame_len,
                uint32_t amount,
                uint32_t interval);

/**
   Initialize VCA management module including specific VCA CMD to CLI and SNMP

   @retval ::ATLK_OK for success
   @return ATLK return code
 */
atlk_rc_t atlk_must_check
vca_mng_init(void);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_VCAD_H */
