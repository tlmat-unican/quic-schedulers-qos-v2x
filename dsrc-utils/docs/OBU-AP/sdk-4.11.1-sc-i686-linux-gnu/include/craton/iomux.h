/* Copyright (C) 2014-2015 Autotalks Ltd. */
#ifndef _CRATON_IOMUX_H
#define _CRATON_IOMUX_H

#include <atlk/sdk.h>

#include <craton/check.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   CRATON IOMUX control API

   Reference: "Craton top level register file", revision 10.
*/

/** IOMUX configuration table entry */
typedef enum {
  IOMUX_TX_RX_MODE_0 = 0,
  IOMUX_TR_SW_OUT_0 = 1,
  IOMUX_PA_CNTL_0 = 2,
  IOMUX_LNAEXT_0 = 3,
  IOMUX_GPIO_28 = 4,
  IOMUX_GPIO_29 = 5,
  IOMUX_GPIO_30 = 6,
  IOMUX_GPIO_31 = 7,
  IOMUX_IRQ_0 = 8,
  IOMUX_IRQ_1 = 9,
  IOMUX_IRQ_2 = 10,
  IOMUX_IRQ_3 = 11,
  IOMUX_IRQ_4 = 12,
  IOMUX_IRQ_5 = 13,
  IOMUX_IRQ_6 = 14,
  IOMUX_IRQ_7 = 15,
  IOMUX_PA_CNTL_1 = 16,
  IOMUX_LNAEXT_1 = 17,
  IOMUX_RXD_0 = 18,
  IOMUX_RXD_1 = 19,
  IOMUX_TXD_0 = 20,
  IOMUX_TXD_1 = 21,
  IOMUX_SOR_0 = 22,
  IOMUX_SOR_1 = 23,
  IOMUX_SOR_2 = 24,
  IOMUX_SOR_3 = 25,
  IOMUX_SOR_4 = 26,
  IOMUX_SOR_5 = 27,
  IOMUX_SOR_6 = 28,
  IOMUX_SOR_7 = 29,
  IOMUX_SPI_2_FRAME = 30,
  IOMUX_SPI_2_SCLK = 31,
  IOMUX_SPI_2_TXD = 32,
  IOMUX_SPI_2_RXD = 33,
  IOMUX_RESERVED_1 = 34,
  IOMUX_TX_RX_MODE_1 = 35,
  IOMUX_TR_SW_OUT_1 = 36,
  IOMUX_RESERVED_2 = 37,
  IOMUX_RX_CAN_0 = 38,
  IOMUX_RESERVED_3 = 39,
  IOMUX_TX_CAN_0 = 40,
  IOMUX_EMD_IO = 41,
  IOMUX_ETX_0 = 42,
  IOMUX_ETX_1 = 43,
  IOMUX_ETX_EN = 44,
  IOMUX_EMDC = 45,
  IOMUX_ERX_0 = 46,
  IOMUX_ERX_1 = 47,
  IOMUX_ERXDV = 48,
  IOMUX_GPS_RF2_BB_0 = 49,
  IOMUX_GPS_RF2_BB_1 = 50,
  IOMUX_GPS_RF2_BB_2 = 51,
  IOMUX_GPS_RF2_BB_3 = 52,
  IOMUX_SCH_EXIST = 53,
  IOMUX_SCH_CCA_S = 54,
  IOMUX_CCH_TX_M = 55,
  IOMUX_CCH_CCA_M = 56,
  IOMUX_RESERVED_4 = 57,
  IOMUX_RX_CAN_1 = 58,
  IOMUX_TX_CAN_1 = 59,
  IOMUX_I2SSCK = 60,
  IOMUX_I2SWS = 61,
  IOMUX_I2SSD = 62,
  IOMUX_MAX_ENTRY = 63,

} iomux_entry_t;

/**
   Write one entry of CRATON's IOMUX configuration table.

   @remark For example, to set IOMUX_SOR_3 to 0b1 call
   iomux_write(IOMUX_SOR_3, 1).
  
   @param[in] entry IOMUX configuration table entry
   @param[in] value Value to be set for that entry

   @retval ::ATLK_OK if succeeded
   @retval ::ATLK_E_INVALID_ARG Invalid entry or value
 */
atlk_rc_t atlk_must_check
iomux_write(iomux_entry_t entry,
            uint32_t value);

#ifdef __cplusplus
}
#endif

#endif /* _CRATON_IOMUX_H */
