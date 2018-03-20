#pragma once
#include "GameObject.h"
#include "StateMachine.h"
#include "Messenger.h"
#include "World.h"
#include "SteeringBehaviors.h"
#include "EnemyStates.h"
#include "WindowsClock.h"

class Tile;
class World;
class GameObject;
class Enemy;

class Enemy : public GameObject
{
private:
	typedef std::list<math::Vector2D>	Route;

private:
	StateMachine<Enemy>*	fsm_;
	Texture*				sprite_;
	World*					world_;
	SteeringBehaviors*		steering_;
	Pathfinder*				pathfinder_;
	GameObject*				target_;
	math::Vector2D			lastTargetPos_;
	float					rateOfFire_;
	float					nextShot_ ;
	int						patrolRouteId_;
	Route					patrolRoute_;
	bool					readyToPatrol_;
	double					fov_;
	float					attackDist_;

public:
	Enemy(
		World*			world, 
		math::Vector2D	pos, 
		Texture*		sprite,
		int				route
	);

	virtual					~Enemy();
	void					Update(float secs);
	void					Draw();
	void					TakeDamage(int damage);
	virtual bool			HandleMessage(const Message& msg);
	StateMachine<Enemy>*	GetFSM()const{ return fsm_; }
	SteeringBehaviors*		GetSteering()const{ return steering_; }
	void					SetPlayerAsTarget();
	void					RotateTo(math::Vector2D dir);
	void					ChaseTarget();
	bool					SeeingPlayer();
	bool					IsCloseToAttack();
	void					TargetLost();
	bool					HasTarget()const { return !(target_ == NULL); }
	void					ShootAtTarget();
	void					DoPatrolling();
	void					StopMovement();
	math::Vector2D			GetLastTargetPos()const { return lastTargetPos_; }
	void					SetLastTargetPosAsTarget();
	void					SetPatrolRoute(int route) { patrolRouteId_ = route; }
	int						GetPatrolRoute()const { return patrolRouteId_; }
	void					PreparePatrolRoute();
	bool					HasValidPatrolRoute();
};

