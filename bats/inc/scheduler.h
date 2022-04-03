#include <cstdint>
#include <vector>
#include <array>
#include "task.h"

class Scheduler {
    public:
        std::vector<Task> taskSet;
        std::vector<Task> queue;
        Task runningTask;
        std::array<float, 7> cpuSpeedSet;         // Assume normalized values from BATS paper
        float currentSpeed;
        std::array<int, 5> resourcesList;
        long long int LCM;

        /*
         * Constructor for Scheduler
         * - Generate random taskSet
         * - Setup CPU Speed, taskSet and queue
         */
        Scheduler(int no_of_tasks, std::array<float, 7> SpeedSet, std::array<int, 5> ResourceSet);

        /*
         * Initialize the task set
         * - calculates LCM of task set
         * - calculates initial LOW speed
         */
        void Init(void);

        /*
         * Starts the scheduler and start counting up
         * - calls background_check every unit time
         */
        void Start(void);

    private:
        int upTime = 0;
        int totalPC = 0;        // Total power consumption

        /*
         * Calculate the LCM of tasks
         */
        long long int calculate_LCM(void);

        /*
         * Background checks the arrival of tasks and events
         * - checks if tasks have arrived, if yes, push to queue
         * - checks if preemption should take place
         * - checks for potential blocking
         */
        void background_check(void);

        /*
         * Calculate the initial LOW speed
         */
        float calculate_initial_speed(std::vector<Task> taskSet);

        /*
         * Calculate the HIGH speed
         */
        float calculate_high_speed(std::vector<Task> taskSet);

        /*
         * Update system state
         * - update the CPU speed if needed
         * - update the upTime and totalPC
         */
        void update_system_state(void); 

        /*
         * Update the state of the tasks in the task set
         */
        void update_task_state(void); 

        /*
         * Update the state of the resources
         */
        void update_resources_state(void); 

        /*
         * Update the state of the queue
         * - check for task arrivals and add them to queue
         * - sort by priority
         */
        void update_queue_state(void); 

        /*
         * Greatest Common Divisor
         * - helper function for calculate_LCM()
         */
        int gcd(int a, int b) {
            if (b == 0)
                return a;
            return gcd(b, a % b);
        }
};
