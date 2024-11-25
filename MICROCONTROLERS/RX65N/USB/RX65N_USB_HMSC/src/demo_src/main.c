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
/**********************************************************************************************************************
 * File Name    : main.c
 * Description  : main process
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 22.08.2024 1.00 First Release
 *********************************************************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "r_usb_hmsc_apl.h"
#include "r_usb_hmsc_apl_config.h"

#if USB_SUPPORT_RTOS == USB_APL_ENABLE
#include "r_usb_typedef.h"
#include "r_usb_rtos_apl.h"
#include "r_rtos_abstract.h"
#endif /* USB_SUPPORT_RTOS == USB_APL_ENABLE */

st_usb0_t* USBB = (volatile struct st_usb0*)0xA0000;
// st_sci10_t* SPI6 = (volatile struct st_sci10*)0x8A0C0;



/******************************************************************************
Exported global variables (to be accessed by other files)
 ******************************************************************************/
extern void usb_main(void);

void PinSet_USB0_HOST();

#if USB_SUPPORT_RTOS == USB_APL_ENABLE
/******************************************************************************
 Function Name   : main_task
 Description     : Main task
 Arguments       : none
 Return value    : none
 ******************************************************************************/
#if (BSP_CFG_RTOS_USED == 4)        /* Renesas RI600V4 & RI600PX */
void main_task (VP_INT b)
#else  /* (BSP_CFG_RTOS_USED == 4) */
void main_task (void)
#endif /* (BSP_CFG_RTOS_USED == 4) */
{
    usb_main(); /* USB sample application */
}
/******************************************************************************
 End of function main()
 ******************************************************************************/
/******************************************************************************
 Function Name   : usb_apl_rec_msg
 Description     : Receive a message to the specified id (mailbox).
 Argument        : uint8_t id        : ID number (mailbox).
                 : usb_msg_t** mess  : Message pointer
                 : usb_tm_t tm       : Timeout Value
 Return          : uint16_t          : USB_OK / USB_ERROR
 ******************************************************************************/
usb_er_t usb_apl_rec_msg (rtos_mbx_id_t *p_id, usb_msg_t **pp_mess, usb_tm_t tm)
{
    rtos_err_t err;

    (void *)tm;

    if (NULL == pp_mess)
    {
        return USB_APL_ERROR;
    }

    *pp_mess  = NULL;

    /** Receive message from queue specified by *(mbx_table[id]) **/
    err = rtos_receive_mailbox(p_id, (void **)pp_mess, RTOS_FOREVER);
    if ((RTOS_SUCCESS == err) && (NULL != (*pp_mess)))
    {
        return USB_APL_OK;
    }
    else
    {
        return USB_APL_ERROR;
    }
}
/******************************************************************************
 End of function usb_apl_rec_msg
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_apl_snd_msg
 Description     : Send a message to the specified id (mailbox).
 Argument        : uint8_t id        : ID number (mailbox).
                 : usb_msg_t* mess   : Message pointer
 Return          : usb_er_t          : USB_OK / USB_ERROR
 ******************************************************************************/
usb_er_t usb_apl_snd_msg (rtos_mbx_id_t *p_id, usb_msg_t *p_mess)
{
    rtos_err_t err;

    if (NULL == p_mess)
    {
        return USB_APL_ERROR;
    }

    /** Send message to queue specified by *(mbx_table[id]) **/
    err = rtos_send_mailbox(p_id, (void *)p_mess);
    if (RTOS_SUCCESS == err)
    {
        return USB_APL_OK;
    }
    else
    {
        return USB_APL_ERROR;
    }
}
/******************************************************************************
 End of function usb_apl_snd_msg
 ******************************************************************************/

#else   /* USB_SUPPORT_RTOS == USB_APL_ENABLE */

/******************************************************************************
 Function Name   : main
 Description     : Main task
 Arguments       : none
 Return value    : none
 ******************************************************************************/

void main (void)
{
    usb_main(); /* USB sample application */

}
/******************************************************************************
 End of function main()
 ******************************************************************************/

#endif /* USB_SUPPORT_RTOS == USB_APL_ENABLE */

/******************************************************************************
 End  Of File
 ******************************************************************************/

/******************************************************************************
 User function
 ******************************************************************************/
void PinSet_USB0_HOST()
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);

    /* Set USB0_VBUSEN pin */
    MPC.P24PFS.BYTE = 0x13U;
    PORT2.PMR.BIT.B4 = 1U;

    /* Set USB0_OVRCURA pin */
    MPC.P14PFS.BYTE = 0x12U;
    PORT1.PMR.BIT.B4 = 1U;

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}
