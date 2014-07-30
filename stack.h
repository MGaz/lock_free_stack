///////////////////////////////////////////////////////////////////////////////
///                         SIMPLE LOCK FREE STACK                          ///
///           Copyright (c) 2014 Michael Gazonda - http://mgaz.ca           ///
///                              CPOL Licensed                              ///
///       See CPOL.htm, or http://www.codeproject.com/info/cpol10.aspx      ///
///               http://www.codeproject.com/Articles/801537/               ///
///                 https://github.com/MGaz/lock_free_stack                 ///
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include <atomic>
#include "node.h"
namespace gaz
{
	class stack
	{
	public:
		void push(node* n);
		bool pop(node*& n);
	protected:
		std::atomic<node> head_;
	};	
}
