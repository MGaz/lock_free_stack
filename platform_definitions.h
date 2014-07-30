///////////////////////////////////////////////////////////////////////////////
///                         SIMPLE LOCK FREE STACK                          ///
///           Copyright (c) 2014 Michael Gazonda - http://mgaz.ca           ///
///                              CPOL Licensed                              ///
///       See CPOL.htm, or http://www.codeproject.com/info/cpol10.aspx      ///
///               http://www.codeproject.com/Articles/801537/               ///
///                 https://github.com/MGaz/lock_free_stack                 ///
///////////////////////////////////////////////////////////////////////////////
#pragma once

// Preprocessor magic to figure out what platform we're on.
// This is not exhaustive by any means - it's just a quick and dirty place to get started for most projects

// gcc requires the atomic node constructor to be noexcept
// visual studio 2013 doesn't know what noexcept is
// hence the __noexcept...


#if _WIN32
#	if _WIN64
#		define PROCESSOR_BITS		64
#	else
#		define PROCESSOR_BITS		32
#	endif
#	if _MSC_VER > 1800
#		define __noexcept			noexcept
#	else
#		define __noexcept
#	endif
#endif

#if __GNUC__
#	if __x86_64__ || __ppc64__
#		define PROCESSOR_BITS		64
#	else
#		define PROCESSOR_BITS		32
#	endif
#	define __noexcept				noexcept
#endif

#ifdef __APPLE__
#	import "TargetConditionals.h"
#	ifdef __MACH__
#		define PROCESSOR_BITS		64
#	endif
#	define __noexcept				noexcept
#endif

