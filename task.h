class Task {
    public:
        uint8_t priority;                   // Derived from period
        uint8_t arrivalTime;
        uint8_t period;                     // Deadline is assume to be the period
        uint8_t wccTime;                    // Worst case computation time (scaled wrt cpuSpeed)
        uint8_t burstTime;
        uint8_t waitTime;                   // Total time when task has arrived but not currently executing
        uint8_t preemptionLevel;
        bool blocked;
        uint8_t sharedResources[];
}
