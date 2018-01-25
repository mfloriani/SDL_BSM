#pragma once
#include "GameObject.h"
class Enemy : public GameObject
{
public:
	Enemy(Vector2D pos, Texture* sprite);
	virtual ~Enemy();
	virtual void Update(float secs, Tile *tileMap[], vector<GameObject*> *gameObjects);
	void TakeDamage(int damage);
};

