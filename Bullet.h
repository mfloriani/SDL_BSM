#pragma once
#include "GameObject.h"
#include "Texture.h"
#include "Messenger.h"
#include "World.h"
#include "GameMessages.h"

class Bullet : public GameObject
{
private:
	int			ownerId_;
	World*		world_;
	SDL_Rect	sprite_;

public:
	Bullet(	
		World*			world, 
		int				id,
		math::Vector2D	pos,
		math::Vector2D	dir
	):
		GameObject(pos, dir),
		ownerId_(id),
		world_(world)
	{
		sprite_.x = pos.x;
		sprite_.y = pos.y;
		sprite_.w = 4;
		sprite_.h = 4;
	}

	virtual			~Bullet() {}
	void			Update(float secs);
	void			Draw();
	virtual bool	HandleMessage(const Message& msg);
	int				GetOwnerId()const{ return ownerId_; }
	void			HandleCollision();


};
