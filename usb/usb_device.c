#include "usb_device.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"

USBD_HandleTypeDef hUsbDeviceFS;

void MX_USB_DEVICE_Init(void)
{
	/* Init Device Library, add supported class and start the library. */
	//HAL_Delay(2000);

	USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS);

	USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC);

	USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS);

	USBD_Start(&hUsbDeviceFS);
}
