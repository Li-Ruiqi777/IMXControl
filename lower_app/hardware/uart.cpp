#include "uart.h"
#include <cstdint>

Uart::Uart()
{
    this->RxCacheBuffer = new uint8_t[UART_BUFFER_SIZE];
    this->TxCacheBuffer = new uint8_t[UART_BUFFER_SIZE];

    this->protocol_info = new UartProtocolInfo(this->RxCacheBuffer, this->TxCacheBuffer,
                                               &this->RxCacheBuffer[FRAME_HEAD_SIZE], this->UART_BUFFER_SIZE);
}

Uart::~Uart()
{
    delete[] this->RxCacheBuffer;
    delete[] this->TxCacheBuffer;
    delete this->protocol_info;
}

bool Uart::init(int nFd, int nBaudrate, int nDatabits, int nStopbits, std::string nParity)
{
    this->fd = nFd;
    this->baudrate = nBaudrate;
    this->databits = nDatabits;
    this->stopbits = nStopbits;
    this->parity = nParity;

    struct termios newtio;

    bzero(&newtio, sizeof(newtio));
    // 接收模式
    newtio.c_cflag |= (CLOCAL | CREAD);
    newtio.c_cflag &= ~CSIZE;

    // 设置数据位
    switch (this->stopbits)
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
    switch (this->parity[0])
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
    switch (this->baudrate)
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
    if (this->baudrate == 1)
    {
        newtio.c_cflag &= ~CSTOPB;
    }
    else if (this->baudrate == 2)
    {
        newtio.c_cflag |= CSTOPB;
    }
    // 最小等待数和最小等待时间，表示不等待
    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 1;

    tcflush(nFd, TCIFLUSH);
    if ((tcsetattr(nFd, TCSANOW, &newtio)) != 0)
    {
        return false;
    }
    return true;
}

void Uart::start()
{
    USR_DEBUG("Uart Main Task Start\n");
    int nFlag = 0;

    this->rx_thread = std::thread(
        [&]
        {
            while (true)
            {
                nFlag = this->protocol_info->CheckRxBuffer(this->fd);
                if (nFlag == RT_OK)
                {
                    USR_DEBUG("数据格式正确! \n");
                    this->protocol_info->ExecuteCommand(this->fd);
                    this->protocol_info->SendTxBuffer(this->fd);
                }
                else
                {
                    USR_DEBUG("数据格式不对! \n");
                    sleep(1);
                    // usleep(50); // 通讯结束让出线程
                }
            }
        });

    this->rx_thread.detach();
}