#include <iostream>
#include "scheduler.h"

int main(void) {
    float duration = 500.0;
    int no_of_tasks = 50;
    int no_of_resources = 5;
    std::array<float, 6> SpeedSet = {1.0, 0.83, 0.67, 0.50, 0.33, 0.17};
    //std::array<int, 5> ResourceSet = {1, 2, 3, 4, 5};
    //Scheduler scheduler(duration, no_of_tasks, SpeedSet, ResourceSet);
    Scheduler scheduler(duration, no_of_tasks, SpeedSet, no_of_resources);

    scheduler.Init();
    //std::cout << "LCM: " << scheduler.LCM << std::endl;
    std::cout << "LowSpeed: " << scheduler.LowSpeed << std::endl;
    std::cout << "duration: " << scheduler.duration << std::endl;
    scheduler.Start();
}


