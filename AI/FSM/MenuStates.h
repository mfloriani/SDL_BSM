#ifndef _SDL_BSM_MENU_STATES_
#define _SDL_BSM_MENU_STATES_

#include "State.h"
#include <iostream>
#include "../../Messaging/Message.h"
#include "../../Game/Menu.h"
#include "../../Messaging/GameMessages.h"


//all the states are a singleton

//GLOBAL 

class MenuGlobal : public State<Menu>
{
private:
	MenuGlobal() {}
	MenuGlobal(const MenuGlobal&);
	MenuGlobal& operator=(const MenuGlobal&);

public:
	static MenuGlobal* GetInstance();
	virtual void Enter(Menu* e);
	virtual void Execute(Menu* e);
	virtual void Exit(Menu* e);
	virtual bool OnMessage(Menu* enemy, const Message& msg);
};



//GAME 

class MenuGame : public State<Menu>
{
private:
	MenuGame() {}
	MenuGame(const MenuGame&);
	MenuGame& operator=(const MenuGame&);

public:
	static MenuGame* GetInstance();
	virtual void Enter(Menu* e);
	virtual void Execute(Menu* e);
	virtual void Exit(Menu* e);
	virtual bool OnMessage(Menu* enemy, const Message& msg);
};

//PAUSE 

class MenuPause : public State<Menu>
{
private:
	MenuPause() {}
	MenuPause(const MenuPause&);
	MenuPause& operator=(const MenuPause&);

public:
	static MenuPause* GetInstance();
	virtual void Enter(Menu* e);
	virtual void Execute(Menu* e);
	virtual void Exit(Menu* e);
	virtual bool OnMessage(Menu* enemy, const Message& msg);
};


#endif // !_SDL_BSM_MENU_STATES_
