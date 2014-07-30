///////////////////////////////////////////////////////////////////////////////
///                         SIMPLE LOCK FREE STACK                          ///
///           Copyright (c) 2014 Michael Gazonda - http://mgaz.ca           ///
///                              CPOL Licensed                              ///
///       See CPOL.htm, or http://www.codeproject.com/info/cpol10.aspx      ///
///               http://www.codeproject.com/Articles/801537/               ///
///                 https://github.com/MGaz/lock_free_stack                 ///
///////////////////////////////////////////////////////////////////////////////
#include "stack.h"
#include <thread>
#include <iostream>
// This is it! This is the magic right here.
// A simple lock-free stack.

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
