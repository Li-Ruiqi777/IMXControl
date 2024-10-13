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

void Uart::init(int nFd, int nBaudrate, int nDatabits, int nStopbits, std::string nParity)
{
    this->fd = nFd;
    this->baudrate = nBaudrate;
    this->databits = nDatabits;
    this->stopbits = nStopbits;
    this->parity = nParity;
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