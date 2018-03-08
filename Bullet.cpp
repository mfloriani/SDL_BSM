#include "Bullet.h"

void Bullet::Update(float secs)
{
	if (!IsActive()) return;	//dont process this gameobject

	position_ += direction_ * 1000.0f * secs;
	//UpdateBoxCollider();

	boxCollider_.x = position_.x;
	boxCollider_.y = position_.y;
	boxCollider_.w = sprite_.w;
	boxCollider_.h = sprite_.h;

	sprite_.x = position_.x;
	sprite_.y = position_.y;

	HandleCollision();
}

void Bullet::HandleCollision()
{
	std::vector<GameObject*>::iterator go;
	for (go = world_->GetCollidableObjects()->begin(); go != world_->GetCollidableObjects()->end(); ++go)
	{
		if ((*go) != NULL && (*go)->IsActive() && (*go)->GetId() != ownerId_)
		{
			if (SDL_HasIntersection(&boxCollider_, &(*go)->GetBoxCollider()))
			{
				//std::cout << owner_->GetId() << " | " << (*go)->GetId() << std::endl;
				Msger->SendMsg(ownerId_, (*go)->GetId(), 0.0f, Msg_BulletHit, NULL);
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

	SDL_RenderDrawRect(Game->GetRenderer(), &sprite_);
}

bool Bullet::HandleMessage(const Message& msg)
{
	return false;
}
