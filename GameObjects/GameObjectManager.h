#pragma once
#include <map>
#include "GameObject.h"

#define GoManager GameObjectManager::GetInstance()

class GameObject;

class GameObjectManager
{
private:
	typedef std::map<int, GameObject*> GoMap;

	GoMap m_goMap;

private:
	GameObjectManager(){}
	GameObjectManager(const GameObjectManager&);
	GameObjectManager& operator=(const GameObjectManager&);

public:
	static GameObjectManager* GetInstance();
	void AddGameObject(GameObject* go);
	void RemoveGameObject(GameObject* go);
	GameObject* GetGoFromId(int id) const;

};

