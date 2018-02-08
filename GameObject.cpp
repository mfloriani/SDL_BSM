#include "GameObject.h"

int GameObject::m_nextId = 0;

GameObject::GameObject(Vector2D pos, Texture *sprite) : m_position(pos), m_sprite(sprite), m_mass(1), 
m_radianAngle(0.0f), m_degreeAngle(0.0f), m_velocity(Vector2D(0,0)), m_active(true), m_maxForce(2.0), m_maxVelocity(150.0)
{
	SetId();
	UpdateDirection();
	UpdateCollider();
}


GameObject::~GameObject()
{
}

void GameObject::SetId()
{
	m_id = ++m_nextId;
}

Vector2D GameObject::CalcForces()
{
	return Vector2D();
}

void GameObject::Update(float secs, Tile *tileMap[], vector<GameObject*> *gameObjects)
{
	if (!m_active) return;	//dont process this gameobject

	
	Vector2D forces = CalcForces();
	Vector2D accelSecs = (forces / m_mass);
	
	m_velocity += accelSecs * secs;
	if (m_velocity.size() > m_maxVelocity)
	{
		m_velocity.normalize();
		m_velocity *= m_maxVelocity;
	}

	Vector2D nextPosition = m_position + m_velocity * secs;	//projection of the future position
	
	SDL_Rect nextPosCollider;
	nextPosCollider.x = nextPosition.x;
	nextPosCollider.y = nextPosition.y;
	nextPosCollider.w = 16;
	nextPosCollider.h = 16;

	bool collided = false;

	if (nextPosCollider.x < 0 || (nextPosCollider.x + nextPosCollider.w) > SCREEN_WIDTH) collided = true;					//check limits of the screen on X

	if (!collided && (nextPosCollider.y < 0 || (nextPosCollider.y + nextPosCollider.h) > SCREEN_HEIGHT)) collided = true;	//check limits of the screen on Y

	if (!collided)
	{
		for (int i = 0; i < TILES_MAP_COUNT; i++)
		{
			if (tileMap[i]->isCollidable())
			{
				if (SDL_HasIntersection(&nextPosCollider, tileMap[i]->GetBox()))	//check collision on the tiles of the tilemap
				{
					collided = true;
					break;
				}
			}
		}
	}

	if (!collided)
	{
		m_position += m_velocity * secs;		//move the real position only when there is no collision detected
		UpdateCollider();
	}	
}

void GameObject::UpdateDirection()
{
	m_direction = Vector2D::newBySizeAngle(1, m_radianAngle).normalize();
}

void GameObject::Draw(float secs)
{
	if (!m_active) return;	//dont process this gameobject

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

