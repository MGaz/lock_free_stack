///////////////////////////////////////////////////////////////////////////////
///                         SIMPLE LOCK FREE STACK                          ///
///           Copyright (c) 2014 Michael Gazonda - http://mgaz.ca           ///
///                              CPOL Licensed                              ///
///       See CPOL.htm, or http://www.codeproject.com/info/cpol10.aspx      ///
///               http://www.codeproject.com/Articles/801537/               ///
///                 https://github.com/MGaz/lock_free_stack                 ///
///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include "stack.h"
#include "data.h"

// These are the testing parameters, you can change them
#define data_count 10
#define loop_count 1000000
#define thread_count 4

// This is the test function
// It uses those numbers set at the top

// Using new and delete is part of the test - for our new/delete calls to not crash,
// the data inside the stack must be handled properly. When we don't crash, and there is no
// "lost data", we know that everything went properly when playing with the stack.

void thread_test(stack *s, std::atomic<uint64_t> *max_elapsed, std::atomic<size_t> *empty_count, size_t index)
{
	// Initialization - create the data we'll test with
	data* d[data_count];
	for (size_t i = 0; i < data_count; ++i)
		d[i] = new data;

	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

	// This is the test loop
	// Push and pop x number of times and see if everything comes out ok on the other side.
	// Also a good working sample of moving data into and out of the stack.
	for (size_t loop = 0; loop < loop_count; ++loop)
	{
		for (size_t i = 0; i < data_count; ++i)
		{
			if (d[i])
				s->push(d[i]);
		}

		for (size_t i = 0; i < data_count; ++i)
			s->pop((node*&)d[i]);

	}

	std::chrono::high_resolution_clock::time_point finish = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds span = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);

	std::cout << index << " - thread completed : " << span.count() << "\r\n";
	*max_elapsed = span.count();

	// If the test is successful, every location will hold a valid pointer, and no pointers
	// will be duplicated. We test for a valid pointer, and by using delete we ensure that we
	// didn't have the same data in two places - deleting the same data twice would crash.
	//		- may not crash on all platforms, so this test isn't meant to have total cross-platform support
	for (size_t i = 0; i < data_count; ++i)
	{
		if (d[i])
			delete d[i];
		else
			(*empty_count)++;
	}
}

// This is the place where we get stuff done. Start up the test threads, wait for them, and then
// check the results to display to the user afterwards. No big deal :)
int main(int argc, const char * argv[])
{
	std::thread threads[thread_count];
	std::atomic<uint64_t> max_elapsed{ 0 };
	std::atomic<size_t> empty_count{ 0 };
	stack s;

	std::cout << R"_(///////////////////////////////////////////////////////////////////////////////
///                         SIMPLE LOCK FREE STACK                          ///
///           Copyright (c) 2014 Michael Gazonda - http://mgaz.ca           ///
///                              CPOL Licensed                              ///
///       See CPOL.htm, or http://www.codeproject.com/info/cpol10.aspx      ///
///               http://www.codeproject.com/Articles/801537/               ///
///                 https://github.com/MGaz/lock_free_stack                 ///
///////////////////////////////////////////////////////////////////////////////)_";
	std::cout << "\r\nstarting\r\n";

	// Start threads
	for (size_t i = 0; i < thread_count; ++i)
		threads[i] = std::thread(thread_test, &s, &max_elapsed, &empty_count, i);

	// Wait for them to all finish
	for (size_t i = 0; i < thread_count; ++i)
		threads[i].join();

	// Output information
	size_t operation_count = data_count * loop_count * thread_count * 2;
	if (!empty_count)
		std::cout << "no lost data\r\n";
	else
		std::cout << "___lost data count___: " << empty_count << " of " << data_count * thread_count << "\r\n";
	std::cout << "thread count         : " << thread_count << "\r\n";
	std::cout << "target processor bits: " << PROCESSOR_BITS << "\r\n";
	std::cout << "total pushes and pops: " << operation_count << "\r\n";
	std::cout << "operations per second: " << operation_count / (max_elapsed.load() > 0 ? max_elapsed.load() : 1) * 1000 << "\r\n";
	std::cout << "processing time      : " << max_elapsed.load() << "ms\r\n";
	std::cout << "press any key to exit\r\n";
	std::getchar();
    return 0;
}
