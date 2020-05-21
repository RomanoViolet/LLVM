#ifndef TYPE_OUTPUT_INTERFACE_INL_
#define TYPE_OUTPUT_INTERFACE_INL_

namespace RomanoViolet
{
  template < typename T >
  T TypeOutputInterface< T >::getValue( ) const
  {
    // dummy return
    return ( T( ) );
  }

  template < typename T >
  void TypeOutputInterface< T >::doPostConditionCheck( )
  {
    this->_value.doPreconditionCheck( );
  }
}  // namespace RomanoViolet
#endif  // TYPE_OUTPUT_INTERFACE_INL_
