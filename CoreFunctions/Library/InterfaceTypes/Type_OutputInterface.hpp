#ifndef TYPE_OUTPUT_INTERFACE_HPP_
#define TYPE_OUTPUT_INTERFACE_HPP_

namespace RomanoViolet
{
  template < typename T >
  class TypeOutputInterface
  {
  public:
    TypeOutputInterface( ) = default;
    T getValue( ) const;

  protected:
    // T is required to be of bounded type. Need a way to ensure this.
    void setValue( const T &value );

    // likely not required on a per interface basis due to usage of bounded types.
    void doPostConditionCheck( );

  private:
    T _value;
  };
}  // namespace RomanoViolet

#include "Type_OutputInterface.inl"

#endif  // TYPE_OUTPUT_INTERFACE_HPP_
