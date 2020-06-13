#include "StateMachine.hpp"

namespace RomanoViolet
{
  // constructor
  StateMachine::StateMachine( const std::string initialClass )
      : _currentState( State::INIT ), _classToInspect( initialClass )
  {
    // the rules
    this->rules[State::INIT] = { { Event::NAMESPACE, State::NAMESPACE_COLLECTION },
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
  }

  void StateMachine::ComputeTransition( std::string astLabel )
  {
  }

  // reads the AST line by line in order to advance the state machine
  void StateMachine::AdvanceStateMachine( const std::string astLine )
  {
  }
}  // namespace RomanoViolet
