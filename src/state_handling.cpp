#include "state_handling.h"

State curState = State::NOT_CONNECTED;
Event curEvent = {0, EventType::NONE};

bool moveTo(int32_t position)
{
  return true;
}

// handle state NOT_CONNECTED

template <>
Result submitEventInternal<State::NOT_CONNECTED, EventType::CONNECT>(int32_t param)
{
  serialPrintln("<entry> submitEventInternal<State::NOT_CONNECTED, EventType::CONNECT>(param=%d), currentState=%s", param, mapState(curState));
  if (param == 1)
  {
    curState = State::CONNECTED;
  }
  serialPrintln("<exit> submitEventInternal<State::NOT_CONNECTED, EventType::CONNECT>(), currentState=%s", mapState(curState));
  return Result::OK;
}

template <>
Result submitEventInternal<State::NOT_CONNECTED, EventType::MOVE>(int32_t param)
{
  serialPrintln("<entry> submitEventInternal<State::NOT_CONNECTED, EventType::MOVE>(param=%d), currentState=%s", param, mapState(curState));
  serialPrintln("<exit> submitEventInternal<State::NOT_CONNECTED, EventType::MOVE>(), currentState=%s", mapState(curState));
  return Result::NOT_CONNECTED;
}

template <>
Result submitEventInternal<State::NOT_CONNECTED, EventType::HALT>(int32_t param)
{
  serialPrintln("<entry> submitEventInternal<State::NOT_CONNECTED, EventType::HALT>(param=%d), currentState=%s", param, mapState(curState));
  serialPrintln("<exit> submitEventInternal<State::NOT_CONNECTED, EventType::HALT>(), currentState=%s", mapState(curState));
  return Result::NOT_CONNECTED;
}

template <>
Result submitEventInternal<State::NOT_CONNECTED, EventType::NONE>(int32_t param)
{
  serialPrintln("<entry> submitEventInternal<State::NOT_CONNECTED, EventType::NONE>(param=%d), currentState=%s", param, mapState(curState));
  serialPrintln("<exit> submitEventInternal<State::NOT_CONNECTED, EventType::NONE>(), currentState=%s", mapState(curState));
  return Result::OK;
}

// handle state CONNECTED

template <>
Result submitEventInternal<State::CONNECTED, EventType::CONNECT>(int32_t param)
{
  serialPrintln("<entry> submitEventInternal<State::CONNECTED, EventType::CONNECT>(param=%d), currentState=%s", param, mapState(curState));
  if (param == 0)
  {
    curState = State::NOT_CONNECTED;
  }
  serialPrintln("<exit> submitEventInternal<State::CONNECTED, EventType::CONNECT>(), currentState=%s", mapState(curState));
  return Result::OK;
}

template <>
Result submitEventInternal<State::CONNECTED, EventType::MOVE>(int32_t param)
{
  serialPrintln("<entry> submitEventInternal<State::CONNECTED, EventType::MOVE>(param=%d), currentState=%s", param, mapState(curState));
  targetPosition = param;
  curState = State::MOVING;
  serialPrintln("<exit> submitEventInternal<State::CONNECTED, EventType::MOVE>(), currentState=%s", mapState(curState));
  return Result::OK;
}

template <>
Result submitEventInternal<State::CONNECTED, EventType::HALT>(int32_t param)
{
  serialPrintln("<entry> submitEventInternal<State::CONNECTED, EventType::HALT>(param=%d), currentState=%s", param, mapState(curState));
  serialPrintln("<exit> submitEventInternal<State::CONNECTED, EventType::HALT>(), currentState=%s", mapState(curState));
  return Result::OK;
}

template <>
Result submitEventInternal<State::CONNECTED, EventType::NONE>(int32_t param)
{
  serialPrintln("<entry> submitEventInternal<State::CONNECTED, EventType::NONE>(param=%d), currentState=%s", param, mapState(curState));
  serialPrintln("<exit> submitEventInternal<State::CONNECTED, EventType::NONE>(), currentState=%s", mapState(curState));
  return Result::OK;
}

// handle state MOVING

template <>
Result submitEventInternal<State::MOVING, EventType::CONNECT>(int32_t param)
{
  serialPrintln("<entry> submitEventInternal<State::MOVING, EventType::CONNECT>(param=%d), currentState=%s", param, mapState(curState));
  if (param == 0)
  {
    curState = State::NOT_CONNECTED;
  }
  serialPrintln("<exit> submitEventInternal<State::MOVING, EventType::CONNECT>(), currentState=%s", mapState(curState));
  return Result::OK;
}

template <>
Result submitEventInternal<State::MOVING, EventType::MOVE>(int32_t param)
{
  serialPrintln("<entry> submitEventInternal<State::MOVING, EventType::MOVE>(param=%d), currentState=%s", param, mapState(curState));
  targetPosition = param;
  // if (moveTo(param))
  // {
  //   // arrived at target
  //   curState = State::CONNECTED;
  // }
  serialPrintln("<exit> submitEventInternal<State::MOVING, EventType::MOVE>(), currentState=%s", mapState(curState));
  return Result::OK;
}

template <>
Result submitEventInternal<State::MOVING, EventType::HALT>(int32_t param)
{
  serialPrintln("<entry> submitEventInternal<State::MOVING, EventType::HALT>(param=%d), currentState=%s", param, mapState(curState));
  curState = State::CONNECTED;
  serialPrintln("<exit> submitEventInternal<State::MOVING, EventType::HALT>(), currentState=%s", mapState(curState));
  return Result::OK;
}

template <>
Result submitEventInternal<State::MOVING, EventType::NONE>(int32_t param)
{
  serialPrintln("<entry> submitEventInternal<State::MOVING, EventType::NONE>(param=%d), currentState=%s", param, mapState(curState));
  serialPrintln("<exit> submitEventInternal<State::MOVING, EventType::NONE>(), currentState=%s", mapState(curState));
  return Result::OK;
}

String mapState(State s)
{
  switch (s)
  {
  case State::NOT_CONNECTED:
    return "NOT_CONNECTED";
  case State::CONNECTED:
    return "CONNECTED";
  case State::MOVING:
    return "MOVING";
  }
  return "!!! NONE !!!";
}
