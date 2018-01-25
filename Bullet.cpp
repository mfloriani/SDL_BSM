#include "Bullet.h"


Bullet::Bullet(Vector2D pos, Vector2D dir, Texture* sprite) : GameObject(pos, 1000.0f, sprite)
{
	m_direction = dir;
}


Bullet::~Bullet()
{
}

void Bullet::Update(float secs, Tile *tileMap[], vector<GameObject*> *gameObjects)
{
	if (!m_active) return;	//dont process this gameobject

	m_position += m_direction * m_velocity * secs;
	UpdateCollider();

	vector<GameObject*>::iterator goIter;
	for (goIter = gameObjects->begin(); goIter != gameObjects->end(); ++goIter)
	{
		if ((*goIter) != NULL && (*goIter)->IsActive())
		{
			if (dynamic_cast<Enemy*>(*goIter) != nullptr)
			{
				if (SDL_HasIntersection(&m_collider, dynamic_cast<Enemy*>(*goIter)->GetCollider()))
				{
					dynamic_cast<Enemy*>(*goIter)->TakeDamage(100);
				}
			}
		}
	}

	if (m_collider.x < 0 || (m_collider.x + m_collider.w) > SCREEN_WIDTH)	//check limits of the screen on X
	{
		m_active = false;
		return;
	}
	if (m_collider.y < 0 || (m_collider.y + m_collider.h) > SCREEN_HEIGHT)	//check limits of the screen on Y
	{
		m_active = false;	
		return;
	}

	for (int i = 0; i < TILES_MAP_COUNT; i++)
	{
		if (tileMap[i]->isCollidable())
		{
			if (SDL_HasIntersection(&m_collider, tileMap[i]->GetBox()))		//check collision on the tiles of the tilemap
			{
				m_active = false;
				break;
			}
		}
	}
}
