#include "scheduler.h"
#include <random>
#include <algorithm>

void Scheduler::Scheduler(uint32_t no_of_tasks, float *SpeedSet, uint8_t no_of_speeds, uint8_t *ResourceSet, uint8_t no_of_resources) {
    // Initialize taskSet
    std::default_random_engine generator;
    std::uniform_int_distribution<int> arrivaltime_dist(0, 100);
    std::uniform_int_distribution<int> period_dist(0, 100);
    std::uniform_int_distribution<int> wcc_dist(0, 100);
    std::uniform_int_distribution<int> resource_dist(0, no_of_resources);

    for (uint8_t i=0; i<no_of_tasks; i++) {
        Task T;
        T.arrivalTime = arrivaltime_dist(generator);
        T.period = period_dist(generator);
        T.wcc = wcc_dist(generator);
        T.burstTime = 0;
        T.waitTime = 0;
        T.blocked = false;

        uint8_t num_of_resources = resource_dist(generator);
        uint8_t j = 0;
        while (j < num_of_resources) {
            uint8_t resource = resource_dist(generator);
            if (T.sharedResource.find(resource) != T.sharedResource.end()) {
                T.sharedResource.insert(resource);
                j++
            }
        }

    taskSet.push_back(T);
    }

    // Initialize cpuSpeedSet
    std::sort(std::begin(SpeedSet), std::end(SpeedSet));
    cpuSpeedSet = SpeedSet;

    // Initialize ResourceSet
    resourcesList = ResourceSet;
}

void Scheduler::Init(void) {

}
