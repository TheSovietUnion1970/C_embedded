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
 * File Name    : r_usb_hmsc_apl.c
 * Description  : USB Host MSC application code
 ***********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 22.08.2024 1.00 First Release
 ***********************************************************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "r_usb_hmsc_apl.h"
#include "r_usb_hmsc_apl_config.h"

#define MAX_PATH_LEN 256

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/
typedef enum
{
    RR_OK = 0, /* (0) Succeeded */
    RR_ERR /* (1) Error */
} RRESULT;

typedef enum
{
    SCAN_TREE = 0, SCAN_LIST
} SCAN_MODE;

UINT fd_temp[5];


/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/
static FRESULT scan_directory (char *path, uint8_t hierarchy, SCAN_MODE scan_mode);
static void str_hierarchy (char *out_str, uint8_t hierarchy, SCAN_MODE scan_mode);
FRESULT tree (const char *path, SCAN_MODE scan_mode);
#if USB_SUPPORT_RTOS == USB_APL_DISABLE
#if USB_SUPPORT_MULTI == USB_APL_DISABLE
/*******************************************************************************
 Typedef definitions
 ******************************************************************************/
typedef enum
{
    STATE_ATTACH, STATE_DATA_READY, STATE_DATA_WRITE, STATE_FILE_READ, STATE_DETACH, STATE_ERROR,
} state_t;

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
static  uint16_t    g_state;        /* Area to store the drive number */
static  uint8_t     g_file_data[FILE_SIZE];
static  FATFS       g_file_object;  /* File system object structure */

const   static FATFS *g_pfat = &g_file_object;
const   static uint8_t g_msc_file[15] = "0:hmscdemo.txt";
// const   static uint8_t g_msc_file_sub[15] = "0:dir/hmscdemo.txt";

static  void msc_file_write(void);
static  void msc_file_read(void);
static  void usb_pin_setting(void);
static  void apl_init(void);

/******************************************************************************
 Renesas Host MSC Sample Code functions
 ******************************************************************************/
int a = 0;
/******************************************************************************
 Function Name   : usb_main
 Description     : Host MSC application main process
 Arguments       : none
 Return value    : none
 ******************************************************************************/
uint8_t track_id;
void usb_main (void)
{
    uint16_t    event;
    usb_ctrl_t  ctrl;
    usb_cfg_t   cfg;
    const char *drv0 = "0:";
    apl_init();                 /* Application program initialization */
    usb_pin_setting();

#if ((USE_USBIP & USE_USBIP0) == USE_USBIP0)
    ctrl.module     = USB_IP0;
    ctrl.type       = USB_HMSC;
    cfg.usb_speed   = USB_FS;
    cfg.usb_mode    = USB_HOST;
    R_USB_Open(&ctrl, &cfg);
#endif

#if((USE_USBIP & USE_USBIP1) == USE_USBIP1)
    ctrl.module     = USB_IP1;
    ctrl.type       = USB_HMSC;
    cfg.usb_speed   = USB_FS;
    cfg.usb_mode    = USB_HOST;
    R_USB_Open(&ctrl, &cfg);
#endif

    track_id = 0;

    while (1)
    {
    	a++;
        event = R_USB_GetEvent(&ctrl); /* Get event code */

        switch (event)
        {
            case USB_STS_CONFIGURED :
                track_id++;
            	printf("a = %d\n", a);
                /* Create a file object. */

                //tree(drv0, SCAN_TREE);

                printf(" ================== f_mount =================== \n");
                f_mount((FATFS *) g_pfat,(const TCHAR*)"0:", 1);

                // // printf(" ================== msc_file_write =================== \n");
                // // msc_file_write();

                printf(" ================== tree =================== \n");
                // tree(drv0, SCAN_TREE);

                // FIL file;
                // FRESULT res;
                // DIR dir;

                // // Step 1: Check if the folder exists
                // res = f_opendir(&dir, "0:dir");  // Check if "dir" exists
                // if (res == FR_NO_PATH) {
                //     // Step 2: Create the folder
                //     res = f_mkdir("0:dir");
                //     if (res != FR_OK) {
                //         printf("Error creating directory: %d\n", res);
                //         return;
                //     }
                // }

                // // Step 3: Open or create the file
                // res = f_open(&file, "0:dir/hmscdemo.txt", FA_CREATE_ALWAYS | FA_WRITE);
                // if (res == FR_OK) {
                //     // File created successfully
                //     printf("File created successfully.\n");
                //     f_close(&file);  // Close the file
                // } else {
                //     // Handle error
                //     printf("Error creating file: %d\n", res);
                // }

                // UINT file_size;

                // f_open(&file, "0:dir/hmscdemo.txt", (FA_CREATE_ALWAYS | FA_WRITE));
                // f_write(&file, g_file_data, sizeof(g_file_data), &file_size);
                // f_close(&file); /* Close the file object. */

                 printf(" ================== Second time =================== \n");
                // tree(drv0, SCAN_TREE);

                // f_unlink("0:dir/hmscdemo.txt");
                // printf(" ================== Third time =================== \n");
                // tree(drv0, SCAN_TREE);

                // g_state = STATE_FILE_READ; /* Set Application status  */
            break;

            case USB_STS_DETACH :
                g_state = STATE_DETACH; /* Set Application status  */
            break;

            default :
            break;
        } /* switch( event ) */

        if (STATE_FILE_READ == g_state)
        {
            msc_file_read();
        }
    } /* while(1) */
} /* End of function usb_main */

