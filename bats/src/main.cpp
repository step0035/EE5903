#include <iostream>
#include "scheduler.h"

int main(void) {
    float duration = 100000.0;
    int no_of_tasks = 50;
    int no_of_resources = 1;
    Scheduler scheduler(duration, no_of_tasks, no_of_resources);

    scheduler.Init();
    scheduler.Start();
    std::cout << "duration: " << scheduler.duration << std::endl;
    std::cout << "totalLateCount: " << scheduler.totalLateCount << std::endl;
    std::cout << "totalTaskFinished: " << scheduler.totalTaskFinished << std::endl;
    std::cout << "LowSpeed: " << scheduler.LowSpeed << std::endl;
    std::cout << "totalPC: " << scheduler.totalPC << std::endl;
}


