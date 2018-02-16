#pragma once
#include "GameObject.h"

#define BEHAVIORS_COUNT 4

class SteeringBehaviors
{
public:
	enum BehaviorsType
	{
		none,
		seek,
		flee,
		arrive
	};

	enum Deceleration
	{
		slow = 3, normal = 2, fast = 1
	};

private:
	GameObject*		m_agent;
	math::Vector2D	m_steeringForce;
	math::Vector2D	m_target;
	bool			m_behaviorsStatus[BEHAVIORS_COUNT];	//must match to the totals of BehaviorsType options

	bool			AccumulateForce(math::Vector2D &curTotalForce, math::Vector2D forceToAdd);
	math::Vector2D	Seek(math::Vector2D targetPos);
	math::Vector2D	Arrive(math::Vector2D targetPos, Deceleration deceleration);

public:
	SteeringBehaviors(GameObject* agent);
	virtual ~SteeringBehaviors();

	math::Vector2D	Calculate();
	void			SwitchOnOff(BehaviorsType behavior, bool onOff){ m_behaviorsStatus[behavior] = onOff; }
	bool			IsOn(BehaviorsType behavior){ return m_behaviorsStatus[behavior]; }
	void			SetTarget(math::Vector2D target){ m_target = target; };

};

