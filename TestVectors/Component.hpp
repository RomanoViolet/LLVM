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
    /**
     * @brief Component class, derives from a specific template, here "TypeHighAssuranceComponent"
     * @details Inheritance ensures that the component meets the minimum interface requirements
     * expected from a class of "TypeHighAssuranceComponent" objects.
     * The details of what constitutes the minimum requirement is adjustable.
     */
    class Component : public TypeHighAssuranceComponent
    {
    public:
      enum class ErrorCode : short { NO_ERROR = 0U, BAD_INPUT_DATA = 1U };

      Component( )
          : a_in( ::RomanoViolet::TypeInputInterface< ::RomanoViolet::InterfaceA >( ) )
          , b_in( ::RomanoViolet::TypeInputInterface< ::RomanoViolet::InterfaceB >( ) )
          , b_out( ::RomanoViolet::TypeOutputInterface< ::RomanoViolet::InterfaceB >( ) )
          , b2( ::RomanoViolet::InterfaceB( ) )
          , c( int( ) )
      {
      }
      // The template TypeInputInterface<...> is used to declare an input
      ::RomanoViolet::TypeInputInterface< ::RomanoViolet::InterfaceA > a_in;
      ::RomanoViolet::TypeOutputInterface< ::RomanoViolet::InterfaceB > b_in;

      // The template TypeOutputInterface<...> is used to declare an output
      ::RomanoViolet::TypeOutputInterface< ::RomanoViolet::InterfaceB > b_out;

      // The direction of b2 is ambiguous.
      ::RomanoViolet::InterfaceB b2;

      // The direction of c is ambiguous.
      int c;

      // public member methods
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
