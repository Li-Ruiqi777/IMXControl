#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <termios.h>
#include <unistd.h>

const char *UART_DEVICE_NAME = "/dev/ttymxc4";
const uint32_t UART_BUFFER_SIZE = 256;
uint8_t nCacheBuffer[256];

static int set_opt(int nFd, int nBaud, int nDataBits, std::string cParity, int nStopBits)
{
    struct termios newtio;
    struct termios oldtio;

    tcgetattr(nFd, &oldtio);
    bzero(&newtio, sizeof(newtio));
    // 接收模式
    newtio.c_cflag |= (CLOCAL | CREAD);
    newtio.c_cflag &= ~CSIZE;

    // 设置数据位
    switch (nDataBits)
    {
    case 7:
        newtio.c_cflag |= CS7;
        break;
    case 8:
        newtio.c_cflag |= CS8;
        break;
    default:
        break;
    }
    // 设置奇偶校验位
    switch (cParity[0])
    {
    case 'O':
    case 'o':
        newtio.c_cflag |= PARENB;
        newtio.c_cflag |= PARODD;
        newtio.c_iflag |= (INPCK | ISTRIP);
        break;
    case 'E':
    case 'e':
        newtio.c_iflag |= (INPCK | ISTRIP);
        newtio.c_cflag |= PARENB;
        newtio.c_cflag &= ~PARODD;
        break;
    case 'N':
    case 'n':
        newtio.c_cflag &= ~PARENB;
        break;
    }
    // 设置波特率
    switch (nBaud)
    {
    case 9600:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    case 115200:
        cfsetispeed(&newtio, B115200);
        cfsetospeed(&newtio, B115200);
        break;
    default:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    }
    // 设置停止位
    if (nStopBits == 1)
    {
        newtio.c_cflag &= ~CSTOPB;
    }
    else if (nStopBits == 2)
    {
        newtio.c_cflag |= CSTOPB;
    }
    // 最小等待数和最小等待时间，表示不等待
    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 1;

    tcflush(nFd, TCIFLUSH);
    if ((tcsetattr(nFd, TCSANOW, &newtio)) != 0)
    {
        return -1;
    }
    return 0;
}

int main()
{
    int fd;
    ssize_t nLen;
    std::cout << "start uart test" << std::endl;
    /*开启串口模块*/
    // fd = open(UART_DEVICE_NAME, O_RDWR | O_NOCTTY | O_NONBLOCK);
    fd = open(UART_DEVICE_NAME, O_RDWR);

    if (fd > 0)
    {
        std::cout << "open success!" << std::endl;
        if (set_opt(fd, 115200, 8, "n", 1) == 0)
        {
            std::cout << "setting success!" << std::endl;
            while (1)
            {
                /*从串口缓冲区中读出数据，并写入到发送缓冲区*/
                nLen = read(fd, nCacheBuffer, UART_BUFFER_SIZE);
                if (nLen > 0)
                {
                    std::cout << "receive data!" << std::endl;
                    write(fd, nCacheBuffer, nLen);
                }
            }
        }
    }
    else
    {
        std::cout << "open failed!" << std::endl;
    }
    close(fd);
    std::cout << "end uart test" << std::endl;
    return 0;
}
