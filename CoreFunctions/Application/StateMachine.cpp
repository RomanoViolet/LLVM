#include "StateMachine.hpp"

namespace RomanoViolet
{
  // constructor
  StateMachine::StateMachine( const std::string initialClass )
      : _currentState( State::INIT ), _classToInspect( initialClass )
  {
    // the rules
    // Priority in case of conflicts: The event which comes first in the listing.
    this->rules[State::INIT] = { { Event::NAMESPACE, State::NAMESPACE_COLLECTION },
                                 { Event::CLASS_DECLARATION, State::CLASSNAME_COLLECTION } };

    this->rules[State::NAMESPACE_COLLECTION]
        = { { Event::NAMESPACE, State::NAMESPACE_COLLECTION },
            { Event::CLASS_DECLARATION, State::CLASSNAME_COLLECTION } };

    this->rules[State::CLASSNAME_COLLECTION]
        = { { Event::NAMESPACE, State::NAMESPACE_COLLECTION },
            { Event::CLASSNAME_MATCH_AND_BASECLASS_SPECIFIER, State::WAITING_FOR_TYPEREF } };

    this->rules[State::WAITING_FOR_TYPEREF] = { { Event::NAMESPACE, State::NAMESPACE_COLLECTION },
                                                { Event::TYPEREF, State::BASENAME_COLLECTION } };

    this->rules[State::BASENAME_COLLECTION]
        = { { Event::NAMESPACE, State::NAMESPACE_COLLECTION },
            { Event::FIELD_DECLARATION, State::IONAME_COLLECTION } };

    this->rules[State::IONAME_COLLECTION]
        = { { Event::NAMESPACE, State::NAMESPACE_COLLECTION },
            { Event::NAME_SPACE_REFERENCE, State::IONAMESPACE_COLLECTION },
            { Event::TEMPLATE_REFERENCE, State::IODIRECTION_COLLECTION } };

    this->rules[State::IONAMESPACE_COLLECTION]
        = { { Event::NAMESPACE, State::NAMESPACE_COLLECTION },
            { Event::NAME_SPACE_REFERENCE, State::IONAMESPACE_COLLECTION },
            { Event::TEMPLATE_REFERENCE, State::IODIRECTION_COLLECTION } };

    this->rules[State::IODIRECTION_COLLECTION]
        = { { Event::NAMESPACE, State::NAMESPACE_COLLECTION },
            { Event::TYPEREF, State::IOTYPE_COLLECTION },
            { Event::FIELD_DECLARATION, State::IONAME_COLLECTION } };

    this->_currentState = State::INIT;
  }

  StateMachine::State StateMachine::GetNewState( const State currentState, const Event event )
  {
    // get the vector containing all events, newState pairs
    std::vector< std::pair< Event, _newState_t > > possibilities = this->rules[currentState];

    // The new target state
    StateMachine::State targetState;

    // Index into the vector based on the event
    for ( const std::pair< Event, _newState_t > thisPossibility : possibilities ) {
      if ( thisPossibility.first == event ) {
        targetState = thisPossibility.second;
        break;
      }
    }

    // process events, such as (_currentState == NAMESPACE && !((event == CLASS_DECLARATION) ||
    // (event == NAMESPACE))
    if ( ( this->_currentState == State::NAMESPACE_COLLECTION )
         && ( !( ( event == Event::NAMESPACE ) || ( event == Event::CLASS_DECLARATION ) ) ) ) {
      targetState = State::INIT;
    }

    return ( targetState );
  }

  std::string StateMachine::toString( CXString cxString )
  {
    std::string string = clang_getCString( cxString );
    clang_disposeString( cxString );
    return string;
  }

  void StateMachine::DoInStateAction( const StateMachine::State currentState,
                                      const CXCursor cursor )
  {
    switch ( currentState ) {
      case StateMachine::State::INIT: {
        this->ResetAllData( );
        break;
      }
      case StateMachine::State::NAMESPACE_COLLECTION: {
        this->CollectNamespace( cursor );
        break;
      }

      default:
        break;
    }
  }

  void StateMachine::ComputeTransition( const Event event )
  {
  }

  // reads the AST line by line in order to advance the state machine
  void StateMachine::AdvanceStateMachine( const CXCursor cursor )
  {
    const CXCursorKind kind = clang_getCursorKind( cursor );
    State newState;
    switch ( kind ) {
      case CXCursorKind ::CXCursor_Namespace: {
        newState = this->GetNewState( this->_currentState, Event::NAMESPACE );
        this->DoInStateAction( newState, cursor );
        this->_currentState = newState;
        break;
      }

      case CXCursorKind::CXCursor_ClassDecl: {
        newState = this->GetNewState( this->_currentState, Event::NAMESPACE );
        this->DoInStateAction( newState, cursor );
        this->_currentState = newState;
        break;
      }

      default:
        newState = State::INIT;
        this->DoInStateAction( newState, cursor );
        this->_currentState = newState;
        break;
    }
  }

  void StateMachine::ResetAllData( )
  {
    this->_classDetails.clear( );
  }

  void StateMachine::ClassDetails::clear( )
  {
    this->_baseclass.clear( );
    this->_name.clear( );
    this->_namespace.clear( );
    this->_io.clear( );
  }

  void StateMachine::CollectNamespace( const CXCursor cursor )
  {
    // TODO: Clear namespace on entering the namespace the first time.
    if ( this->_classDetails._namespace.empty( ) ) {
      this->_classDetails._namespace.append( this->toString( clang_getCursorSpelling( cursor ) ) );
    } else {
      this->_classDetails._namespace.append(
          "::" + this->toString( clang_getCursorSpelling( cursor ) ) );
    }
  }  // StateMachine::CollectNamespace
}  // namespace RomanoViolet
