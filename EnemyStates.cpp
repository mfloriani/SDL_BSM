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
	if (e->GetFSM()->IsInState(*EnemyDying::GetInstance()) || e->GetFSM()->IsInState(*EnemyDead::GetInstance()))
	{
		return;
	}

	//std::cout << "Execute global "<< e->GetId()<< std::endl;
	if (e->SeeingPlayer())
	{
		//Msger->SendMsg(e->GetId(), e->GetId(), 0, MessageType::Msg_PlayerSpotted, NULL);
		e->SetPlayerAsTarget();
	}
	else if(e->HasTarget())
	{
		//Msger->SendMsg(e->GetId(), e->GetId(), 0, MessageType::Msg_TargetLost, NULL);
		e->TargetLost();
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

	case Msg_PlayerSpotted:
		
		

		/*if(e->IsCloseToAttack())
			e->GetFSM()->ChangeState(EnemyAttack::GetInstance());
		else
			e->GetFSM()->ChangeState(EnemyChase::GetInstance());*/

		break;

	case Msg_TargetLost:

		
		//e->GetFSM()->ChangeState(EnemyIdle::GetInstance());

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

	Msger->SendMsg(e->GetId(), e->GetId(), 3.0f, MessageType::Msg_Patrol, NULL);
	
}

void EnemyIdle::Execute(Enemy* e)
{
	//std::cout << "Execute idle " << e->GetId() << std::endl;
	
	if (e->HasTarget())
	{
		if (e->IsCloseToAttack())
		{
			e->GetFSM()->ChangeState(EnemyAttack::GetInstance());
		}
		else
		{
			e->GetFSM()->ChangeState(EnemyChase::GetInstance());
		}
	}
}

void EnemyIdle::Exit(Enemy* e)
{
	std::cout << "Exit idle " << e->GetId() << std::endl;
}

bool EnemyIdle::OnMessage(Enemy* enemy, const Message& msg)
{
	switch (msg.Msg)
	{
	case Msg_Patrol:

		enemy->GetFSM()->ChangeState(EnemyPatrol::GetInstance());
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
	e->DoPatrolling();

	//std::cout << "Execute Patrol " << e->GetId() << std::endl;
	if (e->HasTarget())
	{
		if (e->IsCloseToAttack())
		{
			e->GetFSM()->ChangeState(EnemyAttack::GetInstance());
		}
		else
		{
			e->GetFSM()->ChangeState(EnemyChase::GetInstance());
		}
	}
}

void EnemyPatrol::Exit(Enemy* e)
{
	std::cout << "Exit Patrol " << e->GetId() << std::endl;
}

bool EnemyPatrol::OnMessage(Enemy* enemy, const Message& msg)
{
	switch (msg.Msg)
	{
	

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
	
	e->ChaseTarget();
}

void EnemyChase::Execute(Enemy* e)
{
	//std::cout << "Execute Chase " << e->GetId() << std::endl;
	if (e->IsCloseToAttack())
	{
		e->GetFSM()->ChangeState(EnemyAttack::GetInstance());
		return;
	}
	
	if (e->GetSteering()->IsPathEnded())
	{
		e->GetFSM()->ChangeState(EnemyIdle::GetInstance());
	}
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

		//enemy->GetFSM()->ChangeState(EnemyIdle::GetInstance());
		break;

	case Msg_TargetLost:

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
	e->ShootAtTarget();

	if (e->HasTarget())
	{
		if (!e->IsCloseToAttack())
		{
			e->GetFSM()->ChangeState(EnemyChase::GetInstance());
		}
	}
	else
	{
		//e->GetFSM()->ChangeState(EnemyIdle::GetInstance());
		e->GetFSM()->ChangeState(EnemyChase::GetInstance());
	}
	//std::cout << "Execute Attack " << e->GetId() << std::endl;
}

void EnemyAttack::Exit(Enemy* e)
{
	std::cout << "Exit Attack " << e->GetId() << std::endl;
	e->SetLastTargetPosAsTarget();
}

bool EnemyAttack::OnMessage(Enemy* enemy, const Message& msg)
{
	switch (msg.Msg)
	{
	
	

	default:
		break;
	}
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
	Msger->SendMsg(e->GetId(), e->GetId(), 1.0f, Msg_Die, NULL);
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
	case Msg_Die:
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
	switch (msg.Msg)
	{
	case Msg_Blank:
		//enemy->GetFSM()->ChangeState(EnemyIdle::GetInstance());
		break;

	default:
		break;
	}
	return false;
}
