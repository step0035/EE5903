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
         * - Generate random taskSet
         * - Setup CPU Speed, taskSet and queue
         */
        Scheduler(float Duration, int no_of_tasks);

        /*
         * Initialize the task set
         * - calculates initial LOW speed
         */
        void Init(void);

        /*
         * Starts the scheduler and start counting up
         * - calls background_check every unit time
         */
        void Start(void);

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

    private:

        /*
         * StaticF algorithm
         */
        void StaticF(void);

        /*
         * AdjustF algorithm
         */
        void AdjustF(Task *nextTask, float rcet);
        
        float calculate_exec_time(void);

        int check_earliest_queue_task(void);

        void handle_finished_task(float rcet); 

        void handle_late_task(int index);

        float get_wattage(float);

        void calculate_scheduling_points(void);

        float calculate_eflb(Task T, int q);

        float GetSupportedSpeed(float speed);
};
