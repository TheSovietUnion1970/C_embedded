/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_usb_hcdc_apl_config.h
 * Description  : USB Host CDC program configuration file.
 ***********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 22.08.2024 1.00 First Release
 ***********************************************************************************************************************/

#ifndef R_USB_HCDC_APL_CONFIG_H
#define R_USB_HCDC_APL_CONFIG_H

/******************************************************************************
 User define macro definitions
 ******************************************************************************/

/** [USB module selection setting]
 *  USE_USBIP0              : Uses USB0 module
 *  USE_USBIP1              : Uses USB1 module
 *  USE_USBIP0 | USE_USBIP1 : Uses USB0 and USB1 modules
 */
#define USE_USBIP       (USE_USBIP0)

/** [dwDTERate]
 *  BPS_9600                : 9600bps
 *  BPS_14400               : 14400bps
 *  BPS_19200               : 19200bps
 *  BPS_38400               : 38400bps
 *  BPS_57600               : 57600bps
 *  BPS_115200              : 115200bps
 */
#define COM_SPEED       (BPS_9600)

/** [bParityType]
 *  PARITY_NONE             : None Parity
 *  PARITY_EVEN             : Odd Parity
 *  PARITY_ODD              : Even Parity
 */
#define COM_PARITY_BIT  (PARITY_NONE)

/** [bCharFormat]
 *  STOP_BIT1               : 1Stop bits
 *  STOP_BIT15              : 1.5Stop bits
 *  STOP_BIT2               : 2Stop bits
 */
#define COM_STOP_BIT    (STOP_BIT1)

/** [bDataBits]
 *  DATA_BIT7               : 7Data bits
 *  DATA_BIT8               : 8Data bits
 */
#define COM_DATA_BIT    (DATA_BIT8)

/** [Select Support Multi Device]
 *  USB_APL_ENABLE        : Support Multi Device
 *  USB_APL_DISABLE       : Not support Multi Device(Support Single device)
 */
#define USB_SUPPORT_MULTI   (USB_APL_DISABLE)

/** [Select Support Free RTOS]
 *  USB_APL_ENABLE        : Support Free RTOS
 *  USB_APL_DISABLE       : Not support Free RTOS(Support non OS)
 */
#define USB_SUPPORT_RTOS    (USB_APL_DISABLE)

#endif  /* R_USB_HCDC_APL_CONFIG_H */
/******************************************************************************
 End  Of File
 ******************************************************************************/

