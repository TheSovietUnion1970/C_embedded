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
 * File Name    : r_usb_hhid_apl.c
 * Description  : USB HID application code
 ******************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 22.08.2024 1.00 First Release
 ******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "r_usb_hhid_apl.h"
#include "r_usb_hhid_apl_config.h"
#include "r_usb_typedef.h"

#if (BSP_CFG_RTOS_USED)
#include "r_usb_rtos_apl.h"
#endif /*(BSP_CFG_RTOS_USED)*/

#if USB_SUPPORT_MULTI == USB_APL_DISABLE
#if OPERATION_MODE == HID_NORMAL
/******************************************************************************
Macro definitions
******************************************************************************/
#define SET_PROTOCOL            (USB_HID_SET_PROTOCOL | USB_HOST_TO_DEV | USB_CLASS | USB_INTERFACE)
#define SET_REPORT              (USB_HID_SET_REPORT   | USB_HOST_TO_DEV | USB_CLASS | USB_INTERFACE)

#define KEYBOARD_SIZE           (0x8u)
#define MOUSE_SIZE              (0x3u)

uint8_t track_id = 0;

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
static  uint8_t     g_data[8]; /* USB Receive data */
static  uint8_t     g_size;
static  uint8_t     g_hid_disp_data[DISP_CHAR_LENGTH];
static  void        apl_init(void);
static  void        set_protocol(usb_ctrl_t *, uint8_t);
static  void        usb_pin_setting(void);
static  void        set_size(usb_ctrl_t *);
static  uint32_t    get_size(void);

uint8_t     hid_receive_data (usb_ctrl_t *, uint8_t *);
static  uint8_t     hid_kbd_data (uint8_t *data);
static  void        hid_mse_data (uint8_t *data);
static  void        hid_val_to_str (uint8_t *data, int8_t val);

#if (BSP_CFG_RTOS_USED != 0)    /* Use RTOS */
static  rtos_mbx_id_t   g_usb_apl_mbx_id;
#endif /* (BSP_CFG_RTOS_USED != 0) */

/******************************************************************************
Exported global functions
 ******************************************************************************/
void    R_USB_PinSet_USB0_HOST(void);
void    R_USB_PinSet_USBA_HOST(void);

#if USB_SUPPORT_RTOS == USB_APL_ENABLE
/******************************************************************************
 Function Name   : usb_apl_callback
 Description     : Callback function for Application program
 Arguments       : usb_ctrl_t *p_ctrl   : Control structure for USB API.
                   rtos_task_id_t  cur_task  : Task Handle
                   uint8_t    usb_state : USB_ON(USB_STS_REQUEST) / USB_OFF
 Return value    : none
 ******************************************************************************/
void usb_apl_callback (usb_ctrl_t *p_ctrl, rtos_task_id_t cur_task, uint8_t usb_state)
{
    rtos_send_mailbox(&g_usb_apl_mbx_id, (void *)p_ctrl);
} /* End of function usb_apl_callback */
#endif /* USB_SUPPORT_RTOS == USB_APL_ENABLE */
/******************************************************************************
 Function Name   : hid_val_to_str
 Description     : Convert value to string.
 Argument        : uint8_t  *p_data     : Output string data
                 : int8_t   val         : Input value
 Return value    : none
 ******************************************************************************/
void hid_val_to_str (uint8_t *p_data, int8_t val)
{
    /* Output String data init */
    p_data[0] = ' ';
    p_data[1] = ' ';
    p_data[2] = ' ';
    p_data[3] = ' ';
    p_data[4] = 0x00;

    /* Minus check */
    if (val < 0)
    {
        /* Sign inversion */
        val ^= 0xff;
        val += 1;

        /* Value check hundreds place */
        if (val >= 100)
        {
            p_data[0] = '-'; /* Minus Mark set */
        }
        else
        {
            /* Value check tens place */
            if (val >= 10)
            {
                p_data[1] = '-'; /* Minus Mark set */
            }
            else
            {
                p_data[2] = '-'; /* Minus Mark set */
            }
        }
    }

    /* Value check hundreds place */
    if (val >= 100)
    {
        p_data[1] = '1'; /* Value set hundreds place */
        p_data[2] = '0'; /* Value set tens place */
        val -= 100; /* Update Value */
    }

    /* Value check tens place */
    if (val >= 10)
    {
        p_data[2] = 0x30 + (val / 10); /* Value set tens place */
        val %= 10; /* Update Value */
    }

    p_data[3] = 0x30 + val; /* Value set Place of one */
}
/******************************************************************************
End of function hid_val_to_str
/******************************************************************************
 Function Name   : hid_receive_data
 Description     : USB receive data process.
 Argument        : usb_ctrl_t   *p_ctrl     : Pointer to usb_ctrl_t structure
 Argument        : uint8_t      *p_data     : USB receive data
 Return value    : USB_TRUE/USB_FALSE       : Request LCD Display
 ******************************************************************************/
