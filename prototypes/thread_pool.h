#pragma once

//Includes:
#include<thread>
#include<vector>
#include<functional>
#include"collections/queue.h"
#include"thread_guard.h"

class ThreadPool
{
private:
	//Attributes:
	std::vector<std::thread> threads;
	thread_safe::Queue<std::function<void()>> tasks;
	std::atomic<bool> active;

public:
	//Constructor:
	ThreadPool() : active(false)
	{
		for (int i = 0; i < Size(); i++)
		{
			std::thread t(&ThreadPool::Poll, this);
			threads.push_back(std::move(t));
		}
	}

	//Methods:
	const unsigned int Size() { return std::thread::hardware_concurrency(); }

	void PollTask(std::function<void()> task)
	{
		tasks.Enqueue(task);
	}

	//Destructor:
	~ThreadPool()
	{
		active = true;

		for (int i = 0; i < threads.size(); i++)
		{
			if(threads[i].joinable()) threads[i].join();
		}

		threads.clear();
	}

private:
	//Methods:
	void Poll()
	{
		while (!active)
		{

			std::function<void()> task;
			
			if (tasks.TryRetrieve(&task))
			{
				task();
			}

			std::this_thread::yield();
			std::cout << std::this_thread::get_id() << ", Is Polling tasks." << std::endl;
		}
	}
};