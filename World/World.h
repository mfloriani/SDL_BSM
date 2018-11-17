#pragma once
#include <vector>
#include "Tile.h"
#include "../GameObjects/Player.h"
#include "../Utils/Config.h"
#include "../Utils/Texture.h"
#include "../AI/Pathfinding/Pathfinder.h"
#include <fstream>
#include "../GameObjects/GameObjectManager.h"
#include "../GameObjects/Enemy.h"
#include "../GameObjects/Bullet.h"
#include "../Game/SDLGame.h"
#include "Wall.h"
#include "WorldFunctions.h"
#include <map>
#include "../Scripting/LuaTiledImporter.h"
#include "../AI/Steering/PatrolRoute.h"
#include <SDL_mixer.h>

class Player;

class World
{
private:
	typedef Graph<NavGraphNode<>, NavGraphEdge>	NavGraph;

	enum MissionStatus
	{
		ongoing,
		success,
		failure
	};

private:
	std::vector<GameObject*>*	gameObjects_;
	std::vector<GameObject*>*	collidableObjects_;
	std::vector<GameObject*>*	projectiles_;
	Tile*						tiles_[TILES_MAP_COUNT];
	Player*						player_;
	Texture*					playerSprite_;
	Texture*					enemySprite_;
	Texture*					spriteSheet_;
	SDL_Rect*					clips_[TILES_SPRITESHEET];
	NavGraph*					navGraph_;
	double						cellWidth_;
	double						cellHeight_;
	int							numCellsX_;
	int							numCellsY_;
	std::vector<Wall*>			walls_;
	PatrolRoutes				routes_;
	bool						debugOn_;
	bool						debugGraphOn_;
	Mix_Music*					music_;
	Mix_Chunk*					shot_;
	std::list<Mix_Chunk*>		sfxList_;
	int 						enemiesCount_;
	int							missionStatus_;
	bool						playerAlive_;
	Texture*					missionSuccessTex_;
	Texture*					missionFailureTex_;
	TTF_Font*					arialFont_;

public:
	World();
	virtual ~World();

	bool Initialize();
	bool LoadAssets();
	bool LoadTiledScene();
	void SetSpriteSheetClips();
	void AddNewEnemy(math::Vector2D pos, int route);
	void AddNewPlayer(math::Vector2D pos);
	void AddNewBullet(int id, math::Vector2D pos, math::Vector2D dir);
	void RemoveEnemy();
	void RemovePlayer();
	void HandleInput(SDL_Event* evt);
	void Update(float secs);
	void Draw();
	void CreateNavGraph();
	void UpdateNavGraph(int cell, bool available);
	void CreateGrid();
	void LinkingEdges(int row, int col);
	bool IsValidNeighbour(int x, int y);
	bool HasLOS(math::Vector2D source, math::Vector2D target)const;
	bool HasFOV(math::Vector2D origin, math::Vector2D direction, math::Vector2D target, double fov)const;

	Player*						GetPlayer()const{ return player_; }
	std::vector<GameObject*>*	GetGameObjects()const{ return gameObjects_; }
	Tile*const*					GetTiles() const{ return tiles_; }
	std::vector<GameObject*>*	GetCollidableObjects()const{ return collidableObjects_; }
	NavGraph&					GetNavGraph()const { return *navGraph_; }
	Route						GetPatrolRoute(int route) { return routes_.patrolRoutes_[route]; }
	const std::vector<Wall*>&	GetWalls()const{ return walls_; }
	bool						IsDebugOn()const { return debugOn_; }
	void						PlayMusic();
	void						ProcessSFX();
	void						AddShotSFX();
	bool						IsPlayerAlive()const { return playerAlive_; };
	bool						HasEnemiesAlive()const { return enemiesCount_ > 0; };
};

