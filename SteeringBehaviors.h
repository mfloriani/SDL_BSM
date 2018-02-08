#pragma once
#include "GameObject.h"

#define BEHAVIORS_COUNT 3

class SteeringBehaviors
{
private:
	enum BehaviorsType
	{
		none,
		seek,
		flee
	};

private:
	GameObject* m_agent;
	Vector2D	m_steeringForce;
	Vector2D	m_target;
	bool		m_behaviorsStatus[BEHAVIORS_COUNT];	//must match to the totals of BehaviorsType options

	//bool		IsOn(BehaviorsType bt){ return m_behaviorsStatus[bt]; };
	bool		AccumulateForce(Vector2D &curTotalForce, Vector2D forceToAdd);

	Vector2D Seek(Vector2D targetPos);


public:
	SteeringBehaviors(GameObject* agent);
	virtual ~SteeringBehaviors();

	Vector2D Calculate();

	void SwitchSeekOnOff(bool onOff){ m_behaviorsStatus[seek] = onOff; }
	bool IsSeekOn(){ return m_behaviorsStatus[seek]; }

};