/******************************************************************************
 Function Name   : msc_file_write
 Description     : USB receive state process
 Arguments       : none
 Return value    : none
 ******************************************************************************/
static  void msc_file_write (void)
{
    FIL file;
    UINT file_size;

    f_open(&file, (const char *) g_msc_file, (FA_CREATE_ALWAYS | FA_WRITE));
    f_write(&file, g_file_data, sizeof(g_file_data), &file_size);
    f_close(&file); /* Close the file object. */

    //fd_temp[0] = file;
} /* End of function msc_file_write */

/******************************************************************************
 Function Name   : msc_file_read
 Description     : USB receive state process
 Arguments       : none
 Return value    : none
 ******************************************************************************/
static  void msc_file_read (void)
{
    FIL file;
    UINT file_size;

    f_open(&file, (const char *) g_msc_file, (FA_OPEN_ALWAYS | FA_READ));
    f_read(&file, g_file_data, sizeof(g_file_data), &file_size); /* Read data from file. */
    f_close(&file); /* Close the file object. */

    //f_unlink();
} /* End of function msc_file_read */

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

#if((USE_USBIP & USE_USBIP1) == USE_USBIP1)
    R_USB_PinSet_USBA_HOST();
#endif
} /* End of function usb_pin_setting */

/******************************************************************************
 Function Name   : apl_init
 Description     : Application program initialization
 Argument        : none
 Return          : none
 ******************************************************************************/
static void apl_init (void)
{
    uint16_t    i;

    g_state = STATE_DETACH;

    for (i =0; i <FILE_SIZE; i++)   /* File data = '0','1','2','3'... */
    {
        g_file_data[i] = (0x30 + (i % 10));
    }

    g_state = STATE_DETACH;
    memset((void *)&g_file_object, 0, sizeof(FATFS));
} /* End of function apl_init */

/******************************************************************************
 Function Name   : usb_compliance_disp
 Description     : 
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void usb_compliance_disp (void * param)
{
}   /* End of function usb_compliance_disp() */
/**********************************************************************************************************************
 * Function Name: tree
 * Description  : Scan folders and files. This function is NOT reentrancy.
 * Arguments    : const char *path: Start node to be scanned.
 *              : SCAN_MODE scan_mode: Scan mode (Tree or List structure).
 * Return Value : The result of scanning.
 *********************************************************************************************************************/
