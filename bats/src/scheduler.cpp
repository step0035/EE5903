#include <iostream>
#include "scheduler.h"
#include <random>
#include <algorithm>

Scheduler::Scheduler(int no_of_tasks, std::array<float, 7> SpeedSet, std::array<int, 5> ResourceSet) {
    // Initialize taskSet
    std::random_device rd;      // for seeding the engine
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<int> arrivaltime_dist(0, 20);
    std::uniform_int_distribution<int> period_dist(1, 10);
    std::uniform_int_distribution<int> wcc_dist(0, 100);
    std::uniform_int_distribution<int> resource_dist(0, ResourceSet.size());

    std::cout << "no_of_tasks = " << no_of_tasks << std::endl << std::endl;
    for (int i=0; i<no_of_tasks; i++) {
        Task T;
        T.arrivalTime = arrivaltime_dist(generator);
        std::cout << "arrivalTime = " << T.arrivalTime << std::endl;
        T.period = period_dist(generator);
        std::cout << "period = " << T.period << std::endl;
        T.wcc = wcc_dist(generator);
        std::cout << "wcc = " << T.wcc << std::endl;
        T.burstTime = 0;
        T.waitTime = 0;
        T.blocked = false;

        int num_of_resources = resource_dist(generator);
        std::cout << "num_of_resources = " << num_of_resources << std::endl;
        int j = 0;
        while (j < num_of_resources) {
            int resource = resource_dist(generator);
            if (T.sharedResources.find(resource) != T.sharedResources.end()) {
                T.sharedResources.insert(resource);
            }
            j++;
        }

    taskSet.push_back(T);
    std::cout << std::endl;
    }

    // Initialize cpuSpeedSet
    cpuSpeedSet = SpeedSet;

    // Initialize ResourceSet
    resourcesList = ResourceSet;
}

void Scheduler::Init(void) {
    std::sort(taskSet.begin(), taskSet.end());
    LCM = calculate_LCM();
}

/*
 * Private Functions
 */

long long int Scheduler::calculate_LCM(void) {
    int temp_LCM = taskSet[0].period;
    for (std::size_t i = 1; i < taskSet.size(); i++) {
        temp_LCM = (taskSet[i].period * temp_LCM) / (gcd(taskSet[i].period, temp_LCM));
    }
    return temp_LCM;
}
