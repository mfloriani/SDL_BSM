#include "GameObjectManager.h"

GameObjectManager* GameObjectManager::GetInstance()
{
	static GameObjectManager instance;
	return &instance;
}

GameObject* GameObjectManager::GetGoFromId(int id) const
{
	GoMap::const_iterator go = m_goMap.find(id);
	if (go != m_goMap.end())
		return go->second;
	return NULL;
}

void GameObjectManager::AddGameObject(GameObject* go)
{
	m_goMap.insert(std::make_pair(go->GetId(), go));
}

void GameObjectManager::RemoveGameObject(GameObject* go)
{
	m_goMap.erase(m_goMap.find(go->GetId()));
}
