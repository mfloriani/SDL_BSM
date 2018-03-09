#pragma once
#include <vector>
#include "Tile.h"
#include "Player.h"
#include "Config.h"
#include "Texture.h"
#include "Pathfinder.h"
#include <fstream>
#include "GameObjectManager.h"
#include "Enemy.h"
#include "Bullet.h"
#include "SDLGame.h"
#include "Wall.h"
#include "WorldFunctions.h"
#include <map>

class Player;

class World
{
private:
	typedef Graph<NavGraphNode<>, NavGraphEdge>	NavGraph;
	typedef std::list<math::Vector2D>			Route;
	typedef std::map<int, Route>				PatrolRoutes;
	

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

public:
	World();
	virtual ~World();

	bool Initialize();
	bool LoadAssets();
	bool LoadScene();
	void SetSpriteSheetClips();
	void AddNewEnemy(math::Vector2D pos, int route);
	void AddNewPlayer(math::Vector2D pos);
	void AddNewBullet(int id, math::Vector2D pos, math::Vector2D dir);
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
	Route						GetPatrolRoute(int route) { return routes_[route]; }
	const std::vector<Wall*>&	GetWalls()const{ return walls_; }

};

