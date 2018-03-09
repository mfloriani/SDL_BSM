#pragma once
#include "GameObject.h"
#include "Path.h"
//#include "World.h"
#include "Utils.h"

#define BEHAVIORS_COUNT 6

class World;
class Wall;

class SteeringBehaviors
{
public:
	enum BehaviorsType
	{
		none,
		seek,
		flee,
		arrive,
		follow_path,
		wall_avoidance
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
	Path*			m_path;
	World*			m_world;
	
	std::vector<math::Vector2D> m_feelers;	//a vertex buffer to contain the feelers rqd for wall avoidance  

	bool			AccumulateForce(math::Vector2D &curTotalForce, math::Vector2D forceToAdd);

	math::Vector2D	Seek(math::Vector2D targetPos);
	math::Vector2D	Arrive(math::Vector2D targetPos, Deceleration deceleration);
	math::Vector2D	FollowPath();
	math::Vector2D	WallAvoidance(const std::vector<Wall*> &walls);

public:
	SteeringBehaviors(GameObject* agent, World* world);
	virtual ~SteeringBehaviors();

	math::Vector2D	Calculate();
	void			SwitchOnOff(BehaviorsType behavior, bool onOff){ m_behaviorsStatus[behavior] = onOff; }
	bool			IsOn(BehaviorsType behavior){ return m_behaviorsStatus[behavior]; }
	void			SetTarget(math::Vector2D target){ m_target = target; }
	void			SetNewPath(WaypointsList& wp) { delete m_path;  m_path = new Path(wp); }
	bool			IsPathEnded()const { return m_path->IsPathEnded(); }
	void			CreateFeelers();

};

