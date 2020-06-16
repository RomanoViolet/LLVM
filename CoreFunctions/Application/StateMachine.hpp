#ifndef _STATEMACHINE_HPP_
#define _STATEMACHINE_HPP_

#include <clang-c/Index.h>
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
      BASENAME_COLLECTION,
      IONAME_COLLECTION,
      IONAMESPACE_COLLECTION,
      IODIRECTION_COLLECTION,
      IOTYPE_COLLECTION,
      IDLE  ///< No explicit action to be performed. Waiting for expected event.
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

    void AdvanceStateMachine( const CXCursor cursor );
    void DoInStateAction( const State currentState, const CXCursor cursor );

  private:
    State _currentState;
    const std::string _classToInspect;
    void ComputeTransition( const Event event );
    State GetNewState( const State currentState, const Event event );
    std::string toString( CXString cxString );
    void ResetAllData( );
    void CollectNamespace( const CXCursor cursor );
    void CollectClassName( const CXCursor cursor );
    void CollectBaseClassName( const CXCursor cursor );
    void CollectIOName( const CXCursor cursor );

    struct IODetails {
      std::string _ioName;
      std::string _type;
      std::string _direction;
    };

    struct ClassDetails {
      std::string _name;
      std::string _namespace = "";
      std::string _baseclass;
      std::vector< IODetails > _io;

      void clear( );
    };

    ClassDetails _classDetails;

    // sugar
    using _currentState_t = State;
    using _newState_t = State;

    // ruleset: _currentState >--- Event ----> _newState
    // Multiple _newStates may be reachable from _currentState based on Events (thus, vector<...>)
    std::map< _currentState_t, std::vector< std::pair< Event, _newState_t > > > rules;

  };  // class StateMachine;
}  // namespace RomanoViolet
#endif  // !_STATEMACHINE_HPP_
