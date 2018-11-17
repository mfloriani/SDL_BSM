#include "Player.h"


Player::Player(	
	World*			world, 
	math::Vector2D	pos, 
	Texture*		sprite
):	
	GameObject(pos, math::Vector2D(1, 0)),
	sprite_(sprite),
	aim_(math::Vector2D(0,0)),
	world_(world),
	rateOfFire_(params->Get<float>("player_rateoffire")),
	lastShot_(0),
	inputForce_(params->Get<float>("plyer_inputforce"))
{
	spriteW_ = sprite->GetWidth();
	spriteH_ = sprite->GetHeight();

	gunPos_ = math::Vector2D(spriteW_/2, spriteH_/2);
	
	boxCollider_.x = position_.x;
	boxCollider_.y = position_.y;
	boxCollider_.w = spriteW_;
	boxCollider_.h = spriteH_;
}

Player::~Player()
{
	
}

void Player::HandleInput(SDL_Event *ev)
{
	m_fireButtonPressed = false;

	if (ev->type == SDL_KEYDOWN)
	{
		switch (ev->key.keysym.sym)
		{
		case SDLK_d:
			m_rightButtonPressed = true;
			break;
		case SDLK_a:
			m_leftButtonPressed = true;
			break;
		case SDLK_w:
			m_upButtonPressed = true;
			break;
		case SDLK_s:
			m_downButtonPressed = true;
			break;
		}
	}
	else if (ev->type == SDL_KEYUP)
	{
		switch (ev->key.keysym.sym)
		{
		case SDLK_d:
			m_rightButtonPressed = false;
			break;
		case SDLK_a:
			m_leftButtonPressed = false;
			break;
		case SDLK_w:
			m_upButtonPressed = false;
			break;
		case SDLK_s:
			m_downButtonPressed = false;
			break;
		}
	}
	else if (ev->type == SDL_MOUSEMOTION)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		aim_.x = x;
		aim_.y = y;
		//std::cout << "Mouse Position: " << x << " " << y << endl;
	}
	else if (ev->type == SDL_MOUSEBUTTONDOWN && ev->button.button == SDL_BUTTON_LEFT)
	{
		m_fireButtonPressed = true;
	}
	else if (ev->type == SDL_MOUSEBUTTONDOWN && ev->button.button == SDL_BUTTON_RIGHT)
	{
		
	}
	else if (ev->type == SDL_MOUSEBUTTONDOWN && ev->button.button == SDL_BUTTON_MIDDLE)
	{
		
	}
	else if (ev->type == SDL_MOUSEBUTTONUP && ev->button.button == SDL_BUTTON_LEFT)
	{
		m_fireButtonPressed = false;
	}

}

math::Vector2D Player::CalcForces()
{	
	math::Vector2D forces;
	if (m_rightButtonPressed)
		forces += math::Vector2D(inputForce_, 0);

	if (m_leftButtonPressed)
		forces += math::Vector2D(-inputForce_, 0);

	if (m_upButtonPressed)
		forces += math::Vector2D(0, -inputForce_);

	if (m_downButtonPressed)
		forces += math::Vector2D(0, inputForce_);

	if (forces.isZero()) velocity_ = math::Vector2D(0, 0); //stop the movement when player is not pressing movement button

	return forces;
}

