#pragma once
#include <set>
#include "Message.h"
#include "../GameObjects/GameObject.h"

#define Msger Messenger::GetInstance()

class GameObject;

class Messenger
{
private:
	Messenger(){}
	Messenger(const Messenger&);
	Messenger& operator=(const Messenger&);

	std::set<Message> m_delayedMessages;

	void DeliverMsg(GameObject* receiver, const Message& msg);

public:
	static Messenger* GetInstance();

	void SendMsg(int sender, int receiver, double delay, int msg, void* extraInfo);
	void SendDelayedMsg();
};

