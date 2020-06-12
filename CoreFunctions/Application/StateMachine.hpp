#ifndef _STATEMACHINE_HPP_
#define _STATEMACHINE_HPP_

#include <map>
#include <string>
#include <vector>
namespace RomanoViolet
{
  class StateMachine
  {
  public:
    /**
     * @brief Construct a new State Machine object
     *
     * @param initialClass: The class from AST which is to be examined.
     */
    StateMachine( const std::string initialClass );

    // All possible states of the state machine.
    enum class State : short {
      INIT,
      NAMESPACE_COLLECTION,
      CLASSNAME_COLLECTION,
      WAITING_FOR_TYPEREF,
      BASENAME_COLLECTION,
      IONAME_COLLECTION,
      IODIRECTION_COLLECTION,
      IOTYPE_COLLECTION
    };

    // Explicitly agreed events in order to transition between states.
    enum class Event : short {
      NAMESPACE,
      CLASS_DECLARATION,
      BASECLASS_SPECIFIER,
      TYPEREF,
      CXX_ACCESS_SPECIFIER,
      FIELD_DECLARATION,
      NAME_SPACE_REFERENCE,
      TEMPLATE_REFERENCE
    };

    void advanceStateMachine( const Event event );
    void doInStateAction( );

  private:
    State _currentState;
    const std::string _classToInspect;
    void computeTransition( std::string astLabel );

    struct IODetails {
      std::string _ioName;
      std::string _type;
      std::string _direction;
    };

    struct ClassDetails {
      std::string _name;
      std::string _namespace = "";
      std::string _baseclass;
      std::vector< IODetails > io;
    };

    ClassDetails _classDetails;

    // sugar
    using _currentState_t = State;
    using _newState_t = State;

    // ruleset: _currentState >--- Event ----> _newState
    // Multiple _newStates may be reachable from _currentState based on Events (thus, vector<...>)
    std::map< _currentState_t, std::vector< Event, _newState_t > > rules;

  };  // class StateMachine;
}  // namespace RomanoViolet
#endif  // !_STATEMACHINE_HPP_
