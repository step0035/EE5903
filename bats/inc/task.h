class Task {
    public:
        int arrivalTime;                // Range from 0 to 100
        int period;                     // Relative deadline is assume to be the period, the smaller the period, the higher the preemption level
        int preemptionLevel;            // Derived from period, preemptionLevel = ceil(1 / period), since we don't want it to be 0
        float wcc;                        // Worst case computation amount
        float rc = wcc;                       // Remaining computation amount
        int burstTime = 0;
        int waitTime = 0;                   // Total time when task has arrived but not currently executing
        int criticalStart = 0;          // Assume 0
        int criticalEnd;                // TODO: Assume whole burst time?
        float B;                        // Remaining critical section computation time
        bool blocked = false;
        int Resource;
};
