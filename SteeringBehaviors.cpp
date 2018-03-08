#include "SteeringBehaviors.h"

SteeringBehaviors::SteeringBehaviors(GameObject* agent) : m_agent(agent), m_path(NULL)
{
	for (int i = 0; i < BEHAVIORS_COUNT; i++) m_behaviorsStatus[i] = false;
}

SteeringBehaviors::~SteeringBehaviors()
{
	delete m_path;
}

math::Vector2D SteeringBehaviors::Calculate()
{
	m_steeringForce = math::Vector2D(0, 0);

	math::Vector2D force;

	if (IsOn(seek))
	{
		force = Seek(m_target);
		if (!AccumulateForce(m_steeringForce, force)) return m_steeringForce;
	}
	if (IsOn(arrive))
	{
		force = Arrive(m_target, Deceleration::slow);
		if (!AccumulateForce(m_steeringForce, force)) return m_steeringForce;
	}
	if (IsOn(follow_path))
	{
		force = FollowPath();
		if (!AccumulateForce(m_steeringForce, force)) return m_steeringForce;
	}
	
	return m_steeringForce;
}

bool SteeringBehaviors::AccumulateForce(math::Vector2D &curTotalForce, math::Vector2D forceToAdd)
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

math::Vector2D SteeringBehaviors::Seek(math::Vector2D targetPos)
{
	//std::cout << targetPos << std::endl;
	math::Vector2D desiredVel = ((targetPos - m_agent->GetPosition()).normalize()) * m_agent->GetMaxVelocity();
	return (desiredVel - m_agent->GetVelocity());
}

math::Vector2D SteeringBehaviors::Arrive(math::Vector2D targetPos, Deceleration deceleration)
{
	math::Vector2D toTarget = targetPos - m_agent->GetPosition();

	//calculate the distance to the target
	double dist = toTarget.size();

	//std::cout << dist << std::endl;

	if (dist > 0)
	{
		//because Deceleration is enumerated as an int, this value is required to provide fine tweaking of the deceleration..
		const double DecelerationTweaker = 1;

		//calculate the speed required to reach the target given the desired deceleration
		double speed = dist / ((double)deceleration * DecelerationTweaker);

		//make sure the velocity does not exceed the max
		speed = (speed < m_agent->GetMaxVelocity()) ? speed : m_agent->GetMaxVelocity();

		//from here proceed just like Seek except we don't need to normalize 
		//the ToTarget vector because we have already gone to the trouble
		//of calculating its length: dist. 
		math::Vector2D desiredVelocity = toTarget * speed / dist;

		return (desiredVelocity - m_agent->GetVelocity());
	}

	return math::Vector2D(0, 0);
}

math::Vector2D SteeringBehaviors::FollowPath()
{
	if (m_path == NULL) 
		return math::Vector2D(0, 0);//there is no path

	if (m_path->IsCurWaypointClose(m_agent->GetPosition()))
		m_path->SetNextWaypoint();

	if (!m_path->IsPathEnded())
		return Seek(m_path->GetCurrentWaypoint());
	else
		return Arrive(m_path->GetCurrentWaypoint(), fast);

	return math::Vector2D(0, 0);
}
