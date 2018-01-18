#include "GameObject.h"


GameObject::GameObject(Vector2D pos, Texture *sprite) : m_position(pos), m_sprite(sprite), m_mass(1), m_radianAngle(0.0f), m_degreeAngle(0.0f)
{
	UpdateDirection();
	UpdateCollider();
}


GameObject::~GameObject()
{
}

Vector2D GameObject::CalcForces()
{
	return Vector2D();
}

void GameObject::Update(float secs, Tile *tileMap[])
{
	Vector2D curPos = m_position;

	Vector2D forces = CalcForces();
	Vector2D accelSecs = (forces / m_mass) * secs;
	
	curPos += accelSecs * secs;
	SDL_Rect boxCollider;
	boxCollider.x = curPos.x;
	boxCollider.y = curPos.y;
	boxCollider.w = 16;
	boxCollider.h = 16;

	bool collided = false;

	if (boxCollider.x < 0 || (boxCollider.x + boxCollider.w) > SCREEN_WIDTH) collided = true;					//check limits of the screen on X

	if (!collided && (boxCollider.y < 0 || (boxCollider.y + boxCollider.h) > SCREEN_HEIGHT)) collided = true;	//check limits of the screen on Y

	if (!collided)
	{
		for (int i = 0; i < TILES_MAP_COUNT; i++)
		{
			if (tileMap[i]->isCollidable())
			{
				if (SDL_HasIntersection(&boxCollider, tileMap[i]->GetBox()))									//check collision on the tiles of the tilemap
				{
					collided = true;
					break;
				}
			}
		}
	}

	if (!collided)
	{
		m_position += accelSecs * secs;
		UpdateCollider();
	}	
}

void GameObject::UpdateDirection()
{
	m_direction = Vector2D::newBySizeAngle(1, m_radianAngle).normalize();
}

void GameObject::Draw(float secs)
{
	m_sprite->Render(m_position.x, m_position.y, NULL, m_degreeAngle, NULL, SDL_FLIP_NONE);
}

void GameObject::UpdateCollider()
{
	if (m_sprite != NULL)
	{
		SDL_Rect boxCollider;
		boxCollider.x = m_position.x;
		boxCollider.y = m_position.y;
		boxCollider.w = m_sprite->GetWidth();
		boxCollider.h = m_sprite->GetHeight();
		m_collider = boxCollider;
	}
}

SDL_Rect* GameObject::GetCollider()
{
	return &m_collider;
}
