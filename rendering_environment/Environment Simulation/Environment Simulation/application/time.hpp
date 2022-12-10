#ifndef NP_APP_TIME_H
#define NP_APP_TIME_H

//Includes:
#include<chrono>

namespace np_app
{
	//Type definitions:
	typedef std::chrono::high_resolution_clock clock;
	typedef std::chrono::duration<double, std::ratio<1>> second;
	const unsigned int MILLISECONDS_PER_SECOND = 1000;

	class Timer
	{
	private:
		//Attributes:
		std::chrono::time_point<std::chrono::high_resolution_clock> time_point;

	public:
		//Constructor:
		Timer() : time_point(std::chrono::high_resolution_clock::now()) { }

		//Methods:
		void restart() { time_point = std::chrono::high_resolution_clock::now(); };
		float elapsed_milliseconds() { return (std::chrono::high_resolution_clock::now() - time_point).count(); }
		float elapsed_seconds() { return ((std::chrono::high_resolution_clock::now() - time_point).count() / MILLISECONDS_PER_SECOND); }
	};
}

#endif //!NP_APP_TIME_H