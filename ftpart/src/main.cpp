#include <iostream>
#include "scheduler.h"

int main(void) {
    float duration = 100000.0;
    int no_of_tasks = 500;
    std::array<float, 6> SpeedSet = {1.0, 0.83, 0.67, 0.50, 0.33, 0.17};
    //std::array<int, 5> ResourceSet = {1, 2, 3, 4, 5};
    //Scheduler scheduler(duration, no_of_tasks, SpeedSet, ResourceSet);
    Scheduler scheduler(duration, no_of_tasks, SpeedSet);

    scheduler.Init();
    std::cout << "duration: " << scheduler.duration << std::endl;
    scheduler.Start();
    std::cout << "totalLateCount: " << scheduler.totalLateCount << std::endl;
    std::cout << "totalTaskFinished: " << scheduler.totalTaskFinished << std::endl;
    std::cout << "totalPC: " << scheduler.totalPC << std::endl;
}


