#pragma once
#include <SDL.h>
#include "GameObject.h"
#include "Texture.h"
#include "World.h"
#include "GameMessages.h"
#include "LuaParameters.h"

class World;
class GameObject;

class Player : public GameObject
{
private:
	Texture*		sprite_;
	math::Vector2D	aim_;
	World*			world_;
	
	bool m_leftButtonPressed;
	bool m_rightButtonPressed;
	bool m_upButtonPressed;
	bool m_downButtonPressed;
	bool m_fireButtonPressed;

	float inputForce_;

	float rateOfFire_;
	float lastShot_;

	virtual math::Vector2D CalcForces();
	void Player::Rotate();

public:
	Player(World*			world, 
		   math::Vector2D	pos, 
		   Texture*			sprite);

	virtual	~Player();
	void	HandleInput(SDL_Event *e);
	void	Update(float secs);
	void	Draw();
	bool	HandleMessage(const Message& msg);
	World*	GetWorld()const{ return world_; }
	
};

