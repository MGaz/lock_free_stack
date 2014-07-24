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

#include "stack.h"
// This is it! This is the magic right here.
// A simple lock-free stack.
// It doesn't look like much, but getting this one right is difficult.
// I solved the ABA problem by tagging the pointers incrementally
// This effectively ensures that when we swap pointers, we swap the right one

void stack::push(node* n)
{
	// Process for pushing an item without a lock
	//		- get the head element
	//		- setup the "next" element - it points to the item being pushed
	//		- set the new node's next pointer to point to the head's next pointer
	//		- set the new node's next tag to the current head's tag + 1
	//		- TRY to swap
	//		- compare_exchange will return true if AND ONLY if:
	//				the new item has been sucessfully placed in the head of the stack
	//				the new item points to the item that was previously at the head of the stack
	node next{ n }, head{ head_.load(std::memory_order_relaxed) };
	do
	{
		n->n_ = head.n_;
		next.tag(head.read_tag() + 1);
	} while (!head_.compare_exchange_weak(head, next, std::memory_order_relaxed, std::memory_order_relaxed));
}
bool stack::pop(node*& n)
{
	// Process for popping an item without a lock
	//		- get the head element
	//		- retrieve the head's pointer to the next item - it's stored in clean
	//		- remove the tag from the retrieved pointer - we need to do this to ensure the pointer is valid
	//					the pointer would be invalid on x64 without removing the tag
	//		- set next's pointer to it's next item to the clean item's pointer to next
	//		- set the new node's next tag to the current head's tag + 1
	//		- TRY to swap
	//		- compare_exchange will return true if AND ONLY if:
	//				the head item has been sucessfully removed from the stack
	//				the "clean" item holds a pointer to the node that was just in the head_ item
	//				the head points to the item that was formerly the pointer to n_ in the head
	n = nullptr;
	node clean, next, head{ head_.load(std::memory_order_relaxed) };
	do
	{
		clean.set(head.n_, 0);

		if (!clean.n_)
			return false;

		next.set(clean.n_->n_, head.read_tag() + 1);
	} while (!head_.compare_exchange_weak(head, next, std::memory_order_relaxed, std::memory_order_relaxed));

	n = clean.n_;
	return true;
}