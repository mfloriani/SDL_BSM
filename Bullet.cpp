#include "Bullet.h"
#include "GameMessages.h"

Bullet::Bullet(Texture*			sprite, 
			   GameObject*		owner,
			   World*			world) : GameObject(owner->GetPosition()),
											owner_(owner),
											sprite_(sprite),
											world_(world)
{
	direction_ = owner->GetDirection();
}


Bullet::~Bullet()
{
}

void Bullet::Update(float secs)
{
	if (!IsActive()) return;	//dont process this gameobject

	position_ += direction_ * 1000.0f * secs;
	UpdateBoxCollider();

	HandleCollision();
}

void Bullet::HandleCollision()
{
	std::vector<GameObject*>::iterator go;
	for (go = world_->GetCollidableObjects()->begin(); go != world_->GetCollidableObjects()->end(); ++go)
	{
		if ((*go) != NULL && (*go)->IsActive() /*&& (*go)->GetId() != GetId()*/)
		{
			if (SDL_HasIntersection(&boxCollider_, &(*go)->GetBoxCollider()))
			{
				//std::cout << owner_->GetId() << " | " << (*go)->GetId() << std::endl;
				Msger->SendMsg(owner_->GetId(), (*go)->GetId(), 0.0f, Msg_BulletHit, NULL);
			}
		}
	}

	if (boxCollider_.x < 0 || (boxCollider_.x + boxCollider_.w) > SCREEN_WIDTH)		//check limits of the screen on X
	{
		SetActive(false);
		return;
	}
	if (boxCollider_.y < 0 || (boxCollider_.y + boxCollider_.h) > SCREEN_HEIGHT)	//check limits of the screen on Y
	{
		SetActive(false);
		return;
	}

	for (int i = 0; i < TILES_MAP_COUNT; i++)
	{
		if (world_->GetTiles()[i]->isCollidable())
		{
			if (SDL_HasIntersection(&boxCollider_, world_->GetTiles()[i]->GetBox()))	//check collision on the tiles of the tilemap
			{
				SetActive(false);
				break;
			}
		}
	}
	
}

void Bullet::Draw()
{
	if (!IsActive()) return;	//dont process this gameobject

	sprite_->Render(position_.x, position_.y, NULL, degreeAngle_, NULL, SDL_FLIP_NONE);
}


bool Bullet::HandleMessage(const Message& msg)
{
	return false;
}
