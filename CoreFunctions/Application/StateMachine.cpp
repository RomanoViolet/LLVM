#include "StateMachine.hpp"

namespace RomanoViolet
{
  // constructor
  StateMachine::StateMachine( const std::string initialClass ) : _classToInspect( initialClass )
  {
    // the rules
    this->rules[State::INIT] = { { Event::NAMESPACE, State::NAMESPACE_COLLECTION },
                                 { Event::CLASS_DECLARATION, State::CLASSNAME_COLLECTION } };

    this->rules[State::CLASSNAME_COLLECTION]
        = { { Event::NAMESPACE, State::NAMESPACE_COLLECTION },
            { Event::CLASSNAME_MATCH_AND_BASECLASS_SPECIFIER, State::WAITING_FOR_TYPEREF } };
  }
}  // namespace RomanoViolet
