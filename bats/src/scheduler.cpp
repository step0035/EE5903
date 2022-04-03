#include <iostream>
#include "scheduler.h"
#include <random>
#include <algorithm>

Scheduler::Scheduler(uint32_t no_of_tasks, std::array<float, 7> SpeedSet, std::array<uint8_t, 5> ResourceSet) {
    // Initialize taskSet
    uint8_t no_of_resources = ResourceSet.size();
    std::cout << "debug 1\n";

    std::default_random_engine generator;
    std::uniform_int_distribution<int> arrivaltime_dist(0, 100);
    std::uniform_int_distribution<int> period_dist(0, 100);
    std::uniform_int_distribution<int> wcc_dist(0, 100);
    std::uniform_int_distribution<int> resource_dist(0, no_of_resources);

    std::cout << "debug 2\n";
    std::cout << "no_of_tasks = " << no_of_tasks << std::endl;
    for (uint32_t i=0; i<no_of_tasks; i++) {
        std::cout << "i = " << i << std::endl;
        Task T;
        T.arrivalTime = arrivaltime_dist(generator);
        std::cout << "arrivalTime = " << T.arrivalTime << std::endl;
        T.period = period_dist(generator);
        T.wcc = wcc_dist(generator);
        T.burstTime = 0;
        T.waitTime = 0;
        T.blocked = false;

        uint8_t num_of_resources = resource_dist(generator);
        uint8_t j = 0;
    std::cout << "num_of_resources = " << num_of_resources << std::endl;
        while (j < num_of_resources) {
            uint8_t resource = resource_dist(generator);
            if (T.sharedResources.find(resource) != T.sharedResources.end()) {
                T.sharedResources.insert(resource);
                j++;
            }
        }

    taskSet.push_back(T);
    }

    // Initialize cpuSpeedSet
    cpuSpeedSet = SpeedSet;

    // Initialize ResourceSet
    resourcesList = ResourceSet;
}

void Scheduler::Init(void) {

}
