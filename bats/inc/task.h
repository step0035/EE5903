#include <cstdint>
#include <set>

class Task {
    public:
        uint8_t arrivalTime;                // Range from 0 to 100
        uint8_t preemptionLevel;            // Derived from period, same as priority
        uint8_t period;                     // Deadline is assume to be the period
        uint8_t wcc;                        // Worst case computation amount
        uint8_t burstTime;
        uint8_t waitTime;                   // Total time when task has arrived but not currently executing
        bool blocked;
        std::set<uint8_t> sharedResources;
};
