#ifndef COMPONENT_HPP_
#define COMPONENT_HPP_

#include <Library/ComponentTypes/Type_HighAssuranceComponent.hpp>
#include <Library/InterfaceTypes/InterfaceA.hpp>
#include <Library/InterfaceTypes/InterfaceB.hpp>
#include <Library/InterfaceTypes/Type_InputInterface.hpp>
#include <Library/InterfaceTypes/Type_OutputInterface.hpp>
namespace NN
{
  namespace RomanoViolet
  {
    class Component : public TypeHighAssuranceComponent
    {
    public:
      enum class ErrorCode : short { NO_ERROR = 0U, BAD_INPUT_DATA = 1U };

      Component( )
          : a_in( ::RomanoViolet::TypeInputInterface< ::RomanoViolet::InterfaceA >( ) )
          , b_out( ::RomanoViolet::TypeOutputInterface< ::RomanoViolet::InterfaceB >( ) )
      {
      }
      ::RomanoViolet::TypeInputInterface< ::RomanoViolet::InterfaceA > a_in;
      ::RomanoViolet::TypeOutputInterface< ::RomanoViolet::InterfaceB > b_in;
      ::RomanoViolet::TypeOutputInterface< ::RomanoViolet::InterfaceB > b_out;
      ::RomanoViolet::InterfaceB b2;
      int c;

      void initialize( );
      void doPreconditionCheck( );
      void compute( );
      void doPostConditionCheck( );

    private:
      ErrorCode _error;
    };
  }  // namespace RomanoViolet
}  // namespace NN

#endif  // COMPONENT_HPP_
