//TODO implement the weapon class
//#pragma once
//#include "WindowsClock.h"
//#include "GameObject.h"
//#include "World.h"
//
//
////class GameObject;
////class World;
//
//class Weapon
//{
//public:
//	enum WeaponType
//	{
//		Pistol = 1
//	};
//
//protected:
//	GameObject* owner_;	
//	World&		world_;
//	int			type_;
//	int			ammo_;
//	int			maxAmmo_;
//	double		rateOfFire_;
//	double		nextTimeReady_;
//	double		maxRangeDistance_;
//	
//	
//
//
//	bool IsReadyForNextShot()
//	{
//		//std::cout << "Ready? " << WinClock->GetCurTime() << " > " << nextTimeReady_ << std::endl;
//		if (WinClock->GetCurTime() > nextTimeReady_)
//		{
//			//std::cout << "Ready!!! " << std::endl;
//			return true;
//		}
//		//std::cout << "NOT Ready! " << std::endl;
//		return false;		
//	}
//
//	void PrepareForNextShot() 
//	{ 
//		//nextTimeReady_ = WinClock->GetCurTime() + 1.0 / rateOfFire_; 
//		//std::cout <<  "Next " << nextTimeReady_ << std::endl; 
//	}
//
//	void IncreaseAmmo(int ammo)
//	{
//		ammo_ += ammo;
//		if (ammo_ > maxAmmo_) ammo = maxAmmo_;
//	}
//
//	void ReduceAmmo(){ if (ammo_ > 0) --ammo_; }
//
//public:
//	Weapon(
//		GameObject* owner,
//		World&		world,
//		int			type,
//		int			ammo,
//		int			maxAmmo,
//		float		rateOfFire,
//		float		maxRangeDistance
//	) :
//		owner_(owner),
//		world_(world),
//		type_(type),
//		ammo_(ammo),
//		maxAmmo_(maxAmmo),
//		rateOfFire_(rateOfFire),
//		maxRangeDistance_(maxRangeDistance)
//	{
//		PrepareForNextShot();
//	}
//
//	virtual ~Weapon(){}
//	//virtual void ShootAt(math::Vector2D dir) = 0;
//	void ShootAt(math::Vector2D dir)
//	{
//		if (IsReadyForNextShot())
//		{
//			std::cout << "Shoooot" << std::endl;
//			world_.AddNewBullet(owner_->GetId(), owner_->GetPosition(), dir);
//			PrepareForNextShot();
//		}
//	}
//
//	int GetType()const { return type_; }
//	int GetCurrentAmmo()const { return ammo_; }
//
//	/*void AddNewBulletToWorld(math::Vector2D dir)
//	{
//		std::cout << "Shoooot" << std::endl;
//		world_.AddNewBullet(owner_->GetId(), owner_->GetPosition(), dir);
//	}*/
//
//};