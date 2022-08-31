#include<iostream>
#include<string>
#include<thread>
#include<mutex>
#include <queue>


std::mutex m, m2;
std::condition_variable cond, cond2;
bool flag = false;
using namespace std::literals::chrono_literals;

void even()
{
	for (int i = 0; i < 1000; i+=2)
	{
		std::unique_lock<std::mutex> locker(m);
		cond.wait(locker, []() {return flag == false; });
		std::cout << i << std::endl;
		flag = true;
		locker.unlock();
		cond.notify_all();
	}
}

void odd()
{
	for (int i = 1; i < 1000; i+=2)
	{
		std::unique_lock<std::mutex> locker(m);
		cond.wait(locker, []() {return flag == true; });
		std::cout << i << std::endl;
		flag = false;
		locker.unlock();
		cond.notify_all();
	}
}

int main()
{
	std::thread t2(even);
	std::thread t1(odd);

	t1.join();
	t2.join();
	std::cin.get();
}
