#include<iostream>
#include<string>
#include<thread>
#include<mutex>
#include <queue>

std::mutex m;
std::queue<int> q;
std::condition_variable cond;

using namespace std::literals::chrono_literals;
void producer()
{
	int i = 10;
	while (i > 0)
	{
		std::unique_lock<std::mutex> locker(m);
		q.push(i);
		locker.unlock();
		cond.notify_one();
		//std::this_thread::sleep_for(1s);
		i--;

	}
}

void consumer()
{
	int data = 0;
	while (data != 1)
	{
		std::unique_lock<std::mutex> locker(m);
		cond.wait(locker, []() {return !q.empty(); });
			data = q.front();
			q.pop();
			locker.unlock();
			std::cout << "Data: " << data << std::endl;

	}
}

int main()
{
	std::thread t1(producer);
	std::thread t2(consumer);
	t1.join();
	t2.join();
	std::cin.get();
}
