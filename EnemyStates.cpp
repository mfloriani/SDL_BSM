#include "EnemyStates.h"

/*-----------------------------------------------------------------------

                           GLOBAL

-----------------------------------------------------------------------*/

EnemyGlobal* EnemyGlobal::GetInstance()
{
	static EnemyGlobal instance;
	return &instance;
}

void EnemyGlobal::Enter(Enemy* e)
{
	std::cout << "Enter global " << e->GetId() << std::endl;
}

void EnemyGlobal::Execute(Enemy* e)
{
	//std::cout << "Execute global "<< e->GetId()<< std::endl;
	if (e->SeeingPlayer())
	{
		Msger->SendMsg(e->GetId(), e->GetId(), 0, MessageType::Msg_Chase, NULL);
	}
}

void EnemyGlobal::Exit(Enemy* e)
{
	std::cout << "Exit global " << e->GetId() << std::endl;
}

bool EnemyGlobal::OnMessage(Enemy* e, const Message& msg)
{
	switch (msg.Msg)
	{
	case Msg_BulletHit:
		if (e->GetFSM()->IsInState(*EnemyDying::GetInstance())) return false;

		std::cout << "Enemy " << e->GetId() << " hit " << std::endl;
		e->TakeDamage(100);
		e->GetFSM()->ChangeState(EnemyDying::GetInstance());
		break;

	case Msg_Chase:
		e->GetFSM()->ChangeState(EnemyChase::GetInstance());
		break;

	default:
		break;
	}
	return false;
}

/*-----------------------------------------------------------------------

                 IDLE

-----------------------------------------------------------------------*/

EnemyIdle* EnemyIdle::GetInstance()
{
	static EnemyIdle instance;
	return &instance;
}

void EnemyIdle::Enter(Enemy* e)
{
	std::cout << "Enter idle " << e->GetId() << std::endl;
	//Msger->SendMsg(e->GetId(), e->GetId(), 1.0f, MessageType::Msg_StartPatrolling, NULL);
	//Msger->SendMsg(e->GetId(), e->GetId(), 5.0f, MessageType::Msg_Chase, NULL);
}

void EnemyIdle::Execute(Enemy* e)
{
	//std::cout << "Execute idle " << e->GetId() << std::endl;
	
}

void EnemyIdle::Exit(Enemy* e)
{
	std::cout << "Exit idle " << e->GetId() << std::endl;
}

bool EnemyIdle::OnMessage(Enemy* enemy, const Message& msg)
{
	switch (msg.Msg)
	{
	case Msg_StartPatrolling:
		enemy->GetFSM()->ChangeState(EnemyPatrol::GetInstance());
		break;

	case Msg_Chase:
		enemy->GetFSM()->ChangeState(EnemyChase::GetInstance());
		break;

	default:
		break;
	}
	return false;
}

/*-----------------------------------------------------------------------

                     PATROL 

-----------------------------------------------------------------------*/

EnemyPatrol* EnemyPatrol::GetInstance()
{
	static EnemyPatrol instance;
	return &instance;
}

void EnemyPatrol::Enter(Enemy* e)
{
	std::cout << "Enter Patrol " << e->GetId() << std::endl;
	//Msger->SendMsg(e->GetId(), e->GetId(), 1.0f, MessageType::Msg_Chase, NULL);
}

void EnemyPatrol::Execute(Enemy* e)
{
	//std::cout << "Execute Patrol " << e->GetId() << std::endl;

}

void EnemyPatrol::Exit(Enemy* e)
{
	std::cout << "Exit Patrol " << e->GetId() << std::endl;
}

bool EnemyPatrol::OnMessage(Enemy* enemy, const Message& msg)
{
	switch (msg.Msg)
	{
	case Msg_Chase:
		enemy->GetFSM()->ChangeState(EnemyChase::GetInstance());
		break;

	default:
		break;
	}
	return false;
}

/*--------------------------------------------------------------------

                CHASE 

--------------------------------------------------------------------*/

EnemyChase* EnemyChase::GetInstance()
{
	static EnemyChase instance;
	return &instance;
}

void EnemyChase::Enter(Enemy* e)
{
	std::cout << "Enter Chase " << e->GetId() << std::endl;
	e->SetPlayerAsTarget();
	e->ChaseTarget();
	//e->GetSteering()->SwitchOnOff(SteeringBehaviors::seek, true);
	//e->GetSteering()->SwitchOnOff(SteeringBehaviors::arrive, true);
}

void EnemyChase::Execute(Enemy* e)
{
	//std::cout << "Execute Chase " << e->GetId() << std::endl;
	//if(e->GetSteering()->IsPathEnded())
	//	Msger->SendMsg(e->GetId(), e->GetId(), 0.0f, MessageType::Msg_ChaseEnded, NULL);

}

void EnemyChase::Exit(Enemy* e)
{
	std::cout << "Exit Chase " << e->GetId() << std::endl;
}

bool EnemyChase::OnMessage(Enemy* enemy, const Message& msg)
{
	switch (msg.Msg)
	{
	case Msg_ChaseEnded:
		enemy->GetFSM()->ChangeState(EnemyIdle::GetInstance());
		break;

	default:
		break;
	}
	return false;
}

/*--------------------------------------------------------------------

                   ATTACK 

--------------------------------------------------------------------*/

EnemyAttack* EnemyAttack::GetInstance()
{
	static EnemyAttack instance;
	return &instance;
}

void EnemyAttack::Enter(Enemy* e)
{
	std::cout << "Enter Attack " << e->GetId() << std::endl;
}

void EnemyAttack::Execute(Enemy* e)
{
	std::cout << "Execute Attack " << e->GetId() << std::endl;
}

void EnemyAttack::Exit(Enemy* e)
{
	std::cout << "Exit Attack " << e->GetId() << std::endl;
}

bool EnemyAttack::OnMessage(Enemy* enemy, const Message& msg)
{
	return false;
}

/*--------------------------------------------------------------------

                    DYING

--------------------------------------------------------------------*/

EnemyDying* EnemyDying::GetInstance()
{
	static EnemyDying instance;
	return &instance;
}

void EnemyDying::Enter(Enemy* e)
{
	std::cout << "Enter dying " << e->GetId() << std::endl;
	Msger->SendMsg(e->GetId(), e->GetId(), 1.0f, Msg_Dead, NULL);
}

void EnemyDying::Execute(Enemy* e)
{
	//std::cout << "Execute dying " << e->GetId() << std::endl;
}

void EnemyDying::Exit(Enemy* e)
{
	std::cout << "Exit dying " << e->GetId() << std::endl;
}

bool EnemyDying::OnMessage(Enemy* enemy, const Message& msg)
{
	switch (msg.Msg)
	{
	case Msg_Dead:
		enemy->GetFSM()->ChangeState(EnemyDead::GetInstance());
		break;
	default:
		break;
	}
	return false;
}

/*--------------------------------------------------------------------

           DEAD

--------------------------------------------------------------------*/

EnemyDead* EnemyDead::GetInstance()
{
	static EnemyDead instance;
	return &instance;
}

void EnemyDead::Enter(Enemy* e)
{
	std::cout << "Enter dead " << e->GetId() << std::endl;
	//e->Kill();
	e->SetActive(false);
}

void EnemyDead::Execute(Enemy* e)
{
	//std::cout << "Execute dead " << e->GetId() << std::endl;
}

void EnemyDead::Exit(Enemy* e)
{
	std::cout << "Exit dead " << e->GetId() << std::endl;
}

bool EnemyDead::OnMessage(Enemy* enemy, const Message& msg)
{
	return false;
}
