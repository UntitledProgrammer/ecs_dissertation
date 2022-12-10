#include<variant>

template<class Success, class Fail> struct Result
{
public:
	//Attributes:
	std::variant<Success, Fail> output;
};