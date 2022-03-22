#include <iostream>
#include "task.h"

class Scheduler {
    public:
        vector<Task> taskSet;
        vector<Task> queue;
        Task runningTask;
        float cpuSpeed;         // Assume normalized values from BATS paper

        /*
         * Initialize stuff
         * - calculates LCM of task set
         * - calculates initial LOW speed
         */
        void init(void);

        /*
         * Starts the scheduler and start counting up
         * - calls background_check every unit time
         */
        void start_scheduler(void);

    private:
        uint32_t upTime;
        uint32_t totalPC        // Total power consumption

        /*
         * Calculate the LCM of tasks
         */
        uint32_t calculate_LCM(vector<Task> taskSet);

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
        float calculate_initial_speed(vector<Task> taskSet);

        /*
         * Calculate the HIGH speed
         */
        float calculate_high_speed(vector<Task> taskSet);

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
}
