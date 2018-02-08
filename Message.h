#pragma once
#include <iostream>
#include <math.h>

struct Message
{
	int		Sender;
	int		Receiver;
	int		Msg;
	double	SendTime;
	void*	ExtraInfo;

	Message() : Sender(-1), Receiver(-1), Msg(-1), SendTime(-1){}

	Message(int sender, int receiver, int msg, double delay, void* info = NULL) 
		:Sender(sender), Receiver(receiver), Msg(msg), SendTime(delay), ExtraInfo(info){}
};

const double SmallestDelay = 0.25;	//consider 1/4 of a sec as the same msg

//prevent sending the same msg more than once in a short amount of time
inline bool operator==(const Message& m1, const Message& m2)
{
	return (fabs(m1.SendTime - m2.SendTime) < SmallestDelay) &&
		(m1.Sender == m2.Sender) &&
		(m1.Receiver == m2.Receiver) &&
		(m1.Msg == m2.Msg);
}

//overload the operator to keep sorted by the sendTime value
inline bool operator<(const Message& m1, const Message& m2)
{
	if (m1 == m2)
		return false;
	else
		return (m1.SendTime < m2.SendTime);
}

//help to deference the ExtraInfo to the required type
template <class T>
inline T DereferenceToType(void* p)
{
	return *(T*)(p);
}
