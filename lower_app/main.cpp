#include "hardware/uart.h"
#include "include/UsrTypedef.h"
#include "remote/application_reg.h"
#include <chrono>
#include <iostream>
#include <thread>

int main()
{
    // int uart_fd = open(TTY_DEVICE, O_RDWR | O_NOCTTY | O_NDELAY);
    int uart_fd = open(TTY_DEVICE, O_RDWR);
    if (uart_fd < 0)
    {
        std::cout << "Open uart failed!" << std::endl;
        return -1;
    }

    Uart uart;
    if (!uart.init(uart_fd, 115200, 8, 1, "N"))
    {
        std::cout << "Init uart failed!" << std::endl;
        return -1;
    }

    std::cout << "Open uart success!" << std::endl;
    uart.start();

    std::thread t(
        [&]()
        {
            while (1)
            {
                if (ApplicationReg::get_instance()->RefreshAllDevice() != RT_OK)
                    std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        });
    t.detach();

    while (1)
    {
    }
}