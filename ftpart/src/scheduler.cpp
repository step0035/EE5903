#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>
#include "config.h"
#include "scheduler.h"

Scheduler::Scheduler(float Duration, int no_of_tasks) {
    // Set duration
    duration = Duration;

    // Initialize taskSet
    std::random_device rd;
    //std::default_random_engine generator(rd());     // use this for randomized seeding
    std::default_random_engine generator(CONFIG_RANDOM_NUMBER);
    std::uniform_int_distribution<int> arrivaltime_dist(CONFIG_ARRIVALTIME_LOW, CONFIG_ARRIVALTIME_HIGH);
    std::uniform_int_distribution<int> period_dist(CONFIG_PERIOD_LOW, CONFIG_PERIOD_HIGH);
    std::uniform_real_distribution<float> wcc_dist(CONFIG_WCCT_LOW, CONFIG_WCCT_HIGH);

    // resource_dist and dummy are not needed for FTPART, it is to ensure that the same task set is used in BATS algorithm, less the resource
    std::uniform_int_distribution<int> resource_dist(0, 1);
    int dummy;

    std::cout << "no_of_tasks = " << no_of_tasks << std::endl << std::endl;
    for (int i=0; i < no_of_tasks; i++) {
        Task T;

        T.arrivalTime = arrivaltime_dist(generator);

        T.period = period_dist(generator);

        T.wcc = wcc_dist(generator);

        T.rc = T.wcc;

        // not used, just to ensure task set consistency
        dummy = static_cast<std::size_t> (resource_dist(generator));

    taskSet.push_back(T);
    std::cout << std::endl;
    }

    // Sort tasks by arrivalTime in non-descending order
    SortTaskSet();

    // Sort the cpuSpeedSet
    std::sort(cpuSpeedSet.begin(), cpuSpeedSet.end());
}

void Scheduler::Init(void) {
    // Assign index to the tasks, should already be sorted by period
    for (std::size_t i = 0; i < taskSet.size(); i++) {
        taskSet[i].index = i;
    }

    StaticF();
}

void Scheduler::Start(void) {
    Task arrived_task;
    Task next_task;
    while (upTime < duration) {
        std::cout << "upTime: " << upTime << std::endl;

        float execTime = calculate_exec_time();

        upTime += execTime;

        if (upTime >= nextArriveTime) {
            // There may be multiple tasks arriving at the same time
            while (taskSet[0].arrivalTime <= nextArriveTime) {
                // Append task that arrived into queue
                arrived_task = taskSet[0];
                queue.push_back(arrived_task);
                SortQueue();

                // Replace the task in taskSet with the next instance
                taskSet[0].arrivalTime += taskSet[0].period;
                SortTaskSet();
            }
        }

        // If no tasks are running, run the first task on ready queue
        if (runningTask == NULL) {
            if (queue.size() != 0) {
                next_task = queue[0];
                runningTask = &next_task;
                runningTask->startExecTime = upTime;
                queue.erase(queue.begin());
            }
        }
        // check queue if there is any higher priority task to preempt the current task
        else {  
            if (queue.size() != 0) {
                if (queue[0].period < runningTask->period) {
                    queue.push_back(*runningTask);
                    Task next_task = queue[0];
                    runningTask = &next_task;
                    runningTask->startExecTime = upTime;
                    queue.erase(queue.begin());
                    SortQueue();
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

void Scheduler::StaticF(void) {
    float static_frequency;
    float target_speed = 0;

    for (std::size_t i = 0; i < taskSet.size(); i++) {
        target_speed += (taskSet[i].wcc / taskSet[i].period);
    }

    for (std::size_t i = 0; i < cpuSpeedSet.size(); i++) {
        static_frequency = cpuSpeedSet[i];
        if (target_speed <= static_frequency)
            break;
    }

    for (std::size_t i = 0; i < taskSet.size(); i++) {
        taskSet[i].staticFrequency = static_frequency;
        taskSet[i].frequency = static_frequency;
    }

    overallStaticFrequency = static_frequency;
}

void Scheduler::AdjustF(Task *nextTask, float rcet) {
    float deadline = runningTask->arrivalTime + runningTask->period;
    float slack = deadline - runningTask->startExecTime - rcet;
    float nextTaskFrequency = nextTask->wcc / ((nextTask->wcc / nextTask->staticFrequency) + slack);
    nextTask->frequency = GetSupportedSpeed(nextTaskFrequency);
}

float Scheduler::GetSupportedSpeed(float speed) {
    float SupportedSpeed;

    for (std::size_t i = 0; i < cpuSpeedSet.size(); i++) {
        SupportedSpeed = cpuSpeedSet[i];
        if (speed <= SupportedSpeed)
            break;
    }

    return SupportedSpeed;
}

float Scheduler::calculate_exec_time(void) {
    nextArriveTime = (float) taskSet[0].arrivalTime;              // Next task arrive at this time
    float execTime_arrive = nextArriveTime - upTime;              // Time taken to wait for next task arrival
    float execTime_finish;
    float execTime_queue;

    if (runningTask != NULL) {
        execTime_finish = runningTask->rc / runningTask->frequency; 
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

    if (runningTask != NULL) {
        runningTask->burstTime += execTime;
        runningTask->rc -= (runningTask->frequency * execTime);
        
        // Add to total power consumption (Joules)
        float wattage = get_wattage(runningTask->frequency);
        totalPC += execTime * wattage; 
    }

    if (execTime == execTime_finish) {
        handle_finished_task(execTime);
    }

    if (execTime == execTime_queue) {
        handle_late_task(queueTask_index);  //queueTask_index won't be -1 in this case
    }

    return execTime;
}

int Scheduler::check_earliest_queue_task(void) {
    if (queue.size() <= 0) {
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

void Scheduler::handle_finished_task(float rcet) {
    std::cout << "Task " << runningTask->index << " finished\n";

    Task *nextTask = &queue[0];
    AdjustF(nextTask, rcet);
    totalTaskFinished += 1;
    runningTask = NULL;
}

void Scheduler::handle_late_task(int index) {
    Task lateTask = queue[index];
    std::cout << "Task " << lateTask.index << " is late\n";
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
