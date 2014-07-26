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

#pragma once
#include "platform_definitions.h"

// This is a bit of a black box that isn't supposed to be opened unless you _really_ know what you're doing.
// The interface will be the same on all platforms, but the way that the internals work can be different.
struct node
{
	node *n_;

	// 64 bit pointer uses 48 bit addressing
	// the unused 16 bits are going to hold an identification number
#if PROCESSOR_BITS == 64
	using stack_id_t = uint16_t;

	inline node() : n_{ nullptr }                    { }
	inline node(node* n) : n_{ n }                   { }
	inline void create_id(const node& nid)           { ((stack_id_t*)this)[3] = ((const stack_id_t*)&nid)[3] + 1; }
	inline node* next_pointer()                      { return (node*)((uint64_t)n_ & 0x0000ffffffffffff); }

	// 32 bit pointers are "full", so I use another 32bit piece of data for the counter
	// on 32bit x86, we can swap the entire 64 bits without a lock
#elif PROCESSOR_BITS == 32
	using stack_id_t = uint32_t;
	stack_id_t t_;

	inline node() : n_{ nullptr }, t_{ 0 }           { }
	inline node(node* n) : n_{ n }, t_{ 0 }          { }
	inline void create_id(const node& nid)           { t_ = nid.t_ + 1; }
	inline node* next_pointer()                      { return n_; }
#else
	static_assert(false, "unknown pointer size");
#endif
	inline node(node* n, const node& nid) : n_{ n }  { if (n_) create_id(nid); else create_id(*this); }
};
