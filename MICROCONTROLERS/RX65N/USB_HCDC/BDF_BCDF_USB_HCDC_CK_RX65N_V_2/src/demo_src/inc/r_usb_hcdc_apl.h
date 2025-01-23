/**********************************************************************************************************************
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
 *********************************************************************************************************************/
/*********************************************************************************************************************
 * File Name    : r_usb_hcdc_apl.h
 * Description  : USB Host CDC Sample Code
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 22.08.2024 1.00 First Release
 *********************************************************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "r_smc_entry.h"
#include "Pin.h"

#include <string.h>
#include "r_usb_basic_if.h"

/* Access to peripherals and board defines. */
#include "r_usb_hcdc_if.h"


/******************************************************************************
 Macro definitions
 ******************************************************************************/
#define     USE_USBIP0                  (0x01u)     /* USB0 module */
#define     USE_USBIP1                  (0x02u)     /* USB1 module */

#define     USB_APL_DISABLE             (0)
#define     USB_APL_ENABLE              (1)

/* Condition compilation by the difference of user define */
#define     CDC_DATA_LEN                (64)        /* Host CDC Application receive data size(USB_FS) */
#define     LINE_CODING_LENGTH          (7)
#define     USB_CLASS_REQEST_QUE_MAX    (6u)


#define PARITY_NONE   (USB_HCDC_PARITY_BIT_NONE)
#define PARITY_EVEN   (USB_HCDC_PARITY_BIT_EVEN)
#define PARITY_ODD    (USB_HCDC_PARITY_BIT_ODD)

#define STOP_BIT1     (USB_HCDC_STOP_BIT_1)
#define STOP_BIT15    (USB_HCDC_STOP_BIT_15)
#define STOP_BIT2     (USB_HCDC_STOP_BIT_2)

#define DATA_BIT7     (USB_HCDC_DATA_BIT_7)
#define DATA_BIT8     (USB_HCDC_DATA_BIT_8)

#define BPS_115200    (USB_HCDC_SPEED_115200)
#define BPS_57600     (USB_HCDC_SPEED_57600)
#define BPS_38400     (USB_HCDC_SPEED_38400)
#define BPS_19200     (USB_HCDC_SPEED_19200)
#define BPS_14400     (USB_HCDC_SPEED_14400)
#define BPS_9600      (USB_HCDC_SPEED_9600)

/******************************************************************************
 Typedef definitions
 ******************************************************************************/
typedef struct usb_request_que
{
    uint8_t    read_pointer;
    uint8_t    write_pointer;
    usb_ctrl_t ctrl[USB_CLASS_REQEST_QUE_MAX];
    uint8_t    *p_buf[USB_CLASS_REQEST_QUE_MAX];
    uint32_t   size[USB_CLASS_REQEST_QUE_MAX];
} usb_request_que_t;

/******************************************************************************
 Exported global variables
 ******************************************************************************/

/*****************************************************************************
 Function
 ******************************************************************************/
/* Functions */
void usb_mcu_init (usb_ctrl_t *p_ctrl);
void cdc_set_control_line_state (usb_ctrl_t *p_ctrl);
void cdc_set_line_coding (usb_ctrl_t *p_ctrl);
void cdc_get_line_coding (usb_ctrl_t *p_ctrl);
void cdc_class_request_retry (void);

/******************************************************************************
 End  Of File
 ******************************************************************************/
