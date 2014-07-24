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

#pragma once
#include "platform_definitions.h"

// This is a bit of a black box that isn't supposed to be opened unless you _really_ know what you're doing.
// The interface will be the same on all platforms, but the way that the internals work can be different.
struct node
{
	node *n_;

	// 64 bit pointer uses 48 bit addressing, and so we're tagging the last 16 bits
#if PROCESSOR_BITS == 64
	using stack_tag_t = uint16_t;

	inline node() : n_{ nullptr }				{ }
	inline node(node* n) : n_{ n }					{ }
	inline void tag(const stack_tag_t t)				{ reinterpret_cast<stack_tag_t*>(this)[3] = t; }
	inline stack_tag_t read_tag()						{ return reinterpret_cast<stack_tag_t*>(this)[3]; }
	inline void clear_pointer()							{ tag(0); }

	// 32 bit pointers are "full", so I use another 32bit piece of data for the counter
	// on 32bit x86, we can swap the entire 64 bits without a lock
#elif PROCESSOR_BITS == 32
	using stack_tag_t = uint32_t;
	stack_tag_t t_;

	inline node() : n_{ nullptr }, t_{ 0 }	{ }
	inline node(node* n) : n_{ n }, t_{ 0 }			{ }
	inline void tag(const stack_tag_t t)				{ t_ = t; }
	inline stack_tag_t read_tag()						{ return t_; }
	inline void clear_pointer()							{ }
#else
	static_assert(false, "unknown pointer size");
#endif
	inline void set(node* n, const stack_tag_t t)		{ n_ = n; tag(t); }
};