uint8_t hid_receive_data (usb_ctrl_t *p_ctrl, uint8_t *p_data)
{
    uint8_t protocol;
    uint8_t ret = USB_FALSE;

    R_USB_HhidGetType(p_ctrl, &protocol);
    if (USB_HID_KEYBOARD == protocol)   /* Check HID protocol(keyboard) */
    {
        ret = hid_kbd_data(p_data);     /* Keyboard input data process */
    }
    if (USB_HID_MOUSE == protocol)      /* Check HID protocol(Mouse) */
    {
        hid_mse_data(p_data);           /* Mouse input data process */
        ret = USB_TRUE;
    }
    return ret;
}
/******************************************************************************
End of function hid_receive_data
******************************************************************************/

/******************************************************************************
 Function Name   : hid_kbd_data
 Description     : Keyboard data receive process.
 Argument        : uint8_t *p_data      : USB Keyboard Data
 Return value    : USB_TRUE/USB_FALSE       : Request LCD Display
 ******************************************************************************/
static uint8_t hid_kbd_data (uint8_t *p_data)
{
    uint8_t ret = USB_FALSE;
    uint8_t keyboard_data = USB_NULL; /* Keyboard input valus -> Display data for debug LCD */

    g_hid_disp_data[0] = ' ';
    g_hid_disp_data[2] = '\0';

    /* Keyboard input data check */
    if (( KBD_NOT_PUSH != p_data[KBD_KEYCODE1_INDEX]) && (p_data[KBD_KEYCODE1_INDEX] != keyboard_data))
    {
        keyboard_data = p_data[KBD_KEYCODE1_INDEX];

        /* Keyboard input data check 'A' to 'Z' */
        if (keyboard_data < KBD_CODE_1)
        {
            /* ASCII CODE SET a to z */
            keyboard_data += KEYCODE_TO_ASCII_OFFSET0;
        }

        /* Keyboard input data check '1' to '9' */
        else if (keyboard_data < KBD_CODE_0)
        {
            /* ASCII CODE SET 1 to 9 */
            keyboard_data += KEYCODE_TO_ASCII_OFFSET1;
        }

        /* Keyboard input data check '0' */
        else if ( KBD_CODE_0 == keyboard_data)
        {
            /* ASCII CODE SET 0 */
            keyboard_data += KEYCODE_TO_ASCII_OFFSET2;
        }

        /* Keyboard input data check BackSpace */
        else if ( KBD_CODE_BS == keyboard_data)
        {
            /* ASCII CODE SET 'BS' */
            keyboard_data = ASCII_BS;
        }

        /* Keyboard input data check Space' ' */
        else if ( KBD_CODE_SPACE == keyboard_data)
        {
            /* ASCII CODE SET ' ' */
            keyboard_data = ' ';
        }

        /* Keyboard input data check '-' */
        else if ((KBD_CODE_HYPHEN == keyboard_data) || (KPD_CODE_HYPHEN == keyboard_data))
        {
            /* ASCII CODE SET '-' */
            keyboard_data = '-';
        }

        /* Keyboard input data check Enter */
        else if ( KBD_CODE_ENTER == keyboard_data)
        {
            /* ASCII CODE SET '\n' */
            keyboard_data = '\r';
        }

        /* Keyboard input data check ':' */
        else if ( KBD_CODE_COLON == keyboard_data)
        {
            /* ASCII CODE SET ':' */
            keyboard_data = ':';
        }

        /* Keyboard input data check '.' */
        else if ((KBD_CODE_DOT == keyboard_data) || (KPD_CODE_DOT == keyboard_data))
        {
            /* ASCII CODE SET '.' */
            keyboard_data = '.';
        }

        /* Keyboard input data check '/' */
        else if ((KBD_CODE_DIAGONAL == keyboard_data) || (KPD_CODE_DIAGONAL == keyboard_data))
        {
            /* ASCII CODE SET '/' */
            keyboard_data = '/';
        }

        /* Keyboard input data check '_' */
        else if ( KBD_CODE_UNDERSCORE == keyboard_data)
        {
            /* ASCII CODE SET '_' */
            keyboard_data = '_';
        }

        /* Keyboard input data check '\' */
        else if ( KBD_CODE_YEN == keyboard_data)
        {
            /* ASCII CODE SET '\' */
            keyboard_data = '\\';
        }

        /* Keypad input data check '1' to '9' */
        else if ((keyboard_data < KPD_CODE_END) && (keyboard_data > KPD_CODE_START))
        {
            /* ASCII CODE SET 1 to 9 */
            keyboard_data -= KEYCODE_TO_ASCII_OFFSET3;
        }

        /* Keypad input data check '0' */
        else if (KPD_CODE_END == keyboard_data)
        {
            /* ASCII CODE SET 0 */
            keyboard_data = KEYCODE_TO_ASCII_OFFSET4;
        }

        /* Keypad input data check '+' */
        else if (KPD_CODE_PLUS == keyboard_data)
        {
            /* ASCII CODE SET 0 */
            keyboard_data = '+';
        }

        /* Keypad input data check '/' */
        else
        {
            keyboard_data = '*';
        }

        if ( USB_NULL != keyboard_data)
        {
            g_hid_disp_data[1] = (uint8_t) keyboard_data;
            ret = USB_TRUE;
            printf("Key %c is pressed \n", (char)keyboard_data);
        }
    }
    return ret;
}
/******************************************************************************
End of function hid_kbd_data
******************************************************************************/

