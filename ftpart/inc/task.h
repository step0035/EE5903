#include <vector>

class Task {
    public:
        int index;
        int arrivalTime;                // Range from 0 to 100
        int period;                     // Relative deadline is assume to be the period, the smaller the period, the higher the preemption level
        float startExecTime;
        float wcc;                        // Worst case computation amount
        float rc = wcc;                       // Remaining computation amount
        float burstTime = 0;
        float frequency;
        float staticFrequency;
        std::vector<float> scheduling_points;
};
