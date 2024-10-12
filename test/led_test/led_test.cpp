#include "../../lower_app/hardware/led.h"
#include <thread>

#define LEDOFF 0
#define LEDON 1

int main()
{
    LED_Status_Convert(LEDOFF);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    LED_Status_Convert(LEDON);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    LED_Status_Convert(LEDOFF);
}
