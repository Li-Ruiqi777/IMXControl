#include "uart_protocol_info.h"

int UartProtocolInfo::DeviceRead(int nFd, uint8_t *pDataStart, uint16_t nDataSize)
{
    return read(nFd, pDataStart, nDataSize);
}

int UartProtocolInfo::DeviceWrite(int nFd, uint8_t *pDataStart, uint16_t nDataSize)
{
    return write(nFd, pDataStart, nDataSize);
}