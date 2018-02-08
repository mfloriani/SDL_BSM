#pragma once
#include "Texture.h"
#include "Vector2D.h"
#include <string>
#include "Config.h"
#include "Tile.h"
#include <vector>
#include "Message.h"

using namespace math;
using namespace std;

class GameObject
{
private:
	static int m_nextId;

private:
	void SetId();

protected:
	Vector2D m_position;
	Vector2D m_momentum;
	Vector2D m_direction;
	Texture *m_sprite;
	SDL_Rect m_collider;

	float m_mass;
	float m_radianAngle;
	float m_degreeAngle;
	Vector2D m_velocity;
	bool  m_active;
	float m_maxVelocity;
	string m_tag;
	int m_id;
	double m_maxForce;

protected:
	virtual Vector2D CalcForces();
	void UpdateCollider();
	void UpdateDirection();


public:
	GameObject(Vector2D pos, Texture *sprite);
	virtual ~GameObject();
	
	virtual void Update(float secs, Tile *tileMap[], vector<GameObject*> *gameObjects);
	virtual void Draw(float secs);
	virtual bool HandleMessage(const Message& msg) = 0;

	const SDL_Rect* GetCollider() { return &m_collider;  };
	const bool IsActive(){ return m_active; };
	const Vector2D GetPosition(){ return m_position; };
	const Vector2D GetDirection(){ return m_direction; };
	const string GetTag() { return m_tag; };
	const int GetId(){ return m_id; };
	static void ResetNextId() { m_nextId = 0; }
	void Kill(){ m_active = false; }
	const double GetMaxForce(){ return m_maxForce; }
	const double GetMaxVelocity(){ return m_maxVelocity; }
	const Vector2D GetVelocity(){ return m_velocity; }

};
