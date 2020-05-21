#ifndef TYPE_INPUT_INTERFACE_HPP_
#define TYPE_INPUT_INTERFACE_HPP_

namespace RomanoViolet
{
  template < typename T >
  class TypeInputInterface
  {
  public:
    TypeInputInterface( ) = default;
    void setValue( const T &value );
    const T &getValue( ) const;

  protected:
    // likely not required on a per interface basis due to usage of bounded types.
    // void doPreconditionCheck( );

  private:
    T _value;
  };
}  // namespace RomanoViolet

#include "Type_InputInterface.inl"

#endif  // TYPE_INPUT_INTERFACE_HPP_
