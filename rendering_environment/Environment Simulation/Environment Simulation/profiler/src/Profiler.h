#ifndef NP_PROFILER_H
#define NP_PROFILER_H

//Includes:
#include <vector>
#include <string>
#include<map>

namespace np_ecs
{
	//Structures:
	struct Timestamp
	{
	public:
		double time;
		std::string key;
		unsigned int calls;
		
		Timestamp() : time(0.0f), key("empty"), calls(0) { }
		Timestamp(double time, std::string key, unsigned int calls = 1) : time(time), key(key), calls(calls) { }
	};

	class Profiler
	{
	private:
		//Attributes:
		static Profiler* singleton;
		std::map<std::string, Timestamp> record;
		std::string session_name;

		//Methods:
		void add_record(const char* key, double time);

	public:
		//Methods:
		static Profiler* global()
		{
			if (singleton == nullptr) singleton = new Profiler();
			return singleton;
		}

		void start(std::string name);
		void end();
		void end_console();

		//Friends:
		friend class LifetimeTimer;
	};
}

#endif // !NP_PROFILER_H

#define START_SESSION(key) np_ecs::Profiler::global()->start(key)

#ifdef NP_PROFILE_TO_CONSOLE

#define END_SESSION() np_ecs::Profiler::global()->end_console()

#else

#define END_SESSION() np_ecs::Profiler::global()->end()

#endif