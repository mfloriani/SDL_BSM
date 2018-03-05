#pragma once
#include <string>
#include <vector>
#include <SDL.h>
#include "Vector2D.h"
#include "Message.h"
#include "Config.h"

class GameObject
{
private:
	int			id_;
	int			type_;
	bool		active_;
	std::string tag_;

private:
	int			GetNextId(){ static int nextId = 1; return nextId++; }

protected:
	math::Vector2D	position_;
	math::Vector2D	direction_;
	math::Vector2D	velocity_;
	float			mass_;
	float			radianAngle_;
	float			degreeAngle_;
	float			maxVelocity_;
	float			maxForce_;
	SDL_Rect		boxCollider_;
	

	GameObject(	math::Vector2D pos) :	id_(GetNextId()),
										position_(pos), 
										type_(0), 
										active_(true), 
										tag_(""), 
										mass_(1.0f),
										radianAngle_(0.0f),
										degreeAngle_(0.0f),
										velocity_(math::Vector2D(0,0)),
										maxForce_(2.0f),
										maxVelocity_(150.0f),
										direction_(math::Vector2D(1,0))
	{
		UpdateBoxCollider();
	}

	void UpdateBoxCollider()
	{
		boxCollider_.x = position_.x;
		boxCollider_.y = position_.y;
		boxCollider_.w = SPRITE_WIDTH;
		boxCollider_.h = SPRITE_HEIGHT;
	}

public:
	virtual		 ~GameObject(){}
	virtual void Update(float secs)=0;
	virtual void Draw()=0;
	virtual bool HandleMessage(const Message& msg) = 0;

	int				GetId() const{ return id_; }
	int				GetType() const{ return type_; }
	std::string		GetTag() const{ return tag_; }
	bool			IsActive() const{ return active_; }
	void			SetActive(bool active){ active_ = active; }
	math::Vector2D	GetPosition() const{ return position_; }
	math::Vector2D	GetDirection() const{ return direction_; }
	//math::Vector2D	SetDirection(math::Vector2D dir) { direction_ = dir; }
	//void			IncRadAngle(float rad){ radianAngle_ += rad; }
	//void			IncDegAngle(float deg){ degreeAngle_ += deg; }
	//float			GetDegAngle()const{ return degreeAngle_; }
	SDL_Rect		GetBoxCollider()const{ return boxCollider_; }
	float			GetMaxForce()const{ return maxForce_; }
	float			GetMaxVelocity()const{ return maxVelocity_; }
	math::Vector2D	GetVelocity()const { return velocity_; }

};
