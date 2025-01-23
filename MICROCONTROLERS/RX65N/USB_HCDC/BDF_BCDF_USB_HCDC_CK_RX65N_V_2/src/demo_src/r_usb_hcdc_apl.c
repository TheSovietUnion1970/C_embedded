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
 * File Name    : r_usb_hcdc_apl.c
 * Description  : USB Host CDC Sample Code
 *******************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 22.08.2024 1.00 First Release
 ******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_hcdc_apl.h"
#include "r_usb_hcdc_apl_config.h"

#if USB_SUPPORT_RTOS == USB_APL_ENABLE
#include "r_usb_rtos_apl.h"
#include "r_rtos_abstract.h"
#include "r_usb_cstd_rtos.h"
#if (BSP_CFG_RTOS_USED == 4)        /* Renesas RI600V4 & RI600PX */
#include "kernel_id.h"
#endif /* (BSP_CFG_RTOS_USED == 4) */
#endif /* USB_SUPPORT_RTOS == USB_APL_ENABLE */

#if USB_SUPPORT_MULTI == USB_APL_DISABLE
/******************************************************************************
 Macro definitions
 ******************************************************************************/
#define SET_LINE_CODING             (USB_CDC_SET_LINE_CODING | USB_HOST_TO_DEV | USB_CLASS | USB_INTERFACE)
#define GET_LINE_CODING             (USB_CDC_GET_LINE_CODING | USB_DEV_TO_HOST | USB_CLASS | USB_INTERFACE)
#define SET_CONTROL_LINE_STATE      (USB_CDC_SET_CONTROL_LINE_STATE | USB_HOST_TO_DEV | USB_CLASS | USB_INTERFACE)


/******************************************************************************
Typedef definitions
******************************************************************************/
typedef struct
{
    usb_setup_t           setup;    /* Class Specific Notification */
    uint8_t             bitmap[2]; /* UART State bitmap */
} serial_state_t;

/******************************************************************************
Private global variables and functions
 ******************************************************************************/
static usb_hcdc_linecoding_t   g_com_parm;               /* Set Line Coding parameter */
static serial_state_t          g_serial_state;           /* SerialState */
static uint8_t                 g_data[CDC_DATA_LEN + 4];

void set_control_line_state(usb_ctrl_t *p_ctrl);
static void set_line_coding(usb_ctrl_t *p_ctrl);
void get_line_coding(usb_ctrl_t *p_ctrl);
static void cdc_serial_state(void);
static void usb_pin_setting(void);
static void apl_init(void);

#if (BSP_CFG_RTOS_USED != 0)        /* Use RTOS */
static  rtos_mbx_id_t   g_usb_apl_mbx_id;
#endif /* (BSP_CFG_RTOS_USED != 0) */

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/
void    R_USB_PinSet_USB0_HOST(void);
void    R_USB_PinSet_USBA_HOST(void);

/******************************************************************************
 Renesas USB Host CDC Sample Code functions
 ******************************************************************************/

#if USB_SUPPORT_RTOS == USB_APL_ENABLE
/******************************************************************************
 Function Name   : usb_apl_callback
 Description     : Callback function for Application program
 Arguments       : usb_ctrl_t      *p_ctrl   : Control structure for USB API.
                   rtos_task_id_t  cur_task  : Task Handle
                   uint8_t         usb_state : USB_ON(USB_STS_REQUEST) / USB_OFF
 Return value    : none
 ******************************************************************************/
void usb_apl_callback (usb_ctrl_t *p_ctrl, rtos_task_id_t cur_task, uint8_t usb_state)
{
    rtos_send_mailbox(&g_usb_apl_mbx_id, (void *)p_ctrl);
} /* End of function usb_apl_callback */
#endif /* USB_SUPPORT_RTOS == USB_APL_ENABLE */

#define DATA_LENGTH 64
uint8_t tx_buffer[DATA_LENGTH] = "Hello from RX65N!";
uint8_t rx_buffer[DATA_LENGTH];

/******************************************************************************
 Function Name   : usb_main
 Description     : Host CDC application main process
 Arguments       : none 
 Return value    : none
 ******************************************************************************/
