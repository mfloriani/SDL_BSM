#pragma once
#include "LuaScript.h"
#include "../World/Wall.h"
#include "../AI/Steering/PatrolRoute.h"

#define tiled LuaTiledImporter::GetInstance()

struct TiledEnemy
{
	int x, y, route;
};

struct TiledPlayer
{
	int x, y;
};

struct TiledTile
{
	int id, x, y;
};

class LuaTiledImporter : public LuaScript
{	
private:
	LuaTiledImporter();
	LuaTiledImporter(const LuaTiledImporter&);
	LuaTiledImporter& operator=(const LuaTiledImporter&);

	void LoadWalls(std::vector<Wall*>& walls);
	void LoadRoutes(PatrolRoutes& routes);
	void LoadPlayers(std::vector<TiledPlayer>& players);
	void LoadEnemies(std::vector<TiledEnemy>& enemies);
	void LoadTiles(std::vector<int>& tiles);

public:
	virtual ~LuaTiledImporter() {};
	static LuaTiledImporter* GetInstance();

	void LoadMap(
		std::string filename, 
		std::vector<Wall*>& walls, 
		PatrolRoutes& routes,
		std::vector<TiledEnemy>& enemies,
		std::vector<TiledPlayer>& players,
		std::vector<int>& tiles
	);
	
	

};