#include "StateMachine.hpp"
#include <cassert>
#include <iostream>
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
            { Event::BASECLASS_SPECIFIER, State::BASENAME_COLLECTION } };

    this->rules[State::BASENAME_COLLECTION]
        = { { Event::NAMESPACE, State::NAMESPACE_COLLECTION },
            { Event::FIELD_DECLARATION, State::IONAME_COLLECTION },
            { Event::TYPEREF, State::IDLE_AFTER_BASENAMECOLLECTION } };

    this->rules[State::IDLE_AFTER_BASENAMECOLLECTION]
        = { { Event::FIELD_DECLARATION, State::IONAME_COLLECTION } };

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
            { Event::FIELD_DECLARATION, State::IONAME_COLLECTION },
            { Event::NAME_SPACE_REFERENCE, State::IDLE_AFTER_IODIRECTIONCOLLECTION } };

    this->rules[State::IDLE_AFTER_IODIRECTIONCOLLECTION]
        = { { Event::NAMESPACE, State::NAMESPACE_COLLECTION },
            { Event::TYPEREF, State::IOTYPE_COLLECTION } };

    this->rules[State::IOTYPE_COLLECTION]
        = { { Event::NAMESPACE, State::NAMESPACE_COLLECTION },
            { Event::FIELD_DECLARATION, State::IONAME_COLLECTION },
            { Event::OTHERS, State::IDLE_AFTER_BASENAMECOLLECTION } };

    this->_currentState = State::INIT;
  }

  StateMachine::State StateMachine::GetNewState( const State currentState, const Event event )
  {
    // get the vector containing all events, newState pairs
    std::vector< std::pair< Event, _newState_t > > possibilities = this->rules[currentState];

    // was a new target state successfully computed?
    bool isNewTargetStateComputed = false;

    // The new target state
    StateMachine::State targetState;

    // Index into the vector based on the event
    for ( const std::pair< Event, _newState_t > thisPossibility : possibilities ) {
      if ( thisPossibility.first == event ) {
        targetState = thisPossibility.second;
        isNewTargetStateComputed = true;
        break;
      }
    }

    // Is OTHERS event listed?
    if ( !isNewTargetStateComputed ) {
      for ( const std::pair< Event, _newState_t > thisPossibility : possibilities ) {
        if ( thisPossibility.first == Event::OTHERS ) {
          // In case no event matches, the OTHERS rule is taken.
          // TODO Rename Event::OTHERS
          targetState = thisPossibility.second;
          isNewTargetStateComputed = true;
          break;
        }
      }
    }
    // process events, such as (_currentState == NAMESPACE && !((event == CLASS_DECLARATION) ||
    // (event == NAMESPACE))
    if ( ( this->_currentState == State::NAMESPACE_COLLECTION )
         && ( !( ( event == Event::NAMESPACE ) || ( event == Event::CLASS_DECLARATION ) ) ) ) {
      targetState = State::INIT;
      isNewTargetStateComputed = true;
    }

    if ( !isNewTargetStateComputed ) {
      // if no new target state is computed, then state in the current state
      targetState = currentState;
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

      case StateMachine::State::CLASSNAME_COLLECTION: {
        this->CollectClassName( cursor );
        break;
      }

      case StateMachine::State::BASENAME_COLLECTION: {
        this->CollectBaseClassName( cursor );
        break;
      }

      case StateMachine::State::IONAME_COLLECTION: {
        this->CollectIOName( cursor );
        break;
      }

      case StateMachine::State::IDLE_AFTER_BASENAMECOLLECTION: {
        break;
      }

      case StateMachine::State::IONAMESPACE_COLLECTION: {
        this->CollectIONameSpace( cursor );
        break;
      }

      case StateMachine::State::IODIRECTION_COLLECTION: {
        this->CollectIODirection( cursor );
        break;
      }

      case StateMachine::State::IOTYPE_COLLECTION: {
        this->CollectIOType( cursor );
        break;
      }

      case StateMachine::State::IDLE_AFTER_IODIRECTIONCOLLECTION: {
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
        newState = this->GetNewState( this->_currentState, Event::CLASS_DECLARATION );
        this->DoInStateAction( newState, cursor );

        // fi the class name does not match the one that we are after
        if ( this->_classDetails._name.compare( this->_classToInspect ) != 0 ) {
          newState = StateMachine::State::INIT;
          this->DoInStateAction( newState, cursor );
        } else {
          this->_currentState = newState;
        }
        break;
      }

      case CXCursorKind::CXCursor_CXXBaseSpecifier: {
        assert( true && "This case should never have happened" );
        // if class name matches the one that we should find
        if ( this->_classDetails._name.compare( this->_classToInspect ) == 0 ) {
          newState = this->GetNewState( this->_currentState, Event::BASECLASS_SPECIFIER );
        } else {
          // if it is not the class that is being sought, go back to initial state.
          newState = StateMachine::State::INIT;
        }

        this->DoInStateAction( newState, cursor );
        this->_currentState = newState;
        break;
      }

      case CXCursorKind::CXCursor_TypeRef: {
        // if class name matches the one that we should find
        if ( this->_classDetails._name.compare( this->_classToInspect ) == 0 ) {
          newState = this->GetNewState( this->_currentState, Event::TYPEREF );
        } else {
          // if it is not the class that is being sought, go back to initial state.
          newState = StateMachine::State::INIT;
        }

        this->DoInStateAction( newState, cursor );
        this->_currentState = newState;
        break;
      }

      case CXCursorKind::CXCursor_FieldDecl: {
        // The new state is always IONAME_COLLECTION
        newState = this->GetNewState( this->_currentState, Event::FIELD_DECLARATION );

        size_t nIOPrior = this->_classDetails._io.size( );
        this->DoInStateAction( newState, cursor );

        // only if a new IO has been collected, advance the state
        if ( this->_classDetails._io.size( ) > nIOPrior ) {
          this->_currentState = newState;
        }

        break;
      }

      case CXCursorKind::CXCursor_NamespaceRef: {
        newState = this->GetNewState( this->_currentState, Event::NAME_SPACE_REFERENCE );
        this->DoInStateAction( newState, cursor );
        this->_currentState = newState;
        break;
      }

      case CXCursorKind::CXCursor_TemplateRef: {
        newState = this->GetNewState( this->_currentState, Event::TEMPLATE_REFERENCE );
        this->DoInStateAction( newState, cursor );
        this->_currentState = newState;
        break;
      }

      default:
        // newState = State::INIT;
        // this->DoInStateAction( newState, cursor );
        // this->_currentState = newState;
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
    if ( this->_classDetails._namespace.empty( ) ) {
      this->_classDetails._namespace.append( this->toString( clang_getCursorSpelling( cursor ) ) );
    } else {
      this->_classDetails._namespace.append(
          "::" + this->toString( clang_getCursorSpelling( cursor ) ) );
    }
  }  // StateMachine::CollectNamespace

  void StateMachine::CollectClassName( const CXCursor cursor )
  {
    this->_classDetails._name = this->toString( clang_getCursorSpelling( cursor ) );
  }  // StateMachine::CollectClassName

  void StateMachine::CollectBaseClassName( const CXCursor cursor )
  {
    const CXType type = clang_getCursorType( cursor );
    this->_classDetails._baseclass = this->toString( clang_getTypeSpelling( type ) );
  }  // StateMachine::CollectBaseClassName

  void StateMachine::CollectIOName( const CXCursor cursor )
  {
    // we collect only public facing IO
    if ( clang_getCXXAccessSpecifier( cursor ) == CX_CXXAccessSpecifier::CX_CXXPublic ) {
      IODetails _io;
      _io._ioName = this->toString( clang_getCursorSpelling( cursor ) );
      this->_classDetails._io.emplace_back( _io );
    }

  }  // StateMachine::CollectBaseClassName

  void StateMachine::CollectIONameSpace( const CXCursor cursor )
  {
    // Entry to this state is only via IONAME_COLLECTION state wherein an IODetails instance is
    // already created.
    // number of io instances already recorded
    size_t nInstances = this->_classDetails._io.size( );

    // extract the latest instance.
    IODetails &_io = this->_classDetails._io.at( nInstances - 1 );
    if ( _io._namespace.empty( ) ) {
      _io._namespace = this->toString( clang_getCursorSpelling( cursor ) );
    } else {
      _io._namespace.append( "::" + this->toString( clang_getCursorSpelling( cursor ) ) );
    }
  }  // StateMachine::CollectIONameSpace

  void StateMachine::CollectIODirection( const CXCursor cursor )
  {
    // Entry to this state is is only possible by passing through the state IONAME_COLLECTION
    // wherein an IODetails instance is created. already created. number of io instances already
    // recorded
    size_t nInstances = this->_classDetails._io.size( );

    // extract the latest instance.
    IODetails &_io = this->_classDetails._io.at( nInstances - 1 );

    std::string _ioTemplate = this->toString( clang_getCursorSpelling( cursor ) );
    if ( _ioTemplate.compare( "TypeInputInterface" ) == 0 ) {
      // Input type
      _io._direction = "In";
    } else {
      _io._direction = "Out";
    }
  }
  // StateMachine::CollectIODirection

  void StateMachine::CollectIOType( const CXCursor cursor )
  {
    // Entry to this state is is only possible by passing through the state IONAME_COLLECTION
    // wherein an IODetails instance is created. already created. number of io instances already
    // recorded
    size_t nInstances = this->_classDetails._io.size( );

    // extract the latest instance.
    IODetails &_io = this->_classDetails._io.at( nInstances - 1 );

    const CXType type = clang_getCursorType( cursor );
    _io._type = this->toString( clang_getTypeSpelling( type ) );
  }
  // StateMachine::CollectIOType

}  // namespace RomanoViolet
