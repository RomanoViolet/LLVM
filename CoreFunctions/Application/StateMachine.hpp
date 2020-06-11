#ifndef _STATEMACHINE_HPP_
#define _STATEMACHINE_HPP_

namespace RomanoViolet
{
  class StateMachine
  {
  public:
    StateMachine( ) = default;
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

    enum class Event : short {

    };

    void advanceStateMachine( const Event event );
    void doInStateAction( );

  }  // class StateMachine;
}  // namespace RomanoViolet
#endif  // !_STATEMACHINE_HPP_
