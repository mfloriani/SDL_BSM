#pragma once
#include "GameObject.h"
#include <SDL.h>
#include <iostream>
#include <vector>
#include "Bullet.h"

class Player : public GameObject
{
private:
	virtual Vector2D CalcForces();

	bool m_leftButtonPressed;
	bool m_rightButtonPressed;
	bool m_upButtonPressed;
	bool m_downButtonPressed;
	bool m_fireButtonPressed;

	Texture* m_bulletSprite;

public:
	Player(Vector2D pos, Texture *sprite, Texture *bulletSprite);
	virtual ~Player();
	void HandleInput(SDL_Event *e);
	virtual void Update(float secs, Tile *tileMap[], vector<GameObject*> *gameObjects);
	virtual bool HandleMessage(const Message& msg);
	
};

