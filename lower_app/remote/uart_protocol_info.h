#pragma once
#include "protocol_info.h"

class UartProtocolInfo final : public ProtocolInfo
{
public:
    UartProtocolInfo(uint8_t *pRxCachebuf, uint8_t *pTxCacheBuf, uint8_t *pRxData, uint16_t nMaxSize)
        : ProtocolInfo(pRxCachebuf, pTxCacheBuf, pRxData, nMaxSize)
    {
    }

    ~UartProtocolInfo()
    {
    }

    virtual int DeviceRead(int nFd, uint8_t *pDataStart, uint16_t nDataSize) override;
    virtual int DeviceWrite(int nFd, uint8_t *pDataStart, uint16_t nDataSize) override;
};
