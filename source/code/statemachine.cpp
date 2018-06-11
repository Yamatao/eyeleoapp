#include "statemachine.h"

StateMachine::StateMachine() : wxTimer(), _lastDuration(0), _currentState(0), _nextState(0)
{
}

StateMachine::~StateMachine()
{
}
	
void StateMachine::ChangeState(int nextState, int duration)
{
	_lastDuration = duration;
	_nextState = nextState;

	bool success = wxTimer::Start(duration, true);
}

void StateMachine::RepeatState()
{
	ChangeState(_currentState, _lastDuration);
}

void StateMachine::Stop()
{
	wxTimer::Stop();
}

void StateMachine::Notify()
{
	_currentState = _nextState;
	_nextState = 0;
	DoState(_currentState);
}

void StateMachine::DoState(int state)
{
	// override this member
}
