#include "Messenger.h"
#include "../Utils/WindowsClock.h"
#include "../GameObjects/GameObjectManager.h"


Messenger* Messenger::GetInstance()
{
	static Messenger instance;
	return &instance;
}

void Messenger::DeliverMsg(GameObject* receiver, const Message& msg)
{
	receiver->HandleMessage(msg);
}

//process the message and deliver to the receiver or put inside the queue to be delivered later
void Messenger::SendMsg(int sender, int receiver, double delay, int msg, void* extraInfo)
{
	GameObject* senderGO = GoManager->GetGoFromId(sender);
	GameObject* receiverGO = GoManager->GetGoFromId(receiver);

	if (receiver == NULL) return;	//receiver unknown

	Message message(sender, receiver, msg, delay, extraInfo);

	if (delay <= 0.0f)	//must deliver now
	{
		DeliverMsg(receiverGO, message);
	}
	else //schedule the delivery of the message
	{
		double curTime = WinClock->GetCurTime();
		message.SendTime = curTime + delay;
		m_delayedMessages.insert(message);
		std::cout << "Sender: " << message.Sender << " Receiver: " << message.Receiver << std::endl;
	}
}

//deliver the queued messages to the receivers when its time to do that
void Messenger::SendDelayedMsg()
{
	double curTime = WinClock->GetCurTime();

	while (!m_delayedMessages.empty() &&
		(m_delayedMessages.begin()->SendTime < curTime) &&
		(m_delayedMessages.begin()->SendTime > 0))
	{
		const Message& message = *m_delayedMessages.begin();

		GameObject* receiverGo = GoManager->GetGoFromId(message.Receiver);
		DeliverMsg(receiverGo, message);
		m_delayedMessages.erase(m_delayedMessages.begin());
	}
}
