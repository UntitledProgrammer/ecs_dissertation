#include "Profiler.h"
#include <iostream>
#include <fstream>

np_ecs::Profiler* np_ecs::Profiler::singleton = nullptr;

void np_ecs::Profiler::add_record(const char* key, double time)
{
	//if (record.find(key) == record.end()) { record.emplace(Timestamp( time, key)); return; }
	record[key].key = key;
	record[key].calls++;
	record[key].time += time;
}

void np_ecs::Profiler::start(std::string name)
{
	session_name = name;
}

void np_ecs::Profiler::end()
{
	std::ofstream file;
	file.open(session_name + ".txt");
	file.flush();

	file << "ID,Average,Calls,Total\n";

	for (std::pair<std::string, Timestamp> pair : record)
	{
		file << pair.second.key;
		file << ",";
		file << pair.second.time / pair.second.calls;
		file << ",";
		file << pair.second.calls;
		file << ",";
		file << pair.second.time;
		file << "\n";
	}

	file.close();
}

void np_ecs::Profiler::end_console()
{
	for (std::pair<std::string, Timestamp> pair : record)
	{
		std::cout << "ID: " << pair.second.key << ", " << (pair.second.time / pair.second.calls) << "ms" << std::endl;
	}
}