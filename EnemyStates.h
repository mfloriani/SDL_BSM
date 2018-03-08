#pragma once
#include "State.h"
#include <iostream>
#include "Messenger.h"
#include "Enemy.h"
#include "GameMessages.h"

class Enemy;

//all the states are a singleton

//GLOBAL

class EnemyGlobal : public State<Enemy>
{
private:
	EnemyGlobal(){}
	EnemyGlobal(const EnemyGlobal&);
	EnemyGlobal& operator=(const EnemyGlobal&);

public:
	static EnemyGlobal* GetInstance();
	virtual void Enter(Enemy* e);
	virtual void Execute(Enemy* e);
	virtual void Exit(Enemy* e);
	virtual bool OnMessage(Enemy* enemy, const Message& msg);
};

//IDLE

class EnemyIdle : public State<Enemy>
{
private:
	EnemyIdle(){}
	EnemyIdle(const EnemyIdle&);
	EnemyIdle& operator=(const EnemyIdle&);

public:
	static EnemyIdle* GetInstance();
	virtual void Enter(Enemy* e);
	virtual void Execute(Enemy* e);
	virtual void Exit(Enemy* e);
	virtual bool OnMessage(Enemy* enemy, const Message& msg);
};

//PATROL

class EnemyPatrol : public State<Enemy>
{
private:
	EnemyPatrol(){}
	EnemyPatrol(const EnemyPatrol&);
	EnemyPatrol& operator=(const EnemyPatrol&);	

public:
	static EnemyPatrol* GetInstance();
	virtual void Enter(Enemy* e);
	virtual void Execute(Enemy* e);
	virtual void Exit(Enemy* e);
	virtual bool OnMessage(Enemy* enemy, const Message& msg);
};

//CHASE

class EnemyChase : public State<Enemy>
{
private:
	EnemyChase(){}
	EnemyChase(const EnemyChase&);
	EnemyChase& operator=(const EnemyChase&);

public:
	static EnemyChase* GetInstance();
	virtual void Enter(Enemy* e);
	virtual void Execute(Enemy* e);
	virtual void Exit(Enemy* e);
	virtual bool OnMessage(Enemy* enemy, const Message& msg);
};

//ATTACK

class EnemyAttack : public State<Enemy>
{
private:
	EnemyAttack(){}
	EnemyAttack(const EnemyAttack&);
	EnemyAttack& operator=(const EnemyAttack&);

public:
	static EnemyAttack* GetInstance();
	virtual void Enter(Enemy* e);
	virtual void Execute(Enemy* e);
	virtual void Exit(Enemy* e);
	virtual bool OnMessage(Enemy* enemy, const Message& msg);
};

//DYING

class EnemyDying : public State<Enemy>
{
private:
	EnemyDying(){}
	EnemyDying(const EnemyDying&);
	EnemyDying& operator=(const EnemyDying&);

public:
	static EnemyDying* GetInstance();
	virtual void Enter(Enemy* e);
	virtual void Execute(Enemy* e);
	virtual void Exit(Enemy* e);
	virtual bool OnMessage(Enemy* enemy, const Message& msg);
};

//DEAD

class EnemyDead : public State<Enemy>
{
private:
	EnemyDead(){}
	EnemyDead(const EnemyDead&);
	EnemyDead& operator=(const EnemyDead&);

public:
	static EnemyDead* GetInstance();
	virtual void Enter(Enemy* e);
	virtual void Execute(Enemy* e);
	virtual void Exit(Enemy* e);
	virtual bool OnMessage(Enemy* enemy, const Message& msg);
};
