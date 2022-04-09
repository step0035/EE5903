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
        r.index = i;
        resourceList.push_back(r);
    }

    // Initialize taskSet
    std::random_device rd;
    //std::default_random_engine generator(rd());     // use this for randomized seeding
    std::default_random_engine generator(1);
    std::uniform_int_distribution<int> arrivaltime_dist(0, 1000);
    std::uniform_int_distribution<int> period_dist(20, 5000);
    //std::uniform_real_distribution<float> wcc_dist(1, 5);               // (WCC / Lowest_Speed) < Smallest_T, thus, WCC < (Smallest_T * Lowest_Speed), choose 0.15 as max
    std::uniform_int_distribution<int> resource_dist(0, resourceList.size() - 1);

    std::cout << "no_of_tasks = " << no_of_tasks << std::endl << std::endl;
    for (int i=0; i < no_of_tasks; i++) {
        Task T;

        T.arrivalTime = arrivaltime_dist(generator);
        //std::cout << "arrivalTime = " << T.arrivalTime << std::endl;

        T.period = period_dist(generator);
        //std::cout << "period = " << T.period << std::endl;

        std::uniform_real_distribution<float> wcc_dist(0.5, 5.0);
        // TODO: Create an inverse proportion equation for preemption level, or place categorically according to period, or just scratch this and compare period
        //T.preemptionLevel = (int) std::ceil(1.0 / T.period);
        //std::cout << "preemptionLevel = " << T.preemptionLevel << std::endl;

        T.wcc = wcc_dist(generator);
        //std::cout << "wcc = " << T.wcc << std::endl;

        T.rc = T.wcc;

        T.resource = &resourceList[static_cast<std::size_t> (resource_dist(generator))];
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
    SortTaskSet_byPeriod();

    LowSpeed = calculate_low_speed();
    currentSpeed = LowSpeed;

    // Assign resource ceiling for all the resources
    InitResources();
}

