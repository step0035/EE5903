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
        std::array<float, 6> cpuSpeedSet;         // Assume normalized values from BATS paper
        float currentSpeed;
        float LowSpeed;                           // Initial LOW speed
        std::vector<Resource> resourceList;
        int systemCeiling = std::numeric_limits<int>::max(); // systemCeiling should be 0 when no resources are used, but since we are comparing the period, we init this with infinity
        float duration;
        float upTime = 0;
        float totalPC = 0;        // Total power consumption
        float nextArriveTime;
        bool currentTaskFinished = false;
        int totalTaskFinished = 0;
        int totalLateCount = 0;

        /*
         * Constructor for Scheduler
         * - Generate random taskSet
         * - Setup CPU Speed, taskSet and queue
         */
        Scheduler(float Duration, int no_of_tasks, std::array<float, 6> SpeedSet, int no_of_resources);

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
         * Calculate the initial LOW speed
         */
        float calculate_low_speed(void);

        /*
         * Calculate the HIGH speed
         */
        float calculate_high_speed(Task T);
        
        float calculate_exec_time(void);

        int check_earliest_queue_task(void);

        void handle_finished_task(void); 

        void handle_late_task(int index);

        void InitResources(void);

        float get_wattage(float);
};
