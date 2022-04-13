class Resource {
    public: 
        int index;
        int ceiling = std::numeric_limits<int>::max();            // Highest preemption level (or lowest period) of all the tasks that require this resouce
        bool locked = false;
};
