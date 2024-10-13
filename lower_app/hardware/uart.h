#pragma once
#include "../remote/uart_protocol_info.h"
#include <string>
#include <thread>

class Uart
{
public:
    Uart();
    ~Uart();

    bool init(int nFd, int nBaudrate, int nDatabits, int nStopbits, std::string nParity);
    void start();
    void stop();

private:
    int fd;
    int baudrate;
    int databits;
    int stopbits;
    std::string parity;

    const int UART_BUFFER_SIZE = 1024;
    uint8_t *RxCacheBuffer;
    uint8_t *TxCacheBuffer;
    ProtocolInfo *protocol_info;

    std::thread rx_thread;
};
