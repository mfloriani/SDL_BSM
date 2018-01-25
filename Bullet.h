#pragma once
#include "GameObject.h"
#include "Enemy.h"
class Bullet :public GameObject
{
public:
	Bullet(Vector2D pos, Vector2D dir, Texture* sprite);
	virtual ~Bullet();
	virtual void Update(float secs, Tile *tileMap[], vector<GameObject*> *gameObjects);
};

