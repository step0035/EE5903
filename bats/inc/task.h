#include <set>

class Task {
    public:
        int arrivalTime;                // Range from 0 to 100
        int preemptionLevel;            // Derived from period, same as priority
        int period;                     // Deadline is assume to be the period
        int wcc;                        // Worst case computation amount
        int burstTime;
        int waitTime;                   // Total time when task has arrived but not currently executing
        bool blocked;
        std::set<int> sharedResources;
};
