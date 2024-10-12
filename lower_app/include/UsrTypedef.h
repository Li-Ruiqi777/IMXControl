#pragma once
/***************************************************************************
* Include Header Files
***************************************************************************/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>

/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define __SYSTEM_DEBUG          0
#define __DEBUG_PRINTF			1
#define __DRIVER_DEBUG          1
#define __SOCKET_DEBUG          1

/*返回状态*/
#define RT_OK               	0x00
#define RT_FAIL             	0x01
#define RT_EMPTY            	0x02
#define RT_TIMEOUT              0x03
#define RT_INVALID              0x04

/**************************************************************************
* Global Variable Declaration
***************************************************************************/
//DEVICE
#define TTY_DEVICE  "/dev/ttymxc4"
#define LED_DEVICE  "/dev/gpio_led"
#define BEEP_DEVICE "/dev/beep"

//IPAddress
#define IP_ADDR     "127.0.0.1"
#define PORT        8000

//UART
#define BAUD        115200
#define DATABITS    8
#define PARITY      'N'
#define STOPBITS    1

/**************************************************************************
* Global Functon Declaration
***************************************************************************/
/*调试接口*/
#if __DEBUG_PRINTF	== 1
#define USR_DEBUG		printf
#else
static int USR_DEBUG(const char *format, ...){}
#endif

#if __DRIVER_DEBUG == 1
#define DRIVER_DEBUG		USR_DEBUG
#else
static int DRIVER_DEBUG(const char *format, ...){}
#endif

#if __SOCKET_DEBUG == 1
#define SOCKET_DEBUG        USR_DEBUG
#else
static int SOCKET_DEBUG(const char *format, ...){}
#endif

void SystemLogArray(uint8_t *pArrayBuffer, uint16_t nArraySize);

