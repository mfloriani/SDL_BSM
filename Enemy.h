#pragma once
#include "GameObject.h"
#include "StateMachine.h"
#include "Messenger.h"
#include "World.h"
#include "SteeringBehaviors.h"
#include "EnemyStates.h"

class Tile;
class World;
class GameObject;
class Enemy;

class Enemy : public GameObject
{
private:
	StateMachine<Enemy>*	fsm_;
	Texture*				sprite_;
	World*					world_;
	SteeringBehaviors*		steering_;
	Pathfinder*				pathfinder_;

public:
	Enemy(World*			world, 
		  math::Vector2D	pos, 
		  Texture*			sprite);

	virtual					~Enemy();
	void					Update(float secs);
	void					Draw();
	void					TakeDamage(int damage);
	virtual bool			HandleMessage(const Message& msg);
	StateMachine<Enemy>*	GetFSM()const{ return fsm_; }
	SteeringBehaviors*		GetSteering()const{ return steering_; }
	void					SetPlayerAsTarget();
	void					Rotate();
	void					ChaseTarget();
	bool					SeeingPlayer()const;
};

