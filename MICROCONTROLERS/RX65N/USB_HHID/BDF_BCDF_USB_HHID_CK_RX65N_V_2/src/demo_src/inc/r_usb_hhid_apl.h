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
/*******************************************************************************
 * File Name    : r_usb_hhid_apl.h
 * Description  : USB HID application code
 ******************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 22.08.2024 1.00 First Release
 ******************************************************************************/


 /******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "r_smc_entry.h"
#include "Pin.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Access to peripherals and board defines. */

#include "r_usb_basic_if.h"
#include "r_usb_hhid_if.h"
#include "r_usb_hhid.h"

/******************************************************************************
 Macro definitions
 ******************************************************************************/
#define HID_NORMAL                          (0)             /* NORMAL mode */
#define HID_DEMO                            (1)             /* DEMO mode */
#define HID_ECHO                            (2)             /* Loop back(Echo) mode */

#define USE_USBIP0                          (0x01)
#define USE_USBIP1                          (0x02)

#define     USB_APL_DISABLE                 (0)
#define     USB_APL_ENABLE                  (1)

#define USB_CLASS_REQEST_QUE_MAX            (6u)

#define MAX_DEVICE_ADDRESS                  (4 +1)

/* Set Report (Keyborad LED ) */
#define NUM_LOCK                            (0x01)
#define CAPS_LOCK                           (0x02)
#define SCROLL_LOCK                         (0x04)
#define KBD_LED_CLEAR                       (0x00)

/* LCD Display Position */
#define X_AXIS_POS                          (0u)            /* Mouse X axis */
#define Y_AXIS_POS                          (4u)            /* Mouse Y axis */

#define LINE1                               (LCD_POS_U0)    /* Line 1 */
#define LINE2                               (LCD_POS_D0)    /* Line 2 */
#define LINE3                               (LINE2 + 8)     /* Line 3 */
#define LINE4                               (LINE3 + 8)     /* Line 4 */
#define LINE5                               (LINE4 + 8)     /* Line 5 */
#define LINE6                               (LINE5 + 8)     /* Line 6 */
#define LINE7                               (LINE6 + 8)     /* Line 7 */
#define LINE8                               (LINE7 + 8)     /* Line 8 */
#define LINE9                               (LINE8 + 8)     /* Line 9 */

#define DISP_CHAR_LENGTH                    (8+1)           /* LCD Display 8characters +USB_NULL */

/* Mouse Button Defines */
#define LEFT_BUTTON                         (0x01)          /* Left Button */
#define RIGHT_BUTTON                        (0x02)          /* Right Button */
#define WHEEL_BUTTON                        (0x04)          /* Wheel Button */

/* LED Position for USB Mouse */
#define LED_POS_LEFT_BTN                    (0x00)          /* Left Button */
#define LED_POS_RIGHT_BTN                   (0x01)          /* Right Button */
#define LED_POS_WHEEL_BTN                   (0x02)          /* Wheel Button */

/* USB Mouse no movement */
#define MSE_NOT_MOVE                        (0x00)

/* Mouse data Store Position Index */
#define MSE_BTN_DATA_INDEX                  (0x00)          /* Input Key */
#define MSE_X_DATA_INDEX                    (0x01)          /* X-axis */
#define MSE_Y_DATA_INDEX                    (0x02)          /* Y-axis */

#define KBD_NOT_PUSH                        (0x00)          /* USB Keyboard no key input */

/* Keyboard data Store Position Index */
#define KBD_KEYCODE1_INDEX                  (0x02)          /* Input Key */

/* USB HID Keyboard Usage ID */
#define KBD_CODE_1                          (0x1E)          /* 1 */
#define KBD_CODE_0                          (0x27)          /* 0 */
#define KBD_CODE_ENTER                      (0x28)          /* '\n' */
#define KBD_CODE_BS                         (0x2A)          /* 'BS' */
#define KBD_CODE_SPACE                      (0x2C)          /* ' ' */
#define KBD_CODE_HYPHEN                     (0x2D)          /* '-' */
#define KBD_CODE_COLON                      (0x34)          /* ':' */
#define KBD_CODE_DOT                        (0x37)          /* '.' */
#define KBD_CODE_DIAGONAL                   (0x38)          /* '/' */
#define KBD_CODE_UNDERSCORE                 (0x87)          /* '_' */
#define KBD_CODE_YEN                        (0x89)          /* '\' */
#define KPD_CODE_START                      (0x58)          /* Keypad Number 1 */
#define KPD_CODE_END                        (0x62)          /* Keypad Number 0 */
#define KPD_CODE_DIAGONAL                   (0x54)          /* Keypad '/' */
#define KPD_CODE_HYPHEN                     (0x56)          /* Keypad '-' */
#define KPD_CODE_PLUS                       (0x57)          /* Keypad '+' */
#define KPD_CODE_DOT                        (0x63)          /* Keypad '.' */

/* Keycode to ASCII code offset */
#define KEYCODE_TO_ASCII_OFFSET0            (0x3D)          /* 'A'-'Z' */
#define KEYCODE_TO_ASCII_OFFSET1            (0x13)          /* '1'-'9' */
#define KEYCODE_TO_ASCII_OFFSET2            (0x09)          /* '0' */
#define KEYCODE_TO_ASCII_OFFSET3            (0x28)          /* Keypad '1' - '9' */
#define KEYCODE_TO_ASCII_OFFSET4            (0x30)          /* Keypad '0' */
#define ASCII_BS                            (0x08)          /* BackSpace */

/* Set Protocol */
#define BOOT_PROTOCOL                       (0)
#define REPORT_PROTOCOL                     (1)

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

/*****************************************************************************
 Enumerated Types
 ******************************************************************************/

/*****************************************************************************
 Exported global functions (to be accessed by other files)
 ******************************************************************************/
/* r_usb_hhid_apl.c */
void usb_main (void);

/* r_usb_hhid_apl_data.c */

/******************************************************************************
 End  Of File
 ******************************************************************************/
