#ifndef STATE_HANDLING_H
#define STATE_HANDLING_H

#include <stdint.h>
#include <type_traits>

#include "debug.h"

// struct NOT_CONNECTED{};
// struct CONNECTED{};
// struct MOVING{};

extern int32_t targetPosition;

enum struct State
{
  NOT_CONNECTED,
  CONNECTED,
  MOVING
};

extern State curState;

enum struct EventType
{
  NONE,
  CONNECT,
  MOVE,
  HALT
};

struct Event
{
  int32_t param;
  EventType eventType;
};

extern Event curEvent;

enum struct Result
{
  OK,
  NOT_CONNECTED
};

void setupStateHandling();

template <State S, EventType E>
Result submitEventInternal(int32_t param);

template <EventType E>
Result submitEvent(int32_t param)
{
  switch (curState)
  {
  case State::NOT_CONNECTED:
    return submitEventInternal<State::NOT_CONNECTED, E>(param);
    break;
  case State::CONNECTED:
    return submitEventInternal<State::CONNECTED, E>(param);
    break;
  case State::MOVING:
    return submitEventInternal<State::MOVING, E>(param);
    break;
  }
  return Result::OK;
}

String mapState(State s);

#endif // STATE_HANDLING_H
