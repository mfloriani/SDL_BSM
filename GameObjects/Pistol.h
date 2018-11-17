//#pragma once
//#include "Weapon.h"
//
//class Pistol: public Weapon
//{
//public:
//	Pistol(
//		GameObject* owner,
//		World&		world
//	) :
//		Weapon(
//			owner,
//			world,
//			WeaponType::Pistol,
//			18,	//ammo
//			18,	//max ammo
//			3,	//rate of fire
//			15	//max range distance
//		)
//	{
//	}
//
//	virtual ~Pistol(){}
//
//	void ShootAt(math::Vector2D dir)
//	{
//		if (IsReadyForNextShot())
//		{
//			AddNewBulletToWorld(dir);
//			PrepareForNextShot();
//		}
//	}
//};