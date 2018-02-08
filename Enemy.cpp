#include "Enemy.h"


Enemy::Enemy(Vector2D pos, Texture* sprite) : GameObject(pos, sprite)
{
	m_fsm = new StateMachine<Enemy>(this);
	m_fsm->SetInitialCurState(EnemyIdle::GetInstance());
	m_fsm->SetGlobalState(EnemyGlobal::GetInstance());
	cout << "enemy " << m_id << endl;
}


Enemy::~Enemy()
{
	delete m_fsm;
}

void Enemy::Update(float secs, Tile *tileMap[], vector<GameObject*> *gameObjects)
{
	if (!m_active) return;	//dont process this gameobject if isnt active

	m_fsm->Update();

	//m_position += m_direction * m_velocity * secs;
	UpdateCollider();
}

void Enemy::TakeDamage(int damage)
{
	//m_active = false;
}

bool Enemy::HandleMessage(const Message& msg)
{
	return m_fsm->HandleMessage(msg);
}
