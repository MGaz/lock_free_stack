///////////////////////////////////////////////////////////////////////////////
///                        SIMPLE LOCK FREE STACK                           ///
///                   Copyright (c) 2014 Michael Gazonda                    ///
///                       MIT Open Source Licensed                          ///
///                            http://mgaz.ca                               ///
///                 https://github.com/MGaz/lock_free_stack                 ///
///////////////////////////////////////////////////////////////////////////////

//   The MIT License (MIT)

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

void gaz::stack::push(node* n)
{
	node old_head, new_head{ n };
	n->n_ = nullptr;
	while (!head_.compare_exchange_weak(old_head, new_head))
	{
		n->n_ = old_head.n_;
		new_head.create_id(old_head);
	}
}
bool gaz::stack::pop(node*& n)
{
	node old_head, new_head;
	n = nullptr;
	while (!head_.compare_exchange_weak(old_head, new_head))
	{
		n = old_head.next_pointer();
		if (!n)
			break;
		new_head.set(n->n_, old_head);
	}
	return n != nullptr;
}
