#include "Enemy.h"


Enemy::Enemy(Vector2D pos, Texture* sprite) : GameObject(pos, 100.0f, sprite)
{
}


Enemy::~Enemy()
{
}

void Enemy::Update(float secs, Tile *tileMap[], vector<GameObject*> *gameObjects)
{
	if (!m_active) return;	//dont process this gameobject if isnt active

	m_position += m_direction * m_velocity * secs;
	UpdateCollider();
}

void Enemy::TakeDamage(int damage)
{
	m_active = false;
}
