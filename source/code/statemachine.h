#ifndef STATEMACHINE_H
#define STATEMACHINE_H
#include "wx/timer.h"

class StateMachine : protected wxTimer
{
public:
	StateMachine();
	virtual ~StateMachine();
	
	void ChangeState(int nextState, int duration = 0);
	void RepeatState();
	void Stop();

	int GetStateDuration() const { return _lastDuration; }
	int GetNextState() const { return _nextState; }

protected:
	virtual void DoState(int state);

private:
	virtual void Notify();

	int	_lastDuration;
	int _currentState;
	int _nextState;
};

#endif