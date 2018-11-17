#pragma once
#include "GameObject.h"
#include "../Utils/Texture.h"
#include "../Messaging/Messenger.h"
#include "../World/World.h"
#include "../Messaging/GameMessages.h"

class Bullet : public GameObject
{
private:
	int			ownerId_;
	World*		world_;
	SDL_Rect	sprite_;
	float		vel_;

public:
	Bullet(	
		World*			world, 
		int				id,
		math::Vector2D	pos,
		math::Vector2D	dir
	):
		GameObject(pos, dir),
		ownerId_(id),
		world_(world),
		vel_(params->Get<float>("bullet_velocity"))
	{
		sprite_.x = pos.x;
		sprite_.y = pos.y;
		sprite_.w = params->Get<int>("bullet_width");
		sprite_.h = params->Get<int>("bullet_height");
	}

	virtual			~Bullet() {}
	void			Update(float secs);
	void			Draw();
	virtual bool	HandleMessage(const Message& msg);
	int				GetOwnerId()const{ return ownerId_; }
	void			HandleCollision();


};
