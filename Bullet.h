#pragma once
#include "GameObject.h"
#include "Enemy.h"
class Bullet : public GameObject
{
private:
	GameObject* m_owner;
public:
	Bullet(Vector2D pos, Vector2D dir, Texture* sprite, GameObject* owner);
	virtual ~Bullet();
	virtual void Update(float secs, Tile *tileMap[], vector<GameObject*> *gameObjects);
	virtual bool HandleMessage(const Message& msg);

	GameObject* GetOwner(){ return m_owner; };


};
