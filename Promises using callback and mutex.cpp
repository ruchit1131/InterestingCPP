#include <iostream>
#include <thread>
#include <mutex>
#include "string"
#include<atomic>
using namespace std::literals::chrono_literals;

std::mutex mu;
//! We'll need to use atomic bool in this code or it will cause race condition and somehow crash the code.
//std::atomic<bool> locked = true;
bool locked = true;
std::mutex m2;

void unlock_mutex()
{
	std::cout << "In unlock_mutex" << std::endl;
	//m2.lock();
	locked = false;
	//m2.unlock();
	std::cout << "In locked: " << locked <<std::endl;
}

void decode_video(void fun())
{
	for (int i = 0; i < 100; i++)
	{
		if (i == 50)
		{
			std::cout << i << std::endl;
			fun();
		}
	}
}

void lock_mutex()
{
	mu.lock();
	std::cout << locked << std::endl;
	while (1)
	{
		if (locked)
		{
			/*m2.lock();
			if(locked)
				locked = true;
			m2.unlock();*/
		}
		else
			break;
	}
	std::cout << "Mutex Released" << std::endl;
	mu.unlock();
}

void process()
{
	std::cout << "In process" << std::endl;
	mu.lock();
	std::cout << "Now the value is 50" << std::endl;
	std::cout << "And I can start processing" << std::endl;
	mu.unlock();
}

int main()
{
	std::thread t(lock_mutex);
	std::thread t3(process);
	std::thread t1(decode_video, unlock_mutex);

	t3.join();
	t1.join();
	t.join();
}
