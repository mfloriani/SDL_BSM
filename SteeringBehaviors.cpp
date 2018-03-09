#include "SteeringBehaviors.h"
#include "World.h"

SteeringBehaviors::SteeringBehaviors(GameObject* agent, World* world) : m_agent(agent), m_path(NULL), m_world(world), m_feelers(3)
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

	if (IsOn(wall_avoidance))
	{
		force = WallAvoidance(m_world->GetWalls());
		if (!AccumulateForce(m_steeringForce, force)) return m_steeringForce;
	}
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
	if (m_path == NULL) return math::Vector2D(0, 0);	//there is no path

	if (m_path->IsCurWaypointClose(m_agent->GetPosition()))
		m_path->SetNextWaypoint();

	if (!m_path->IsPathEnded())
		return Seek(m_path->GetCurrentWaypoint());
	else
		return Arrive(m_path->GetCurrentWaypoint(), fast);

	return math::Vector2D(0, 0);
}

math::Vector2D SteeringBehaviors::WallAvoidance(const std::vector<Wall*> &walls)
{

	//the feelers are contained in a std::vector, m_Feelers
	CreateFeelers();

	double DistToThisIP = 0.0;
	double DistToClosestIP = 9999999999999999999;

	//this will hold an index into the vector of walls
	int ClosestWall = -1;

	math::Vector2D SteeringForce,
		point,         //used for storing temporary info
		ClosestPoint;  //holds the closest intersection point

					   //examine each feeler in turn
	for (unsigned int flr = 0; flr<m_feelers.size(); ++flr)
	{
		//run through each wall checking for any intersection points
		for (unsigned int w = 0; w<walls.size(); ++w)
		{
			if (HasLineIntersection2D(
				m_agent->GetPosition(),
				m_feelers[flr],
				walls[w]->GetFrom(),
				walls[w]->GetTo(),
				DistToThisIP,
				point))
			{
				//is this the closest found so far? If so keep a record
				if (DistToThisIP < DistToClosestIP)
				{
					DistToClosestIP = DistToThisIP;

					ClosestWall = w;

					ClosestPoint = point;
				}
			}
		}//next wall


		 //if an intersection point has been detected, calculate a force  
		 //that will direct the agent away
		if (ClosestWall >= 0)
		{
			//calculate by what distance the projected position of the agent
			//will overshoot the wall
			math::Vector2D OverShoot = m_feelers[flr] - ClosestPoint;

			//create a force in the direction of the wall normal, with a 
			//magnitude of the overshoot
			SteeringForce = walls[ClosestWall]->GetNormal() * OverShoot.size();
		}

	}//next feeler

	return SteeringForce;
}

//------------------------------- CreateFeelers --------------------------
//
//  Creates the antenna utilized by WallAvoidance
//------------------------------------------------------------------------
void SteeringBehaviors::CreateFeelers()
{
	double m_dWallDetectionFeelerLength = 20;
	//feeler pointing straight in front
	m_feelers[0] = m_agent->GetPosition() + m_dWallDetectionFeelerLength * m_agent->GetDirection() * m_agent->GetVelocity().size();

	//feeler to left
	math::Vector2D temp = m_agent->GetDirection();
	temp.rotate(HALF_PI * 3.5);
	m_feelers[1] = m_agent->GetPosition() + m_dWallDetectionFeelerLength / 2.0 * temp;

	//feeler to right
	temp = m_agent->GetDirection();
	temp.rotate(HALF_PI * 0.5);
	m_feelers[2] = m_agent->GetPosition() + m_dWallDetectionFeelerLength / 2.0 * temp;
}