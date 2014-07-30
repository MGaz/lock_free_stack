///////////////////////////////////////////////////////////////////////////////
///                         SIMPLE LOCK FREE STACK                          ///
///           Copyright (c) 2014 Michael Gazonda - http://mgaz.ca           ///
///                              CPOL Licensed                              ///
///       See CPOL.htm, or http://www.codeproject.com/info/cpol10.aspx      ///
///               http://www.codeproject.com/Articles/801537/               ///
///                 https://github.com/MGaz/lock_free_stack                 ///
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include "platform_definitions.h"

// This is a bit of a black box that isn't supposed to be opened unless you _really_ know what you're doing.
// The interface will be the same on all platforms, but the way that the internals work can be different.
namespace gaz
{
	struct node
	{
		node *n_;

		// 64 bit pointer uses 48 bit addressing
		// the unused 16 bits are going to hold an identification number
#if PROCESSOR_BITS == 64
		using stack_id = uint16_t;

		inline node() __noexcept : n_{ nullptr }           { }
		inline node(node* n) __noexcept : n_{ n }          { }
		inline void create_id(const node& nid)             { ((stack_id*)this)[3] = ((const stack_id*)&nid)[3] + 1; }
		inline node* next_pointer()                        { return (node*)((uint64_t)n_ & 0x0000ffffffffffff); }

		// 32 bit pointers are "full", so I use another 32bit piece of data for the counter
		// on 32bit x86, we can swap the entire 64 bits without a lock
#elif PROCESSOR_BITS == 32
		using stack_id = uint32_t;
		stack_id t_;

		inline node() __noexcept : n_{ nullptr }, t_{ 0 }  { }
		inline node(node* n) __noexcept : n_{ n }, t_{ 0 } { }
		inline void create_id(const node& nid)             { t_ = nid.t_ + 1; }
		inline node* next_pointer()                        { return n_; }
#else
		static_assert(false, "unknown pointer size");
#endif
		inline void set(node* n, const node& nid)          { n_ = n; if (n_) create_id(nid); else create_id(*this); }
	};
}
