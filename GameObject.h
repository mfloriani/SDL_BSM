#pragma once
#include "Texture.h"
#include "Vector2D.h"
#include <string>
#include "Config.h"
#include "Tile.h"
#include <vector>

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
	float m_velocity;
	bool  m_active;
	//float m_maxVelocity;
	//string m_tag;
	//int m_id;

protected:
	virtual Vector2D CalcForces();
	void UpdateCollider();
	void UpdateDirection();


public:
	GameObject(Vector2D pos, float vel, Texture *sprite);
	virtual ~GameObject();
	virtual void Update(float secs, Tile *tileMap[], vector<GameObject*> *gameObjects);
	virtual void Draw(float secs);
	SDL_Rect* GetCollider();
	bool IsActive();

	//Vector2D GetPosition();
	//Vector2D GetDirection();
	//string GetTag();
	//int GetId();

};
