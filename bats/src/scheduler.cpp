#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>
#include "scheduler.h"

Scheduler::Scheduler(float Duration, int no_of_tasks, std::array<float, 6> SpeedSet, int no_of_resources) {
    // Set duration
    duration = Duration;

    // Initialize resourceList
    for (int i = 0; i < no_of_resources; i++) {
        Resource r;
        resourceList.push_back(r);
    }

    // Initialize taskSet
    std::random_device rd;      // for seeding the engine
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<int> arrivaltime_dist(0, 20);
    std::uniform_int_distribution<int> period_dist(1, 100);
    std::uniform_real_distribution<float> wcc_dist(0.05, 0.17);               // (WCC / Lowest_Speed) < Smallest_T, thus, WCC < (Smallest_T * Lowest_Speed)
    std::uniform_int_distribution<int> resource_dist(0, resourceList.size() - 1);

    std::cout << "no_of_tasks = " << no_of_tasks << std::endl << std::endl;
    for (int i=0; i < no_of_tasks; i++) {
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

        T.resource = resourceList[static_cast<std::size_t> (resource_dist(generator))];
        //std::cout << "Resource = " << T.Resource << std::endl;

    taskSet.push_back(T);
    std::cout << std::endl;
    }

    // Sort tasks by arrivalTime in non-descending order
    SortTaskSet();

    // Initialize cpuSpeedSet
    cpuSpeedSet = SpeedSet;
    std::sort(cpuSpeedSet.begin(), cpuSpeedSet.end());
}

void Scheduler::Init(void) {
    // Assign index to the tasks
    for (std::size_t i = 0; i < taskSet.size(); i++) {
        taskSet[i].index = i;
    }

    // Backup initial task set
    initialTaskSet = taskSet;
    LowSpeed = calculate_low_speed();
}

void Scheduler::Start(void) {
    while (upTime < duration) {
        std::cout << "upTime: " << upTime << std::endl;
        nextArriveTime = (float) taskSet[0].arrivalTime;    // Next task arrive at this time
        float execTime = nextArriveTime - upTime;           // Execution time while waiting for next task arrival

        // Run until next task arrival or until current task finish, whichever comes first
        if (runningTask != NULL) {
            float estFinishTime = runningTask->rc / currentSpeed;    // Time taken to finish the current task
            if (estFinishTime < execTime) {
                execTime = estFinishTime;
                currentTaskFinished = true;
            }
            runningTask->burstTime += execTime;
            runningTask->rc -= (currentSpeed * execTime);
        }

        // TODO: Debug why upTime becomes negative
        upTime += execTime;

        if (currentTaskFinished) {
            std::cout << "Task " << runningTask->index << " finished\n";
            runningTask = NULL;
        }

        // There may be multiple tasks arriving at the same time
        // TODO: After task finished, need to check blocked queue for eligible tasks
        while (taskSet[0].arrivalTime <= nextArriveTime) {
            // Append task that arrived into queue
            Task temp_task = taskSet[0];
            queue.push_back(temp_task);
            SortQueue();

            // Replace the task in taskSet with the next instance
            taskSet.erase(taskSet.begin());
            temp_task.arrivalTime += temp_task.period;
            taskSet.push_back(temp_task);
            SortTaskSet();
            
            currentTaskFinished = false;
        }

        // TODO: From here onwards, consider moving to a private function called ContextSwitch
        // If no tasks are running, run the first task on ready queue
        if (runningTask == NULL) {
            runningTask = &queue[0];
            queue.erase(queue.begin());
        }
        // check queue if any task fit the criteria to preempt the current task
        // 1. It is the highest priority task in the queue (compare using period)
        // 2. It has a preemption level higher than system ceiling (compare using period)
        // 3. TODO: Check for blocking
        else {  
            if (queue[0].period < runningTask->period) {
                if (queue[0].period < systemCeiling) {     // Check for blocking 
                    queue.push_back(*runningTask);
                    runningTask = &queue[0];
                    SortQueue();
                }
                // TODO: First candidate task is blocked, check if another task in the queue is eligible to preempt
                else {
                    // Calculate high speed
                    float newSpeed = calculate_high_speed(queue[0]);
                    // Set speed to high if calculated high speed is higher than currentSpeed
                    if (newSpeed > currentSpeed) {
                        currentSpeed = newSpeed;
                        // TODO: Switch back to low speed after blocked task's deadline or when cpu become idle, whichever first
                    }
                }
            }
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

float Scheduler::calculate_low_speed(void) {
    float low_speed;
    float target_speed = 0;

    for (std::size_t i = 0; i < taskSet.size(); i++) {
        target_speed += (taskSet[i].wcc / taskSet[i].period);
    }

    for (std::size_t i = 0; i < cpuSpeedSet.size(); i++) {
        std::cout << "low_speed now is: " << cpuSpeedSet[i] << std::endl;
        low_speed = cpuSpeedSet[i];
        if (target_speed <= low_speed)
            break;
    }

    return low_speed;
}

float Scheduler::calculate_high_speed(Task T) {
    int period = T.period;
    float B = (runningTask->wcc - runningTask->rc) / currentSpeed;          // Remaining computation of critical section
    float sum_of_product = 0;

    for (std::size_t i = 0; i < initialTaskSet.size(); i++) {
        float product = std::floor(period / initialTaskSet[i].period) * initialTaskSet[i].wcc;
        sum_of_product += product;
    }

    float high_speed;
    float target_speed = (B + sum_of_product) / period;

    for (std::size_t i = 0; i < cpuSpeedSet.size(); i++) {
        std::cout << "high_speed now is: " << cpuSpeedSet[i] << std::endl;
        high_speed = cpuSpeedSet[i];
        if (target_speed <= high_speed)
            break;
    }

    return high_speed;
}
