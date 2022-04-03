#include <iostream>
#include "scheduler.h"

int main(void) {
    std::cout << "OK" << std::endl;
    uint32_t no_of_tasks = 50;
    std::array<float, 7> SpeedSet = {1.0, 0.83, 0.67, 0.50, 0.33, 0.17, 0};
    std::array<uint8_t, 5> ResourceSet = {1, 2, 3, 4, 5};
    Scheduler scheduler(no_of_tasks, SpeedSet, ResourceSet);
}


