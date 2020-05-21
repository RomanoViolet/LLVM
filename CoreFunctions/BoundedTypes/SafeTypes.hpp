#ifndef SAFETYPES_HPP_
#define SAFETYPES_HPP_

#include <cassert>

#if ( __cplusplus == 201103L )
// C++11 standards compliant implementation
#include "SafeTypes_CXX11.hpp"

#elif ( __cplusplus == 201402L )
// C++14 standards compliant implementation
#include "SafeTypes_CXX14.hpp"

#elif ( __cplusplus == 201703L )
// C++11 standards compliant implementation
#include "SafeTypes_CXX17.hpp"

#else
// Library cannot be used.
static_assert( "You need minimum C++11 standard to use this library" );
#endif

#endif  // !SAFETYPES_HPP_