FRESULT tree (const char *path, SCAN_MODE scan_mode)
{
    char scan_path[MAX_PATH_LEN];
    uint8_t hierarchy = 0;
    FRESULT res = FR_OK;

    if (SCAN_TREE == scan_mode)
    {
        printf("*** Scan (Tree Mode) ***\n%s\n", path);
    }
    else
    {
        printf("*** Scan (List Mode) ***\n%s\n", path);
    }

    strcpy(scan_path, path);
    res = scan_directory(scan_path, hierarchy, scan_mode);
    if (FR_OK == res)
    {
        printf("*** End of Scan ***\n");
    }
    else
    {
        printf("*** Scan Error ***\n");
    }
    return res;
}
/* End of function tree() */

/**********************************************************************************************************************
 * Function Name: scan_directory
 * Description  : Scan folders and files.
 * Arguments    : char *path: Start node to be scanned (also used as work area).
 *              : uint8_t hierarchyCnt: The count of directory hierarchy.
 *              : SCAN_MODE scan_mode: Scan mode (Tree or List structure).
 * Return Value : The result of scanning.
 *********************************************************************************************************************/
static FRESULT scan_directory (char *path, uint8_t hierarchyCnt, SCAN_MODE scan_mode)
{
    FRESULT res = FR_OK;
    DIR dir;
    UINT path_len = 0;
    static FILINFO fno;
    static char scaned_path_str[MAX_PATH_LEN];
    scaned_path_str[0] = '\0';

    res = f_opendir( &dir, path); /* Open the directory */
    if (res == FR_OK)
    {
        /* When SCAN_TREE, add one branch string at first. */
        if (SCAN_TREE == scan_mode)
        {
            hierarchyCnt++;
            str_hierarchy(scaned_path_str, hierarchyCnt, scan_mode);
        }

        while (1)
        {
            res = f_readdir( &dir, &fno); /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0)
            {
                if (SCAN_TREE == scan_mode)
                {
                    str_hierarchy(scaned_path_str, hierarchyCnt, scan_mode);
                    printf("%s\n", scaned_path_str);
                    hierarchyCnt--;
                }
                break; /* Break on error or end of dir */
            }

            if (fno.fattrib & AM_DIR)
            {
                /* It is a directory */
                path_len = strlen(path);
                /* Add '/' and the directory name to the last index of path's string('\0') */
                sprintf( &path[path_len], "/%s", fno.fname);

                if (SCAN_TREE == scan_mode)
                {
                    printf("%s%s/\n", scaned_path_str, fno.fname);
                }
                else
                {
                    printf("%s/\n", path);
                }

                res = scan_directory(path, hierarchyCnt, scan_mode); /* Enter the directory */
                if (res != FR_OK)
                {
                    break;
                }

                path[path_len] = 0;
            }
            else
            {
                /* It is a file. */
                if (SCAN_TREE == scan_mode)
                {
                    str_hierarchy(scaned_path_str, hierarchyCnt, scan_mode);
                    printf("%s%s\n", scaned_path_str, fno.fname);
                }
                else
                {
                    printf("%s/%s\n", path, fno.fname);
                }
            }   /* if (fno.fattrib & AM_DIR) */
        }   /* while (1) */
        f_closedir( &dir);
    }   /* if (res == FR_OK) */
    return res;
}
/* End of function scan_directory() */

static void str_hierarchy (char *out_str, uint8_t hierarchy, SCAN_MODE scan_mode)
{
    const static char *space = "| ";
    const static char *branch = "|-";
    uint8_t cnt;

    out_str[0] = '\0';
    for (cnt = 0; cnt < hierarchy - 1; cnt++)
    {
        strcat(out_str, space);
    }

    if (SCAN_TREE == scan_mode)
    {
        strcat(out_str, branch);
    }
    else
    {
        strcat(out_str, "  ");
    }
}
/* End of function str_hierarchy() */
#endif /* USB_SUPPORT_MULTI == USB_APL_DISABLE */
#endif /* USB_SUPPORT_RTOS == USB_APL_DISABLE */

/******************************************************************************
 End  Of File
 ******************************************************************************/