uint8_t track_id;
void usb_main (void)
{
    uint16_t    event;
    usb_ctrl_t  ctrl;
    usb_cfg_t   cfg;

    apl_init();                 /* Host CDC Sample APL Initialize Command Send */
    usb_pin_setting();

    ctrl.module     = USB_IP0;
    ctrl.type       = USB_HCDC;
    cfg.usb_speed   = USB_FS;
    cfg.usb_mode    = USB_HOST;

    R_USB_Open(&ctrl, &cfg); /* Initializes the USB module */

    while (1)
    {
            event   = R_USB_GetEvent(&ctrl);
            switch (event)
            {
                case USB_STS_CONFIGURED :
                    track_id++;
                    printf(" ========================================= Done configuring  ======================\n");
                    set_line_coding(&ctrl); /* CDC Class request "SetLineCoding" */
                break;

                case USB_STS_READ_COMPLETE :
                    printf("USB_STS_READ_COMPLETE\n");
                    if (USB_HCDC == ctrl.type)
                    {
                        if (ctrl.size > 0)
                        {
                            printf("    tx_buffer - 17\n");
                            /* Send the received data to USB Host */
            				R_USB_Write(&ctrl, (uint8_t *)tx_buffer, 17);
                        }
                        else
                        {
                            printf("    g_data - CDC_DATA_LEN\n");
                            /* Send the data reception request when the zero-length packet is received. */
                            R_USB_Read(&ctrl, (uint8_t *)g_data, CDC_DATA_LEN);
                        }
                    }
                    else
                    /* USB_HCDCC */
                    {
                        printf("    cdc_serial_state\n");
                        cdc_serial_state();

                        /* Class notification "SerialState" receive start */
                        ctrl.type = USB_HCDCC;
                        R_USB_Read(&ctrl, (uint8_t *)&g_serial_state, USB_HCDC_SERIAL_STATE_MSG_LEN);
                    }
                break;

                case USB_STS_WRITE_COMPLETE :
                    printf("USB_STS_WRITE_COMPLETE\n");
                    ctrl.type = USB_HCDC;
                    /* Report receive start */
                    R_USB_Read(&ctrl, (uint8_t *)g_data, CDC_DATA_LEN);
                break;

                case USB_STS_REQUEST_COMPLETE :
                    printf("USB_STS_REQUEST_COMPLETE\n");
                    /* Check Complete request "SetLineCoding" */
                    if (USB_CDC_SET_LINE_CODING == (ctrl.setup.type & USB_BREQUEST))
                    {
                        printf("    status - SetLineCoding\n");
                        /* Class notification "SerialState" receive start */
                        ctrl.type = USB_HCDCC;
                        set_control_line_state(&ctrl); /* CDC Class request "SetControlLineState" */
                    }
                    /* Check Complete request "SetControlLineState" */
                    else if (USB_CDC_SET_CONTROL_LINE_STATE == (ctrl.setup.type & USB_BREQUEST))
                    {
                        printf("    status - SetControlLineState\n");
                        ctrl.type = USB_HCDC;
                        get_line_coding(&ctrl); /* CDC Class request "SetLineCoding" */

                        /* Data receive start */
                        ctrl.type = USB_HCDC;
                        R_USB_Read(&ctrl, (uint8_t *) &g_data, CDC_DATA_LEN);

                        /* Class notification "SerialState" receive start */
                        ctrl.type = USB_HCDCC;
                        R_USB_Read(&ctrl, (uint8_t *)&g_serial_state, USB_HCDC_SERIAL_STATE_MSG_LEN);
                    }
                    else
                    { /* Not support request */
                    }
                break;

                default : /* Other event */
                break;
            }
    }
} /* End of function usb_main */

/******************************************************************************
 Function Name   : set_control_line_state
 Description     : Send SetControlLineState request to CDC device.
 Arguments       : p_ctrl : Pointer to usb_ctrl_t structure 
 Return value    : none
 ******************************************************************************/
void set_control_line_state (usb_ctrl_t *p_ctrl)
{
    p_ctrl->type            = USB_REQUEST;
    p_ctrl->setup.type      = SET_CONTROL_LINE_STATE;   /* bRequestCode:SET_CONTROL_LINE_STATE, bmRequestType */
    p_ctrl->setup.value     = 0x0003;                   /* wValue:Control Signal Bitmap */
    p_ctrl->setup.index     = 0x0000;                   /* wIndex:Interface */
    p_ctrl->setup.length    = 0x0000;                   /* wLength:Zero */
    R_USB_Write(p_ctrl, (uint8_t *) USB_NULL, USB_NULL); /* Request Control transfer */
} /* End of function cdc_set_control_line_state */