/******************************************************************************
 Function Name   : hid_mse_data
 Description     : Mouse data receive process.
 Argument        : uint8_t *p_data      : USB Mouse Data
 Return value    : none
 ******************************************************************************/
static void hid_mse_data (uint8_t *p_data)
{
    /* USB mouse move check */
    if ((MSE_NOT_MOVE != p_data[MSE_X_DATA_INDEX]) || (MSE_NOT_MOVE != p_data[MSE_Y_DATA_INDEX]))
    {
        /* Change Type to Character */
        hid_val_to_str((uint8_t *) &g_hid_disp_data[X_AXIS_POS], (int8_t) p_data[MSE_X_DATA_INDEX]);
        hid_val_to_str((uint8_t *) &g_hid_disp_data[Y_AXIS_POS], (int8_t) p_data[MSE_Y_DATA_INDEX]);
    }
    /* USB MOUSE LEFT BUTTON off/on check */
    if (USB_OFF == (p_data[MSE_BTN_DATA_INDEX] & LEFT_BUTTON) )
    {
        /* LED OFF - assign for LEFT BUTTON */
        //LED0 = LED_OFF;
        printf("USB MOUSE LEFT BUTTON off\n");
    }
    else
    {
        /* LED ON - assign for LEFT BUTTON */
        //LED0 = LED_ON;
        printf("USB MOUSE LEFT BUTTON on\n");
    }

    /* USB MOUSE RIGHT BUTTON off/on check */
    if (USB_OFF == (p_data[MSE_BTN_DATA_INDEX] & RIGHT_BUTTON) )
    {
        /* LED OFF - assign for RIGHT BUTTON */
        //LED1 = LED_OFF;
        printf("USB MOUSE RIGHT BUTTON off\n");
    }
    else
    {
        /* LED ON - assign for RIGHT BUTTON */
        //LED1 = LED_ON;
        printf("USB MOUSE RIGHT BUTTON on\n");
    }

    /* USB MOUSE WHEEL BUTTON off/on check */
    if (USB_OFF == (p_data[MSE_BTN_DATA_INDEX] & WHEEL_BUTTON) )
    {
        /* LED OFF - assign for WHEEL BUTTON */
        //LED2 = LED_OFF;
        printf("USB MOUSE WHEEL BUTTON off\n");
    }
    else
    {
        /* LED ON - assign for WHEEL BUTTON */
        //LED2 = LED_ON;
        printf("USB MOUSE WHEEL BUTTON on\n");
    }
}
/******************************************************************************
End of function hid_mse_data
/******************************************************************************
 Renesas Host Human Intergface Devices class Sample Code functions
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_main
 Description     : Host HID application main process
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void usb_main (void)
{

    uint16_t    event;
    usb_ctrl_t  ctrl;
    usb_cfg_t   cfg;


    apl_init();
    usb_pin_setting();
    
    ctrl.module     = USB_IP0;
    ctrl.type       = USB_HHID;
    cfg.usb_mode    = USB_HOST;
    cfg.usb_speed   = USB_FS;
    R_USB_Open(&ctrl, &cfg);



    while (1)
    {
        event = R_USB_GetEvent(&ctrl);
        switch (event)
        {
            case USB_STS_CONFIGURED :
                track_id++;
            	printf("USB_STS_CONFIGURED\n");

                set_size(&ctrl);

                /* Send the HID request(SetProtocol) to HID device */
                set_protocol(&ctrl, BOOT_PROTOCOL);
            break;
            
            case USB_STS_READ_COMPLETE :
            	printf("USB_STS_READ_COMPLETE\n");

                R_USB_Read(&ctrl, (uint8_t *) g_data, get_size());
                hid_receive_data(&ctrl,(uint8_t *)g_data);
            break;
            
            case USB_STS_REQUEST_COMPLETE :
            	printf("USB_STS_REQUEST_COMPLETE\n");

                if (USB_HID_SET_PROTOCOL == (ctrl.setup.type & USB_BREQUEST))
                {
                    ctrl.type = USB_HHID;
                    R_USB_Read(&ctrl, (uint8_t *) g_data, get_size());
                }
            break;
            
            default :
            break;
        }
    }
} /* End of function usb_main */