void Scheduler::Start(void) {
    while (upTime < duration) {
        std::cout << "upTime: " << upTime << std::endl;

        if (currentSpeed > LowSpeed)
            std::cout << "================================================HIGHER THAN LOW SPEED===============================================\n";

        float execTime = calculate_exec_time();

        // TODO: Debug why upTime becomes negative
        upTime += execTime;

        // There may be multiple tasks arriving at the same time TODO: Check if need to loop
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
        }

        // TODO: From here onwards, consider moving to a private function called ContextSwitch
        // If no tasks are running, run the first task on ready queue
        if (runningTask == NULL) {
            Task temp_task = queue[0];
            runningTask = &temp_task;
            queue.erase(queue.begin());
            systemCeiling = runningTask->resource->ceiling;
        }
        // check queue if any task fit the criteria to preempt the current task
        // 1. It is the highest priority task in the queue (compare using period)
        // 2. It has a preemption level higher than system ceiling (compare using period)
        else {  
            if (queue[0].period < runningTask->period) {
                if (queue[0].period < systemCeiling) {     // Check for blocking 
                    queue.push_back(*runningTask);
                    runningTask = &queue[0];
                    SortQueue();
                }
                else {
                    // Calculate high speed
                    queue[0].blocked = true;
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

void Scheduler::SortTaskSet_byPeriod(void) {
    std::sort(
                initialTaskSet.begin(), 
                initialTaskSet.end(),
                [](const Task &lhs, const Task &rhs) {
                    return lhs.period < rhs.period;
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

void Scheduler::InitResources(void) {
    for (std::size_t i = 0; i < resourceList.size(); i++) {
        for (std::size_t j = 0; j < taskSet.size(); j++) {
            if ((taskSet[j].resource->index == resourceList[i].index) && (taskSet[j].period < resourceList[i].ceiling)) {
                resourceList[i].ceiling = taskSet[j].period;
            }
        }
    }
}

float Scheduler::calculate_low_speed(void) {
    float low_speed;
    float target_speed = 0;

    for (std::size_t i = 0; i < taskSet.size(); i++) {
        target_speed += (taskSet[i].wcc / taskSet[i].period);
    }

    //target_speed /= taskSet.size();
    std::cout << "target_speed: " << target_speed << std::endl;

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
    int index = T.index;
    float B = runningTask->rc / currentSpeed;          // Remaining computation of critical section
    float sum_of_product = 0;

    for (std::size_t i = 0; i < initialTaskSet.size(); i++) {
        float product = std::floor(period / initialTaskSet[i].period) * initialTaskSet[i].wcc;
        sum_of_product += product;
        if (index == initialTaskSet[i].index) {
            break;
        }
    }

    float high_speed;
    float target_speed = (B + sum_of_product) / period;
    std::cout << "target_speed: " << target_speed << std::endl;

    for (std::size_t i = 0; i < cpuSpeedSet.size(); i++) {
        std::cout << "high_speed now is: " << cpuSpeedSet[i] << std::endl;
        high_speed = cpuSpeedSet[i];
        if (target_speed <= high_speed)
            break;
    }

    return high_speed;
}

float Scheduler::calculate_exec_time(void) {
    nextArriveTime = (float) taskSet[0].arrivalTime;    // Next task arrive at this time
    float execTime_arrive = nextArriveTime - upTime;           // Time taken to wait for next task arrival
    float execTime_finish;
    float execTime_queue;

    // TODO: wcc might be wcct
    if (runningTask != NULL) {
        execTime_finish = runningTask->rc / currentSpeed; 
    }
    else {
        execTime_finish = std::numeric_limits<float>::max();    // Time taken to finish the current task
    }
        
    int queueTask_index = check_earliest_queue_task();
    if (queueTask_index != -1) {
        float QueueTask_deadline = queue[queueTask_index].arrivalTime + queue[queueTask_index].period;
        execTime_queue = QueueTask_deadline - upTime;  // Time taken until the deadline of a task in queue is up
    }
    else {
        execTime_queue = std::numeric_limits<float>::max();
    }

    // Run until next task arrival or until current task finish, 
    // or until a deadline of a task in queue is up, whichever comes first

    float execTime = std::min({execTime_arrive, execTime_finish, execTime_queue});

    // Add to total power consumption (Joules)
    float wattage = get_wattage(currentSpeed);
    std::cout << "wattage: " << wattage << std::endl;
    totalPC += execTime * wattage; 

    if (execTime == execTime_finish) {
        runningTask->burstTime += execTime;
        runningTask->rc = 0;
        handle_finished_task();
    }

    if (execTime == execTime_queue) {
        runningTask->burstTime += execTime;
        runningTask->rc -= (currentSpeed * execTime);
        handle_late_task(queueTask_index);  //queueTask_index won't be -1 in this case
    }

    return execTime;
}

// returns index to task in queue
int Scheduler::check_earliest_queue_task(void) {
    if (queue.size() <= 0) {
        // Return infinity if no task in queue
        return -1;
    }

    int earliest_deadline = queue[0].arrivalTime + queue[0].period;
    int earliest_task_index = 0;

    for (std::size_t i = 1; i < queue.size(); i++) {
        int new_deadline = queue[i].arrivalTime + queue[i].period;
        if (new_deadline < earliest_deadline) {
            earliest_deadline = new_deadline;
            earliest_task_index = i;
        }
    }
    
    return earliest_task_index;
}

// TODO: Running task might be late also
void Scheduler::handle_finished_task(void) {
    std::cout << "Task " << runningTask->index << " finished\n";
#if 0
    runningTask->arrivalTime += runningTask->period;
    runningTask->rc = runningTask->wcc;
    runningTask->burstTime = 0;
#endif

    // If the task was previously blocked, change back to low speed upon completion 
    if (runningTask->blocked) {
        runningTask->blocked = false;
        currentSpeed = LowSpeed;
        std::cout << "currentSpeed switched back to " << LowSpeed << std::endl;
    }

    totalTaskFinished += 1;
    runningTask = NULL;
    systemCeiling = std::numeric_limits<int>::max();
}

void Scheduler::handle_late_task(int index) {
    Task lateTask = queue[index];
    std::cout << "Task " << lateTask.index << " is late\n";
    // TODO: Check if need to erase from queue
    queue.erase(std::next(queue.begin(), index));
    totalLateCount += 1;
}

float Scheduler::get_wattage(float speed) {
    if (speed == cpuSpeedSet[5])
        return 0.925;
    if (speed == cpuSpeedSet[4])
        return 0.747;
    if (speed == cpuSpeedSet[3])
        return 0.570;
    if (speed == cpuSpeedSet[2])
        return 0.390;
    if (speed == cpuSpeedSet[1])
        return 0.279;
    return 0.116;
}
