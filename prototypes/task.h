struct ITask
{
public:
    //Abstract Methods:
    virtual void Invoke() = 0;

    //Operators:
    void operator() ()
    {
        this->Invoke();
    }
};

struct DebugMessage : ITask
{
private:
    //Attributes:
    std::string message;

public:
    //Constructor:
    DebugMessage(std::string message) : message(message) { }

    //Methods:
    void Invoke()
    {
        std::cout << message << std::endl;
    }
};