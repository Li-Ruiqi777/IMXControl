#include "led.h"
#include "../include/UsrTypedef.h"

void LED_Status_Convert(uint8_t nLedStatus)
{
    int nFd;
    uint8_t nVal;

    DRIVER_DEBUG("led write:%d\n", nLedStatus);
    nFd = open(LED_DEVICE, O_RDWR | O_NDELAY);
    if(nFd != -1)
    {
        nVal = nLedStatus;
        write(nFd, &nVal, 1);  //将数据写入LED
        close(nFd);
    }
}