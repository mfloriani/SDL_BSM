#pragma once
#include <SDL.h>
#include "GameObject.h"
#include "../Utils/Texture.h"
#include "../World/World.h"
#include "../Messaging/GameMessages.h"
#include "../Scripting/LuaParameters.h"

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

	int	spriteW_;
	int	spriteH_;
	math::Vector2D	gunPos_;

	virtual math::Vector2D CalcForces();
	void Rotate();
	void Shoot();

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
	void	Die();
	
};

