#include <cstdint>
#include <vector>
#include <array>
#include <limits>
#include "task.h"

class Scheduler {
    public:
        std::vector<Task> taskSet;
        std::vector<Task> initialTaskSet;         // Backup of the initial task set, used for calculating the high speed
        std::vector<Task> queue;
        Task *runningTask = NULL;
        std::array<float, 6> cpuSpeedSet = {1.0, 0.83, 0.67, 0.50, 0.33, 0.17};         // Assume normalized values from BATS paper
        float duration;
        float upTime = 0;
        float totalPC = 0;        // Total power consumption
        float nextArriveTime;
        bool currentTaskFinished = false;
        int totalTaskFinished = 0;
        int totalLateCount = 0;
        float overallStaticFrequency;

        /*
         * Constructor for Scheduler
         * @Duration: the duration of the entire simulation in seconds
         * @no_of_tasks: the number of tasks in the task set
         * - Generate random taskSet
         * - Setup CPU Speed, taskSet and queue
         */
        Scheduler(float Duration, int no_of_tasks);

        /*
         * Initialize the task set
         * - assign indexes to tasks in the task set
         * - calculates initial LOW speed using StaticF
         */
        void Init(void);

        /*
         * Starts the scheduler and start counting up
         */
        void Start(void);

    private:

        /*
         * Sort the taskSet by arrivalTime in non-descending order
         */
        void SortTaskSet(void);

        /*
         * Sort the taskSet by period in non-descending order
         */
        void SortTaskSet_byPeriod(void);

        /*
         * Sort the queue by period in non-descending order
         * - Tasks with smaller period means they have higher preemption level
         * - This ensures that the task to run next is always at the front of the queue
         */
        void SortQueue(void);

        /*
         * StaticF algorithm
         * - Calculates the static frequency, of all the tasks
         */
        void StaticF(void);

        /*
         * AdjustF algorithm
         * @nextTask: a pointer to the next task to be run on the CPU
         * @rcet: the remaining computation amount of the current task
         * - Adjust the speed of the CPU for the next task, if there
         *   exists slack time upon completion of current task
         */
        void AdjustF(Task *nextTask, float rcet);
        
        /*
         * Execute until next scheduling point
         * @retval: amount of execution time in seconds
         * - Next task arrives
         * - Current task finish executing, call @handle_finish_task
         * - Task in queue becomes late, call @handle_late_task
         */
        float calculate_exec_time(void);

        /*
         * Checks which task is next to be late in queue
         * @retval: index of the task that is next to be late
         *          or -1 if no tasks in queue
         */
        int check_earliest_queue_task(void);

        /*
         * Called when a task is finished
         * @rcet: remaining computation amount of the task
         * - Set speed to LOW speed if a blocked task finished executing
         * - Reset the system ceiling once the task unlocks the resource
         */
        void handle_finished_task(float rcet); 

        /*
         * Called when a task missed its deadline
         * - Discard task from queue
         */
        void handle_late_task(int index);

        /*
         * Gets the wattage of the running speed
         * @speed: the current speed of the CPU
         * @retval: the wattage of the speed
         */
        float get_wattage(float speed);

        /*
         * Gets the actual speed that is supported by the CPU
         * @speed: the calculated target speed to achieve
         * @reval: the lowest speed in the task set that is
         *         higher than the calculated target speed
         */
        float GetSupportedSpeed(float speed);
};
