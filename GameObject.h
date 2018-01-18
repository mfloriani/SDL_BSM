#pragma once
#include "Texture.h"
#include "Vector2D.h"
#include <string>
#include "Config.h"
#include "Tile.h"

using namespace math;
using namespace std;

class GameObject
{
protected:
	Vector2D m_position;
	Vector2D m_momentum;
	Vector2D m_direction;
	Texture *m_sprite;
	SDL_Rect m_collider;

	float m_mass;
	float m_radianAngle;
	float m_degreeAngle;
	//float m_maxVelocity;
	//string m_tag;
	//int m_id;

protected:
	virtual Vector2D CalcForces();
	void UpdateCollider();
	void UpdateDirection();


public:
	GameObject(Vector2D pos, Texture *sprite);
	virtual ~GameObject();
	virtual void Update(float secs, Tile *tileMap[]);
	virtual void Draw(float secs);
	SDL_Rect* GetCollider();

	//Vector2D GetPosition();
	//Vector2D GetDirection();
	//string GetTag();
	//int GetId();

};
