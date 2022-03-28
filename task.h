class Task {
    public:
        uint8_t preemptionLevel;                   // Derived from period, same as priority
        uint8_t arrivalTime;
        uint8_t period;                     // Deadline is assume to be the period
        uint8_t wccTime;                    // Worst case computation time (scaled wrt cpuSpeed)
        uint8_t burstTime;
        uint8_t waitTime;                   // Total time when task has arrived but not currently executing
        bool blocked;
        uint8_t sharedResources[];
}
