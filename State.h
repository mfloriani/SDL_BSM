#pragma once

class Message;

template <class go_type>
class State
{
public:
	virtual ~State(){};
	virtual void Enter(go_type*) = 0;
	virtual void Execute(go_type*) = 0;
	virtual void Exit(go_type*) = 0;
	virtual bool OnMessage(go_type*, const Message&) = 0;
};
