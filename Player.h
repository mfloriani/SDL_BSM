#pragma once
#include "GameObject.h"
#include <SDL.h>
#include <iostream>

class Player : public GameObject
{
private:
	virtual Vector2D CalcForces();

	bool m_leftButtonPressed;
	bool m_rightButtonPressed;
	bool m_upButtonPressed;
	bool m_downButtonPressed;

	int m_velocity;


public:
	Player(Vector2D pos, Texture *sprite);
	virtual ~Player();
	void HandleInput(SDL_Event *e);
	
};

