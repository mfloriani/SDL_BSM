#pragma once
#include "State.h"

template<class go_type>
class StateMachine
{
private:
	go_type* m_owner;
	State<go_type>* m_currentState;
	State<go_type>* m_globalState;
	State<go_type>* m_previousState;

public:
	StateMachine(go_type* owner) : m_owner(owner), m_currentState(NULL), m_globalState(NULL), m_previousState(NULL){}
	virtual ~StateMachine(){}

	void SetInitialCurState(State<go_type>* s)
	{
		m_currentState = s;
		m_currentState->Enter(m_owner);
	}
	//void SetCurrentState(State<go_type>* s){ m_currentState = s; }
	void SetGlobalState(State<go_type>* s){ m_globalState = s; }
	//void SetPreviousState(State<go_type>* s){ m_previousState = s; }

	State<go_type>* GetCurrentState() const{ return m_currentState; }
	State<go_type>* GetGlobalState() const{ return m_globalState; }
	State<go_type>* GetPreviousState() const{ return m_previousState; }

	const void Update()
	{
		if (m_globalState) m_globalState->Execute(m_owner);

		if (m_currentState) m_currentState->Execute(m_owner);
	}

	bool HandleMessage(const Message& msg) const
	{
		if (m_currentState && m_currentState->OnMessage(m_owner, msg)) return true;	//if current state handle the msg then get out

		if (m_globalState && m_globalState->OnMessage(m_owner, msg)) return true;	//if current state didn't handle the msg then global should try to process

		return false;
	}

	void ChangeState(State<go_type>* newState)
	{
		if (m_currentState != NULL)
		{
			m_previousState = m_currentState;
			m_currentState->Exit(m_owner);
		}
		m_currentState = newState;
		m_currentState->Enter(m_owner);
	}

	void RevertToPreviousState()
	{
		ChangeState(m_previousState);
	}

	const bool IsInState(const State<go_type>& s)
	{
		if (typeid(*m_currentState) == typeid(s)) return true;

		return false;
	}

};

