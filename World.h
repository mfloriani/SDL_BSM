#pragma once
#include <vector>
#include "Tile.h"
#include "Player.h"
#include "Config.h"
#include "Texture.h"
#include "Pathfinder.h"

class Player;

class World
{
private:
	std::vector<GameObject*>*	objects_;
	Tile*						tiles_[TILES_MAP_COUNT];
	Player*						player_;
	Texture*					playerSprite_;
	Texture*					bulletSprite_;
	Texture*					enemySprite_;
	Texture*					spriteSheet_;
	SDL_Rect*					clips_[TILES_SPRITESHEET];
	std::vector<GameObject*>*	collidableObjects_;
	Pathfinder*					pathfinder_;

public:
	World();
	virtual ~World();

	bool Initialize();
	bool LoadAssets();
	bool LoadScene();
	void SetSpriteSheetClips();
	void AddNewEnemy(math::Vector2D pos);
	void AddNewPlayer(math::Vector2D pos);
	void AddNewBullet(GameObject* owner);
	void HandleInput(SDL_Event* evt);
	void Update(float secs);
	void Draw();

	Player*						GetPlayer()const{ return player_; }
	std::vector<GameObject*>*	GetGameObjects()const{ return objects_; }
	Tile*const*					GetTiles() const{ return tiles_; }
	std::vector<GameObject*>*	GetCollidableObjects()const{ return collidableObjects_; }
	

};

