#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>
#include "scheduler.h"

Scheduler::Scheduler(int no_of_tasks, std::array<float, 6> SpeedSet, std::array<int, 5> ResourceSet) {
    // Initialize taskSet
    std::random_device rd;      // for seeding the engine
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<int> arrivaltime_dist(0, 20);
    std::uniform_int_distribution<int> period_dist(1, 100);
    std::uniform_real_distribution<float> wcc_dist(0.05, 0.17);               // (WCC / Lowest_Speed) < Smallest_T, thus, WCC < (Smallest_T * Lowest_Speed)
    std::uniform_int_distribution<int> resource_dist(0, ResourceSet.size());

    std::cout << "no_of_tasks = " << no_of_tasks << std::endl << std::endl;
    for (int i=0; i<no_of_tasks; i++) {
        Task T;

        T.arrivalTime = arrivaltime_dist(generator);
        std::cout << "arrivalTime = " << T.arrivalTime << std::endl;

        T.period = period_dist(generator);
        std::cout << "period = " << T.period << std::endl;

        // TODO: Create an inverse proportion equation for preemption level, or place categorically according to period, or just scratch this and compare period
        T.preemptionLevel = (int) std::ceil(1.0 / T.period);
        std::cout << "preemptionLevel = " << T.preemptionLevel << std::endl;

        T.wcc = wcc_dist(generator);
        std::cout << "wcc = " << T.wcc << std::endl;

        T.Resource = resource_dist(generator);
        std::cout << "Resource = " << T.Resource << std::endl;

    taskSet.push_back(T);
    std::cout << std::endl;
    }
    // Sort tasks by arrivalTime in non-descending order
    SortTaskSet();

    // Initialize cpuSpeedSet
    cpuSpeedSet = SpeedSet;
    std::sort(cpuSpeedSet.begin(), cpuSpeedSet.end());

    // Initialize ResourceSet
    resourcesList = ResourceSet;
}

void Scheduler::Init(void) {
    LowSpeed = calculate_low_speed();
}

void Scheduler::Start(void) {
    while (upTime < duration) {
        nextArriveTime = taskSet[0].arrivalTime;
        upTime += (taskSet[0].arrivalTime - upTime);

        // Append task that arrived into queue
        Task temp_task = taskSet[0];
        queue.push_back(temp_task);
        SortQueue();

        // Replace the task in taskSet with the next instance
        taskSet.erase(taskSet.begin());
        temp_task.arrivalTime += temp_task.period;
        taskSet.push_back(temp_task);
        SortTaskSet();

        // If no tasks are running, run the first task on ready queue
        if (runningTask == NULL) {
            runningTask = &queue[0];
        }

    }
}

void Scheduler::SortTaskSet(void) {
    std::sort(
                taskSet.begin(), 
                taskSet.end(),
                [](const Task &lhs, const Task &rhs) {
                    return lhs.arrivalTime < rhs.arrivalTime;
                }
            );
}

void Scheduler::SortQueue(void) {
    std::sort(
                queue.begin(), 
                queue.end(),
                [](const Task &lhs, const Task &rhs) {
                    return lhs.period < rhs.period;
                }
            );
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

float Scheduler::calculate_low_speed(void) {
    float low_speed;
    float temp = 0;

    for (std::size_t i = 0; i < taskSet.size(); i++) {
        temp += (taskSet[i].wcc / taskSet[i].period);
    }

    std::cout << "temp: " << temp << std::endl;

    for (std::size_t i = 0; i < cpuSpeedSet.size(); i++) {
        std::cout << "low_speed now is: " << cpuSpeedSet[i] << std::endl;
        low_speed = cpuSpeedSet[i];
        if (temp <= low_speed)
            break;
    }

    return low_speed;
}

