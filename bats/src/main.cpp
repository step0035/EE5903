#include <iostream>
#include <fstream>
#include "scheduler.h"
#include "config.h"

int main(void) {
    float duration = 100000.0;
    int no_of_tasks = NO_OF_TASKS;
    int no_of_resources = NO_OF_RESOURCES;
    Scheduler scheduler(duration, no_of_tasks, no_of_resources);

    scheduler.Init();
    scheduler.Start();
    std::cout << "duration: " << scheduler.duration << std::endl;
    std::cout << "totalLateCount: " << scheduler.totalLateCount << std::endl;
    std::cout << "totalTaskFinished: " << scheduler.totalTaskFinished << std::endl;
    std::cout << "LowSpeed: " << scheduler.LowSpeed << std::endl;
    std::cout << "totalPC: " << scheduler.totalPC << std::endl;
    float ratio = static_cast<float> (scheduler.totalLateCount) / (static_cast<float> (scheduler.totalLateCount + scheduler.totalTaskFinished));
    std::cout << "Late task ratio: " << ratio << std::endl;

    std::ofstream csv_file;
    csv_file.open("data.csv");

    csv_file << "UpTime, Speed\n";
    for (std::size_t i = 0; i < scheduler.upTimeSeries.size(); i++) {
        csv_file << scheduler.upTimeSeries[i] << "," << scheduler.speedSeries[i] << std::endl;
    }
}