/******************************************************************************
 Function Name   : set_size
 Description     : Set the report size for HID device which is specifed by the argument
 Arguments       : usb_ctrl_t *p_ctrl   : Pointer to usb_ctrl_t structure.
 Return value    : none
 ******************************************************************************/
static void set_size (usb_ctrl_t *p_ctrl)
{
    uint16_t    mxps;

    R_USB_HhidGetMxps(p_ctrl, &mxps, USB_IN);
    g_size = (uint8_t)mxps;
}

/******************************************************************************
 Function Name   : get_size
 Description     : Get the report size for HID device which is specifed by the argument
 Arguments       : none
 Return value    : Report size
 ******************************************************************************/
static uint32_t get_size (void)
{
    return (uint32_t)g_size;
}

/******************************************************************************
 Function Name   : usb_pin_setting
 Description     : USB pin setting
 Arguments       : none
 Return value    : none
 ******************************************************************************/
static void usb_pin_setting (void)
{
#if (USE_USBIP == USE_USBIP0)
    R_USB_PinSet_USB0_HOST();
#else
    R_USB_PinSet_USBA_HOST();
#endif
} /* End of function usb_pin_setting */

/******************************************************************************
 Function Name   : set_protocol
 Description     : Sending SetProtocol request to HID device
 Arguments       : usb_ctrl_t   *p_ctrl : Pointer to usb_ctrl_t structure.
                 : uint8_t      ptorocol: Protocol Type
 Return value    : none
 ******************************************************************************/
static void set_protocol (usb_ctrl_t *p_ctrl, uint8_t protocol)
{
    p_ctrl->type            = USB_REQUEST;
    p_ctrl->setup.type      = SET_PROTOCOL; /* bRequestCode:SET_PROTOCOL, bmRequestType */
    p_ctrl->setup.value     = protocol;     /* wValue: Protocol Type */
    p_ctrl->setup.index     = 0x0000;       /* wIndex:Interface */
    p_ctrl->setup.length    = 0x0000;       /* wLength:Zero */

if (caseeeee == track_id){
    printf("set_protocol\n");
}

    R_USB_Write(p_ctrl, (uint8_t *) USB_NULL, USB_NULL); /* Request Control transfer */
} /* End of function set_protocol */

/******************************************************************************
 Function Name   : apl_init
 Description     : Application program initialization
 Argument        : none
 Return          : none
 ******************************************************************************/
static void apl_init (void)
{
} /* End of function apl_init */

#endif  /* OPERATION_MODE == HID_NORMAL */
#endif  /* USB_SUPPORT_MULTI == USB_APL_DISABLE */

/******************************************************************************
 End  Of File
 ******************************************************************************/
