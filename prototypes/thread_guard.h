//Thread deconstruction example.
class ThreadGuard
{
private:
    //Attributes:
    std::thread& thread_obj;

public:
    //Constructors:
    explicit ThreadGuard(std::thread& thread) : thread_obj(thread) { }

    //Deconstructor:
    ~ThreadGuard()
    {
        if (thread_obj.joinable()) thread_obj.join();
    }
};
//!Thread deconstruction example.


/*
template<class Function, class... Args>
class ThreadGuard
{
private:
    //Attributes:
    std::thread* thread_obj;

public:
    //Constructors:
    Thread(Function&& function, Args&&... arguments) { thread_obj = new std::thread(function, arguments...); }
    Thread(const std::thread* thread) { this->thread_obj = thread; }

    //Deconstructor:
    ~ThreadGuard()
    {
        if (thread_obj != nullptr && thread_obj->joinable()) thread_obj->join();
    }
};



*/