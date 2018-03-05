#pragma once
#include "Vector2D.h"
#include "SDLGame.h"

class Wall
{
private:
	math::Vector2D from_;
	math::Vector2D to_;
	math::Vector2D normal_;

	void CalculateNormal()
	{
		math::Vector2D t = (to_ - from_).normalize();
		normal_.x = -t.y;
		normal_.y = t.x;
	}

public:
	Wall(){}
	Wall(math::Vector2D f, math::Vector2D t): from_(f), to_(t)
	{
		CalculateNormal();
	}

	/*void Draw()
	{
		SDL_RenderDrawLine(Game->GetRenderer(), from_.x, from_.y, to_.x, to_.y);
	}*/

	math::Vector2D	GetFrom()const { return from_; }
	void			SetFrom(math::Vector2D f) { from_ = f; }

	math::Vector2D	GetTo()const { return to_; }
	void			SetTo(math::Vector2D t) { to_ = t; }

	math::Vector2D	GetNormal()const { return normal_; }
	void			SetNormal(math::Vector2D n) { normal_ = n; }

	math::Vector2D	GetCenter()const { (from_ + to_) / 2.0; }
};