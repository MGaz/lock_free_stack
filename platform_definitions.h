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

// Preprocessor magic to figure out what platform we're on.
// This is not exhaustive by any means - it's just a quick and dirty place to get started for most projects
#if _WIN32
#	if _WIN64
#		define PROCESSOR_BITS		64
#	else
#		define PROCESSOR_BITS		32
#	endif
#endif

#if __GNUC__
#	if __x86_64__ || __ppc64__
#		define PROCESSOR_BITS		64
#	else
#		define PROCESSOR_BITS		32
#	endif
#endif

#ifdef __APPLE__
#	import "TargetConditionals.h"
#	ifdef __MACH__
#		define PROCESSOR_BITS		64
#	endif
#endif
