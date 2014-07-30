///////////////////////////////////////////////////////////////////////////////
///                         SIMPLE LOCK FREE STACK                          ///
///           Copyright (c) 2014 Michael Gazonda - http://mgaz.ca           ///
///                              CPOL Licensed                              ///
///       See CPOL.htm, or http://www.codeproject.com/info/cpol10.aspx      ///
///               http://www.codeproject.com/Articles/801537/               ///
///                 https://github.com/MGaz/lock_free_stack                 ///
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include "node.h"
namespace gaz
{
	class data : public node
	{
		// Put your data in here
		// You can make your own data structure to throw in the stack, but it _must_ derive from node
		
		// Just an example, serves no purpose other than that - you may remove
//		int x_;
//		char buffer_[10];
	};	
}
