#ifndef CUSTOM_TYPES_HPP_
#define CUSTOM_TYPES_HPP_

#include "SafeTypes.hpp"

using VelocityType = RomanoViolet::SafeType< Fraction( 1, 2 ), Fraction( 3, 4 ) >;
using CountingType = RomanoViolet::SafeType< 1, 4 >;

#endif  // !CUSTOM_TYPES_HPP_
