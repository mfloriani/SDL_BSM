#include "SteeringBehaviors.h"


SteeringBehaviors::SteeringBehaviors(GameObject* agent) : m_agent(agent)
{
	for (int i = 0; i < BEHAVIORS_COUNT; i++)
		m_behaviorsStatus[i] = false;
}


SteeringBehaviors::~SteeringBehaviors()
{
}

Vector2D SteeringBehaviors::Calculate()
{
	m_steeringForce = Vector2D(0, 0);

	Vector2D force;

	if (IsSeekOn())
	{
		force = Seek(m_target);
		if (!AccumulateForce(m_steeringForce, force)) 
			return m_steeringForce;
	}

}

bool SteeringBehaviors::AccumulateForce(Vector2D &curTotalForce, Vector2D forceToAdd)
{
	double curTotallength = curTotalForce.size();
	double remainingLength = m_agent->GetMaxForce() - curTotallength;
	
	if (remainingLength <= 0.0) return false;

	double addingLength = forceToAdd.size();

	if (addingLength < remainingLength)
		curTotalForce += forceToAdd;
	else
		curTotalForce += forceToAdd.normalize() * remainingLength;

	return true;
}

Vector2D SteeringBehaviors::Seek(Vector2D targetPos)
{
	Vector2D desiredVel = (targetPos - m_agent->GetPosition()).normalize() * m_agent->GetMaxVelocity();
	return (desiredVel - m_agent->GetVelocity());
}
