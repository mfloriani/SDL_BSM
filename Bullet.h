#pragma once
#include "GameObject.h"
#include "Texture.h"
#include "Messenger.h"
#include "World.h"


class Bullet : public GameObject
{
private:
	GameObject* owner_;
	Texture*	sprite_;
	World*		world_;

public:
	Bullet(	Texture*		sprite, 
			GameObject*		owner,
			World*			world);

	virtual			~Bullet();
	void			Update(float secs);
	void			Draw();
	virtual bool	HandleMessage(const Message& msg);
	GameObject*		GetOwner()const{ return owner_; }
	void			HandleCollision();


};