void Player::Update(float secs)
{
	if (!IsActive()) return;

	Rotate();
	
	if (m_fireButtonPressed && lastShot_ >= rateOfFire_)
	{
		Shoot();
	}
	lastShot_ += secs;
	

	math::Vector2D forces = CalcForces();
	math::Vector2D accelSecs = (forces / mass_);

	velocity_ += accelSecs * secs;
	if (velocity_.size() > maxVelocity_)
	{
		velocity_.normalize();
		velocity_ *= maxVelocity_;
	}

	math::Vector2D nextPosition = position_ + velocity_ * secs;	//projection of the future position
	
	SDL_Rect nextPosCollider;
	nextPosCollider.x = nextPosition.x;
	nextPosCollider.y = nextPosition.y;
	nextPosCollider.w = spriteW_;
	nextPosCollider.h = spriteW_;

	bool collided = false;

	if (nextPosCollider.x < 0 || (nextPosCollider.x + nextPosCollider.w) > SCREEN_WIDTH) collided = true;					//check limits of the screen on X

	if (!collided && (nextPosCollider.y < 0 || (nextPosCollider.y + nextPosCollider.h) > SCREEN_HEIGHT)) collided = true;	//check limits of the screen on Y

	if (!collided)
	{
		for (int i = 0; i < TILES_MAP_COUNT; i++)
		{
			if (world_->GetTiles()[i]->isCollidable())
			{
				if (SDL_HasIntersection(&nextPosCollider, world_->GetTiles()[i]->GetBox()))	//check collision on the tiles of the tilemap
				{
					collided = true;
					break;
				}
			}
		}
	}

	if (!collided)
	{
		position_  += velocity_ * secs;		//move the real position only when there is no collision detected
		boxCollider_.x = position_.x;
		boxCollider_.y = position_.y;
	}
	
}

bool Player::HandleMessage(const Message& msg)
{
	std::cout << "I got a message: " << msg.Msg << std::endl;
	switch (msg.Msg)
	{
	case MessageType::Msg_BulletHit:
		Die();
		break;
	default:
		break;
	}
	return false;
}

void Player::Draw()
{
	if (!IsActive()) return;	//dont process this gameobject

	sprite_->Render(position_.x, position_.y, NULL, degreeAngle_, NULL, SDL_FLIP_NONE);

	if (world_->IsDebugOn())
	{
		SDL_SetRenderDrawColor(Game->GetRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(Game->GetRenderer(), &boxCollider_);
		
		math::Vector2D pos = position_;
		pos.x += spriteW_ / 2;
		pos.y += spriteH_ / 2;
		math::Vector2D dirOffset = pos + (direction_ * 50);

		SDL_RenderDrawLine(Game->GetRenderer(), pos.x, pos.y, dirOffset.x, dirOffset.y);

		math::Vector2D perp = math::perp(direction_);
		math::Vector2D perpOffSet1 = pos + (perp * 100);
		math::Vector2D perpOffSet2 = pos - (perp * 100);

		SDL_RenderDrawLine(Game->GetRenderer(), perpOffSet2.x, perpOffSet2.y, perpOffSet1.x, perpOffSet1.y);

		//math::Vector2D pos2 = position_;
		//pos2.x += spriteW_;
		//pos2.y += spriteH_;
		//math::Vector2D dirOffset2 = gunPos_;// pos2 + (gunPos_ * 2);
		//
		//SDL_RenderDrawLine(Game->GetRenderer(), pos2.x, pos2.y, dirOffset2.x, dirOffset2.y);
	}
	

}

void Player::Rotate()
{
	if (aim_ == direction_) return;

	math::Vector2D path = aim_ - position_;
	path.normalize();

	math::Vector2D dir = direction_;
	float angle = dir.relativeAngleBetween(path);
	direction_ = path;

	if (angle != 0) angle *= -1;

	radianAngle_ += angle;
	degreeAngle_ += math::toDegrees(angle);

	//std::cout << "CurGunPos= " << gunPos_ << " Angle= " << angle;

	//gunPos_.rotate(angle);
	//gunPos_ = gunPos_.newBySizeAngle(1, angle);

	//std::cout << " NewGunPos" << gunPos_ << std::endl;
}

void Player::Shoot()
{	
	m_fireButtonPressed = false;
	lastShot_ = 0;
	//math::Vector2D(spriteW_, spriteH_)
	//math::Vector2D pos = math::Vector2D(position_.x + spriteW_ / 2, position_.y + spriteH_ / 2) +gunPos_;
	world_->AddNewBullet(GetId(), (position_ + gunPos_), direction_);
}

void Player::Die()
{
	world_->RemovePlayer();
	SetActive(false);
}