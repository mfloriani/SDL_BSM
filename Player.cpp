#include "Player.h"


Player::Player(Vector2D pos, Texture *sprite, Texture *bulletSprite) : GameObject(pos, sprite), m_bulletSprite(bulletSprite)
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
		//std::cout << "Mouse Position: " << x << " " << y << endl;

		Vector2D mouseVec = Vector2D(x, y);
		Vector2D path = mouseVec - m_position;
		path.normalize();

		Vector2D dir = m_direction;
		//float dot = dir.dot(path);
		float angle = dir.relativeAngleBetween(path);
		m_direction = path;

		if (angle != 0) angle *= -1;

		m_radianAngle += angle;
		m_degreeAngle += toDegrees(angle);

	}
	else if (ev->type == SDL_MOUSEBUTTONDOWN && ev->button.button == SDL_BUTTON_LEFT)
	{
		/*int x, y;
		SDL_GetMouseState(&x, &y);
		
		Vector2D mouseVec = Vector2D(x, y);
		Vector2D path = mouseVec - m_position;
		path.normalize();*/

		m_fireButtonPressed = true;
	}
	else if (ev->type == SDL_MOUSEBUTTONDOWN && ev->button.button == SDL_BUTTON_RIGHT)
	{
		
	}
	else if (ev->type == SDL_MOUSEBUTTONDOWN && ev->button.button == SDL_BUTTON_MIDDLE)
	{
		
	}
	

}

Vector2D Player::CalcForces()
{
	Vector2D forces;
	if (m_rightButtonPressed)
	{
		forces += Vector2D(1000, 0);
	}

	if (m_leftButtonPressed)
	{
		forces += Vector2D(-1000, 0);
	}

	if (m_upButtonPressed)
	{
		forces += Vector2D(0, -1000);
	}

	if (m_downButtonPressed)
	{
		forces += Vector2D(0, 1000);
	}
	return forces;
}

void Player::Update(float secs, Tile *tileMap[], vector<GameObject*> *gameObjects)
{
	GameObject::Update(secs, tileMap, gameObjects);

	if (m_fireButtonPressed)
	{
		Bullet* bullet = new Bullet(m_position, m_direction, m_bulletSprite, this);
		gameObjects->push_back(bullet);
	}
}

bool Player::HandleMessage(const Message& msg)
{
	return false;
}
