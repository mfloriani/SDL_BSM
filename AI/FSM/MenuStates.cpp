#include "MenuStates.h"
/*-----------------------------------------------------------------------

GLOBAL

-----------------------------------------------------------------------*/

MenuGlobal* MenuGlobal::GetInstance()
{
	static MenuGlobal instance;
	return &instance;
}

void MenuGlobal::Enter(Menu* e)
{
	std::cout << "Enter global " <<  std::endl;
}

void MenuGlobal::Execute(Menu* e)
{
	
}

void MenuGlobal::Exit(Menu* e)
{
	std::cout << "Exit global " <<  std::endl;
}

bool MenuGlobal::OnMessage(Menu* e, const Message& msg)
{
	return false;
}

/*-----------------------------------------------------------------------

GAME

-----------------------------------------------------------------------*/

MenuGame* MenuGame::GetInstance()
{
	static MenuGame instance;
	return &instance;
}

void MenuGame::Enter(Menu* e)
{
	std::cout << "Enter game " << std::endl;
}

void MenuGame::Execute(Menu* e)
{

}

void MenuGame::Exit(Menu* e)
{
	std::cout << "Exit game " << std::endl;
}

bool MenuGame::OnMessage(Menu* e, const Message& msg)
{
	return false;
}

/*-----------------------------------------------------------------------

PAUSE

-----------------------------------------------------------------------*/

MenuPause* MenuPause::GetInstance()
{
	static MenuPause instance;
	return &instance;
}

void MenuPause::Enter(Menu* e)
{
	std::cout << "Enter pause " << std::endl;
}

void MenuPause::Execute(Menu* e)
{

}

void MenuPause::Exit(Menu* e)
{
	std::cout << "Exit pause " << std::endl;
}

bool MenuPause::OnMessage(Menu* e, const Message& msg)
{
	return false;
}