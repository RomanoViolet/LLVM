#ifndef INTERFACE_B_HPP_
#define INTERFACE_B_HPP_

#include <BoundedTypes/CustomTypes.hpp>
#include <BoundedTypes/SafeTypes.hpp>
#include <Library/InterfaceTypes/Type_HighAssuranceInterface.hpp>

namespace RomanoViolet
{
  class InterfaceB
  {
  public:
    InterfaceB( ) = default;

    RomanoViolet::SafeType< 1, 4 > minWithIntegerBounds{2.F};
    VelocityType velocity{0.51F};

  private:
  };  // namespace InterfaceB
}  // namespace RomanoViolet

#endif  // INTERFACE_B_HPP_
