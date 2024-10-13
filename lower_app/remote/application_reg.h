#pragma once
#include "../include/UsrTypedef.h"
#include <mutex>

#define REG_NUM        256
#define REG_CONFIG_NUM 64
#define REG_INFO_NUM   192

/*设备配置信息*/
#define DEVICE_LED0   1
#define DEVICE_BEEP   2
#define DEVICE_REBOOT 3

// 远程控制硬件前的中间层,先缓存接收到的数据,再由此控制硬件
class ApplicationReg
{
public:
    ApplicationReg(const ApplicationReg&) = delete;
    ApplicationReg& operator=(const ApplicationReg&) = delete;

    static ApplicationReg* get_instance();
    
    int RefreshAllDevice();
    void SetMultipleReg(uint16_t nRegIndex, uint16_t nRegSize, uint8_t *pDataStart);
    uint16_t GetMultipleReg(uint16_t nRegIndex, uint16_t nRegSize, uint8_t *pDataStart);
    int DiffSetMultipleReg(uint16_t nRegIndex, uint16_t nRegSize, uint8_t *pDataStart, uint8_t *pDataCompare);

private:
    ApplicationReg();
    ~ApplicationReg();

    static ApplicationReg *instance;

    uint8_t m_RegVal[REG_NUM];
    std::mutex m_RegMutex;
};