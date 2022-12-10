
#ifndef SIMULATABLE_H
#define SIMULATABLE_H

//Includes:
#include<assert.h>

//Defines:
#define assert_success(Result) assert(Result == ISimulatable::Result::Success)

//For testing only... never do this!
static unsigned int counter = 0;

template<typename Type>
static unsigned int GetID()
{
	static unsigned int count = 1 << (counter++);
	return count;
}
//For testing only... never do this!


class ISimulatable
{
public:
	//Enum:
	enum class Result { Error = -1, Failure = 0, Success = 1 };

	//Methods:
	virtual Result OnInitialise() = 0;
	virtual Result OnUpdate() = 0;
	virtual Result OnTerminate() = 0;
};

#endif // !SIMULATABLE_H