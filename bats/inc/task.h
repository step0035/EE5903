class Task {
    public:
        int arrivalTime;                // Range from 0 to 100
        int period;                     // Deadline is assume to be the period
        int preemptionLevel;            // Derived from period, preemptionLevel = ceil(1 / period), since we don't want it to be 0
        float wcc;                        // Worst case computation amount
        int burstTime = 0;
        int waitTime = 0;                   // Total time when task has arrived but not currently executing
        int criticalStart = 0;          // Assume 0
        int criticalEnd;                // TODO: Assume whole burst time?
        float B;                        // Remaining critical section computation time
        bool blocked = false;
        int Resource;

        /*
         * Operator to sort Task by arrivalTime
         */
        bool operator< (const Task &other) const {
            return arrivalTime < other.arrivalTime;
        }
};
