#include <iostream>
#include "hardware/uart.h"


int main()
{
    int uart_fd = open(TTY_DEVICE, O_RDWR | O_NOCTTY | O_NDELAY);
    if(uart_fd < 0)
    {
        std::cout << "Open uart failed!" << std::endl;
        return -1;
    }

    std::cout << "Open uart success!" << std::endl;

    Uart uart;
    uart.init(uart_fd, 115200, 8, 1, "N");
    uart.start();
    
    while (1) {
    
    }

}