/******************************************************************************
 Function Name   : set_line_coding
 Description     : Send SetLineCoding request to CDC device.
 Arguments       : p_ctrl : Pointer to usb_ctrl_t structure 
 Return value    : none
 ******************************************************************************/
static void set_line_coding (usb_ctrl_t *p_ctrl)
{
    g_com_parm.dwdte_rate   = (uint32_t)COM_SPEED;
    g_com_parm.bdata_bits   = COM_DATA_BIT;
    g_com_parm.bchar_format = COM_STOP_BIT;
    g_com_parm.bparity_type = COM_PARITY_BIT;

    p_ctrl->type            = USB_REQUEST;
    p_ctrl->setup.type      = SET_LINE_CODING;  /* bRequestCode:SET_LINE_CODING, bmRequestType */
    p_ctrl->setup.value     = 0x0000; /* wValue:Zero */
    p_ctrl->setup.index     = 0x0000; /* wIndex:Interface */
    p_ctrl->setup.length    = LINE_CODING_LENGTH; /* Data:Line Coding Structure */

    /* Request Control transfer */
    R_USB_Write(p_ctrl, (uint8_t *)&g_com_parm, LINE_CODING_LENGTH);
} /* End of function cdc_set_line_coding */

/******************************************************************************
 Function Name   : get_line_coding
 Description     : Send GetLineCoding request to CDC device.
 Arguments       : p_ctrl : Pointer to usb_ctrl_t structure 
 Return value    : none
 ******************************************************************************/
void get_line_coding (usb_ctrl_t *p_ctrl)
{
    p_ctrl->type            = USB_REQUEST;
    p_ctrl->setup.type      = GET_LINE_CODING;      /* bRequestCode:GET_LINE_CODING, bmRequestType */
    p_ctrl->setup.value     = 0x0000;               /* wValue:Zero */
    p_ctrl->setup.index     = 0x0000;               /* wIndex:Interface */
    p_ctrl->setup.length    = LINE_CODING_LENGTH;   /* Data:Line Coding Structure */

    /* Request Control transfer */
    R_USB_Read(p_ctrl, (uint8_t *)&g_com_parm, LINE_CODING_LENGTH);
} /* End of function cdc_get_line_coding */

/******************************************************************************
 Function Name   : cdc_serial_state
 Description     : Class Notification Serial State
 Arguments       : none
 Return value    : none
 ******************************************************************************/
static void cdc_serial_state (void)
{
    usb_hcdc_serialstate_t bitmap;   /* UART State bitmap */

    *(uint16_t *)&bitmap = (uint16_t)g_serial_state.bitmap[0] | ((uint16_t)g_serial_state.bitmap[1] << 8);
    if (bitmap.BIT.bover_run)
    {
        /* none */
    }
    if (bitmap.BIT.bparity)
    {
        /* none */
    }
    if (bitmap.BIT.bframing)
    {
        /* none */
    }
    if (bitmap.BIT.bbreak)
    {
        /* none */
    }
} /* End of function cdc_serial_state */

/******************************************************************************
 Function Name   : usb_pin_setting
 Description     : USB pin setting processing
 Arguments       : none
 Return value    : none
 ******************************************************************************/
static void usb_pin_setting (void)
{
#if ((USE_USBIP & USE_USBIP0) == USE_USBIP0)
    R_USB_PinSet_USB0_HOST();
#endif
#if ((USE_USBIP & USE_USBIP1) == USE_USBIP1)
    R_USB_PinSet_USBA_HOST();
#endif
} /* End of function usb_pin_setting */


/******************************************************************************
 Function Name   : apl_init
 Description     : Send host CDC sample application init message.
 Argument        : none
 Return          : none
 ******************************************************************************/
static void apl_init (void)
{
    memset((void *)&g_data, 0, (CDC_DATA_LEN + 4));
    memset((void *)&g_serial_state, 0, sizeof(serial_state_t));
    memset((void *)&g_com_parm, 0, sizeof(usb_hcdc_linecoding_t));
} /* End of function apl_init */

#endif /* USB_SUPPORT_MULTI == USB_APL_DISABLE */
/******************************************************************************
 End  Of File
 ******************************************************************************/
