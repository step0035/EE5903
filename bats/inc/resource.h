class Resource {
    public: 
        int ceiling;            // Highest preemption level (or lowest period) of all the tasks that require this resouce
        bool locked = false;

        bool isLocked(void) {
            return locked;
        }
};
