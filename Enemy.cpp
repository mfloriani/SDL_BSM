#include "Enemy.h"


Enemy::Enemy(
	World*			world, 
	math::Vector2D	pos, 
	Texture*		sprite,
	int				route
) :	GameObject(pos, math::Vector2D(1, 0)),
	sprite_(sprite),
	world_(world),
	patrolRouteId_(route),
	rateOfFire_(script->Get<float>("enemy_rateoffire")),
	nextShot_(0),
	fov_(script->Get<float>("enemy_fov")),
	attackDist_(script->Get<float>("enemy_attackdist"))
{
	steering_ = new SteeringBehaviors(this, world);
	//steering_->SwitchOnOff(SteeringBehaviors::BehaviorsType::wall_avoidance, true);
		
	std::cout << "enemy " << GetId() << std::endl;

	maxVelocity_ = script->Get<float>("enemy_maxvelocity");
	maxForce_ = script->Get<float>("enemy_maxforce");

	pathfinder_ = new Pathfinder(world->GetNavGraph());

	fsm_ = new StateMachine<Enemy>(this);
	fsm_->SetInitialCurState(EnemyPatrol::GetInstance());
	fsm_->SetGlobalState(EnemyGlobal::GetInstance());
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

	//if(velocity_.size() > 0.00001f)	
		RotateTo(velocity_);
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

void Enemy::RotateTo(math::Vector2D dir)
{
	//if (velocity_ == direction_) return;
	if (dir.isZero()) return;

	math::Vector2D vel = dir;
	vel.normalize();

	math::Vector2D curDir = direction_;
	float angle = curDir.relativeAngleBetween(vel);
	direction_ = vel;

	if (angle != 0) angle *= -1;

	radianAngle_ += angle;
	degreeAngle_ += math::toDegrees(angle);
}

void Enemy::StopMovement()
{
	steering_->SwitchOnOff(SteeringBehaviors::BehaviorsType::follow_path, false);
	velocity_ = math::Vector2D();
}

void Enemy::SetPlayerAsTarget()
{
	target_ = world_->GetPlayer();
	math::Vector2D pos = target_->GetPosition();
	lastTargetPos_ = pos;

	steering_->SetTarget(pos);

	pathfinder_->ChangeSource(GetPosition());
	pathfinder_->ChangeTarget(pos);
}

void Enemy::SetLastTargetPosAsTarget()
{
	steering_->SetTarget(lastTargetPos_);

	pathfinder_->ChangeSource(position_);
	pathfinder_->ChangeTarget(lastTargetPos_);
}

void Enemy::ChaseTarget()
{
	//if (!HasTarget()) return;

	pathfinder_->CreateAStarPath();

	steering_->SetNewPath(pathfinder_->GetPathWaypoints());
	steering_->SwitchOnOff(SteeringBehaviors::BehaviorsType::follow_path, true);
}

bool Enemy::SeeingPlayer()
{
	if (world_->HasFOV(GetPosition(), GetDirection(), world_->GetPlayer()->GetPosition(), fov_))
	{
		SetPlayerAsTarget();
		return true;
	}
	return false;
}

bool Enemy::IsCloseToAttack()
{
	if (!HasTarget()) return false;

	// std::cout << math::distanceSqr(target_->GetPosition(), GetPosition()) << std::endl;

	if (math::distanceSqr(target_->GetPosition(), GetPosition()) < attackDist_)
	{
		//std::cout << "Die MF " << std::endl;
		return true;
	}


	return false;
}

void Enemy::TargetLost()
{
	target_ = NULL;
}

void Enemy::ShootAtTarget()
{
	if (!HasTarget()) return;

	StopMovement();
	math::Vector2D dir = target_->GetPosition() - position_;
	RotateTo(dir);

	if (WinClock->GetCurTime() > nextShot_)
	{
		nextShot_ = WinClock->GetCurTime() + (1 / rateOfFire_);
		//std::cout << "Die MF " << std::endl;
		world_->AddNewBullet(GetId(), position_, direction_);
	}
}

bool Enemy::HasValidPatrolRoute()
{
	if (patrolRouteId_ != NULL) return true;

	return false;
}

void Enemy::DoPatrolling()
{
	if (!HasValidPatrolRoute()) return;

	if(patrolRoute_.size() == 0) 
		Msger->SendMsg(GetId(), GetId(), 0.0f, MessageType::Msg_PatrolEnded, NULL);

	if (!readyToPatrol_ && steering_->IsPathEnded())
	{
		readyToPatrol_ = true;
		steering_->SetNewPath(patrolRoute_);
	}
}

void Enemy::PreparePatrolRoute()
{
	if (!HasValidPatrolRoute()) return;
	
	patrolRoute_ = world_->GetPatrolRoute(patrolRouteId_);
	
	pathfinder_->ChangeSource(position_);
	pathfinder_->ChangeTarget(patrolRoute_.back());
	pathfinder_->CreateAStarPath();

	steering_->SetNewPath(pathfinder_->GetPathWaypoints());
	
	steering_->SwitchOnOff(SteeringBehaviors::BehaviorsType::follow_path, true);
	readyToPatrol_ = false;
}