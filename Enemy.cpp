#include "Enemy.h"
#include "EnemyStates.h"

Enemy::Enemy(World*			world, 
			 math::Vector2D pos, 
			 Texture*		sprite) :	GameObject(pos), 
										sprite_(sprite),
										world_(world)
{
	steering_ = new SteeringBehaviors(this);
	
	fsm_ = new StateMachine<Enemy>(this);
	fsm_->SetInitialCurState(EnemyIdle::GetInstance());
	fsm_->SetGlobalState(EnemyGlobal::GetInstance());
		
	std::cout << "enemy " << GetId() << std::endl;

	maxVelocity_ = 50.0f;
	maxForce_ = 400.0f;

}

Enemy::~Enemy()
{
	delete fsm_;
	delete steering_;
}

void Enemy::Update(float secs)
{
	if (!IsActive()) return;	//dont process this gameobject if isnt active

	fsm_->Update();

	math::Vector2D steeringForce = steering_->Calculate();
	math::Vector2D accelSecs = (steeringForce / mass_);

	velocity_ += accelSecs * secs;
	if (velocity_.size() > maxVelocity_)
	{
		velocity_.normalize();
		velocity_ *= maxVelocity_;
	}

	position_ += velocity_ * secs;

	UpdateBoxCollider();
	Rotate();
}

void Enemy::Draw()
{
	if (!IsActive()) return;	//dont process this gameobject
	
	sprite_->Render(position_.x, position_.y, NULL, degreeAngle_, NULL, SDL_FLIP_NONE);
}

void Enemy::TakeDamage(int damage)
{
	//m_active = false;
}

bool Enemy::HandleMessage(const Message& msg)
{
	return fsm_->HandleMessage(msg);
}

void Enemy::Rotate()
{
	//if (velocity_ == direction_) return;

	math::Vector2D vel = velocity_;
	vel.normalize();

	math::Vector2D dir = direction_;
	float angle = dir.relativeAngleBetween(vel);
	direction_ = vel;

	if (angle != 0) angle *= -1;

	radianAngle_ += angle;
	degreeAngle_ += math::toDegrees(angle);
}
