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
	rateOfFire_(script->Get<float>("player_rateoffire")),
	lastShot_(0),
	inputForce_(script->Get<float>("plyer_inputforce"))
{
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
	Rotate();
	
	if (m_fireButtonPressed && lastShot_ >= rateOfFire_)
	{
		m_fireButtonPressed = false;
		lastShot_ = 0;
		world_->AddNewBullet(GetId(), position_, direction_);
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
	nextPosCollider.w = sprite_->GetWidth();
	nextPosCollider.h = sprite_->GetHeight();

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
		UpdateBoxCollider();
	}
	
}

bool Player::HandleMessage(const Message& msg)
{
	std::cout << "I got a message: " << msg.Msg << std::endl;
	switch (msg.Msg)
	{
	case MessageType::Msg_BulletHit:
		SetActive(false);
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
}
