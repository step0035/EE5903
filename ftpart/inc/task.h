#include <vector>

class Task {
    public:
        int index;
        int arrivalTime;                            // Time when task arrive
        int period;                                 // Relative deadline is assume to be the period, the smaller the period, the higher the priority
        float startExecTime;                        // Time when CPU starts executing the task
        float wcc;                                  // Worst case computation amount
        float rc = wcc;                             // Remaining computation amount
        float burstTime = 0;                        // Time CPU spent on executing the task
        float frequency;                            // Operating frequency of the CPU for this task
        float staticFrequency;                      // Static operating frequency which is calculated at the start
};
