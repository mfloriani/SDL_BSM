#include "Enemy.h"


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

	pathfinder_ = new Pathfinder(world->GetNavGraph());
}

Enemy::~Enemy()
{
	delete fsm_;
	delete steering_;
	delete pathfinder_;
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

	pathfinder_->Draw(Game->GetRenderer());

	math::Vector2D pos = GetPosition();
	pos.x += TILE_WIDTH / 2;
	pos.y += TILE_HEIGHT / 2;
	math::Vector2D dirOffset = pos + (GetDirection() * 50);

	SDL_RenderDrawLine(Game->GetRenderer(), pos.x, pos.y, dirOffset.x, dirOffset.y);
	
	math::Vector2D perp = math::perp(GetDirection());
	math::Vector2D perpOffSet1 = pos + (perp * 100);
	math::Vector2D perpOffSet2 = pos - (perp * 100);

	SDL_RenderDrawLine(Game->GetRenderer(), perpOffSet2.x, perpOffSet2.y, perpOffSet1.x, perpOffSet1.y);
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
	if (velocity_.isZero()) return;

	math::Vector2D vel = velocity_;
	vel.normalize();

	math::Vector2D dir = direction_;
	float angle = dir.relativeAngleBetween(vel);
	direction_ = vel;

	if (angle != 0) angle *= -1;

	radianAngle_ += angle;
	degreeAngle_ += math::toDegrees(angle);
}

void Enemy::SetPlayerAsTarget()
{
	math::Vector2D pos = world_->GetPlayer()->GetPosition();
	steering_->SetTarget(pos);

	pathfinder_->ChangeSource(GetPosition());
	pathfinder_->ChangeTarget(pos);
}

void Enemy::ChaseTarget()
{
	pathfinder_->CreateAStarPath();

	steering_->SetNewPath(pathfinder_->GetPathWaypoints());
	steering_->SwitchOnOff(SteeringBehaviors::BehaviorsType::follow_path, true);
}

bool Enemy::SeeingPlayer()const
{
	if (world_->HasFOV(GetPosition(), GetDirection(), world_->GetPlayer()->GetPosition(), 135)) return true;
	
	return false;
}