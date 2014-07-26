//   The MIT License (MIT)

///////////////////////////////////////////////////////////////////////////////
///                        SIMPLE LOCK FREE STACK                           ///
///                   Copyright (c) 2014 Michael Gazonda                    ///
///                            http://mgaz.ca                               ///
///                 https://github.com/MGaz/lock_free_stack                 ///
///////////////////////////////////////////////////////////////////////////////

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include "stack.h"
#include "data.h"

// These are some simple things you can change, and see how the performance changes
#define data_count 40
#define loop_count 250000
#define thread_count 4


// This is the test function
// It uses those numbers set at the top
// Using new and delete is part of the test - for our new/delete calls to not crash, 
// the data inside the stack must be handled properly. When we don't crash, and there is no
// "lost data", we know that everything went properly when modifying the stack.
void thread_test(stack *s, std::atomic<uint64_t> *elapsed, std::atomic<size_t> *empty_count)
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
	
	*elapsed = span.count();

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
	std::atomic<uint64_t> elapsed{ 0 };
	std::atomic<size_t> empty_count{ 0 };
	stack s;
	
	std::cout << R"_(///////////////////////////////////////////////////////////////////////////////
///                        SIMPLE LOCK FREE STACK                           ///
///                   Copyright (c) 2014 Michael Gazonda                    ///
///                            http://mgaz.ca                               ///
///                 https://github.com/MGaz/lock_free_stack                 ///
///////////////////////////////////////////////////////////////////////////////)_";
	std::cout << "\r\nstarting\r\n";
	
	// Start threads
	for (size_t i = 0; i < thread_count; ++i)
		threads[i] = std::thread(thread_test, &s, &elapsed, &empty_count);

	// Wait for them to all finish
	for (size_t i = 0; i < thread_count; ++i)
		threads[i].join();

	// Output information
	size_t operation_count = data_count * loop_count * thread_count * 2;
	std::cout << "time: " << elapsed << "ms\r\n";
	if (!empty_count)
		std::cout << "no lost data\r\n";
	else
		std::cout << "lost data count: " << empty_count << " of " << data_count * thread_count << "\r\n";
	std::cout << "thread count: " << thread_count << "\r\n";
	std::cout << "target processor bits: " << PROCESSOR_BITS << "\r\n";
	std::cout << "total pushes and pops: " << operation_count << "\r\n";
	std::cout << "operations per second: " << operation_count / elapsed * 1000 << "\r\n";
	std::cout << "press any key to exit\r\n";
	std::getchar();
    return 0;
}