#ifndef NP_TIMER_H
#define NP_TIMER_H

//Includes:
#include <chrono>
#include "Profiler.h"

namespace np_ecs
{
	struct LifetimeTimer
	{
	private:
		//Attributes:
		std::chrono::milliseconds count;
		const char* key;

	public:
		//Constructor:
		LifetimeTimer(const char* key) : key(key)
		{
			count = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
		}

		//Destructor:
		~LifetimeTimer()
		{
			count = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) - count;
			Profiler::global()->add_record(key, count.count());
		}
	};
}

#endif // !NP_TIMER_H

#define LIFETIME(key) np_ecs::LifetimeTimer lifetime_timer_##__LINE__(key)
#define PROFILE_FUNCTION() LIFETIME(__FUNCSIG__)
#define PROFILE_SCOPE(key) LIFETIME(key)