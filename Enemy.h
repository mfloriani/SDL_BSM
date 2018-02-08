#pragma once
#include "GameObject.h"
#include "StateMachine.h"
#include "EnemyStates.h"
#include "Messenger.h"

class Enemy : public GameObject
{
private:
	StateMachine<Enemy>* m_fsm;

public:
	Enemy(Vector2D pos, Texture* sprite);
	virtual ~Enemy();
	virtual void Update(float secs, Tile *tileMap[], vector<GameObject*> *gameObjects);
	void TakeDamage(int damage);
	virtual bool HandleMessage(const Message& msg);
	StateMachine<Enemy>* GetFSM(){ return m_fsm; };
};

