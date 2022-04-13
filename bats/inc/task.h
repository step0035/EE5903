#include "resource.h"

class Task {
    public:
        int index;
        int arrivalTime;                // Time when task arrive
        int period;                     // Relative deadline is assume to be the period, the smaller the period, the higher the preemption level
        float wcc;                      // Worst case computation amount
        float rc = wcc;                 // Remaining computation amount
        float burstTime = 0;            // Time CPU spent executing the task
        float B;                        // Remaining critical section computation time
        bool blocked = false;           // Indicator for task's blocking status
        Resource *resource;             // Resource required by the task
};
