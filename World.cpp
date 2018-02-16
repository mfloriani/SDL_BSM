#include "World.h"
#include "SDLGame.h"
#include <fstream>
#include "GameObjectManager.h"
#include "Enemy.h"
#include "Bullet.h"


World::World()
{
	objects_ = new std::vector<GameObject*>();
	collidableObjects_ = new std::vector<GameObject*>();
	playerSprite_ = new Texture();
	enemySprite_ = new Texture();
	spriteSheet_ = new Texture();
	bulletSprite_ = new Texture();
	player_ = nullptr;
}

World::~World()
{
	delete player_;

	playerSprite_->Free();
	enemySprite_->Free();
	spriteSheet_->Free();
	bulletSprite_->Free();

	std::vector<GameObject*>::iterator it;
	if (objects_->size() > 0)
	{
		for (it = objects_->begin(); it != objects_->end(); ++it)
		{
			if ((*it) != NULL) delete (*it);
		}
	}
	delete objects_;

	for (int i=0; i < TILES_MAP_COUNT; i++)
	{
		delete tiles_[i];
	}

	for (int i=0; i < TILES_SPRITESHEET; i++)
	{
		delete clips_[i];
	}
}

bool World::Initialize()
{
	if (!LoadAssets()) return false;
	if (!LoadScene()) return false;
	return true;
}

bool World::LoadAssets()
{
	if (!spriteSheet_->LoadFromFile(Game->GetRenderer(), "spriteSheet16x16.png")) return false;

	if (!playerSprite_->LoadFromFile(Game->GetRenderer(), "player1.png")) return false;

	if (!bulletSprite_->LoadFromFile(Game->GetRenderer(), "bullet1.png")) return false;

	if (!enemySprite_->LoadFromFile(Game->GetRenderer(), "enemy.png")) return false;

	SetSpriteSheetClips();

	return true;
}

bool World::LoadScene()
{
	int x = 0;
	int y = 0;
	int id = 0;

	std::ifstream map("map_01.map");

	if (!map)
	{
		printf("Failed to load map file!\n");
		return false;
	}

	for (int t = 0; t < TILES_MAP_COUNT; t++)
	{
		int tileType = -1;
		map >> tileType;

		if (map.fail())
		{
			printf("Failed to load map!\n");
			return false;
		}

		if (tileType == TILE_NULL || tileType == TILE_WALL || tileType == TILE_ENEMY || tileType == TILE_PLAYER1 || tileType == TILE_PLAYER2)
		{
			if (tileType == TILE_PLAYER1)
			{
				tileType = TILE_NULL;
				AddNewPlayer(math::Vector2D(x, y));
			}
			else if (tileType == TILE_ENEMY)
			{
				tileType = TILE_NULL;
				AddNewEnemy(math::Vector2D(x, y));
			}

			bool isCollidable = false;
			if (tileType == TILE_WALL)
			{
				isCollidable = true;
			}

			tiles_[t] = new Tile(new SDL_Rect{ x, y, TILE_WIDTH, TILE_HEIGHT }, tileType, id++, spriteSheet_, clips_[tileType], isCollidable);
		}
		else
		{
			printf("Failed to load fileType! [%s]\n", tileType);
			map.close();
			return false;
		}

		x += TILE_WIDTH;
		if (x >= SCREEN_WIDTH)
		{
			x = 0;
			y += TILE_HEIGHT;
		}
	}

	map.close();
	return true;
}

void World::AddNewEnemy(math::Vector2D pos)
{
	Enemy* en = new Enemy(this, pos, enemySprite_);
	objects_->push_back(en);
	GoManager->AddGameObject(en);
	collidableObjects_->push_back(en);
}

void World::AddNewPlayer(math::Vector2D pos)
{
	player_ = new Player(this, pos, playerSprite_, bulletSprite_);
	GoManager->AddGameObject(player_);
}

void World::AddNewBullet(GameObject* owner)
{
	Bullet* bullet = new Bullet(bulletSprite_, owner, this);
	objects_->push_back(bullet);
}

void World::HandleInput(SDL_Event* evt)
{
	if (evt->type == SDL_KEYDOWN)
	{
		switch (evt->key.keysym.sym)
		{
		case SDLK_RETURN:
			

			break;
		case SDLK_SPACE:

			break;
		}
	}
	player_->HandleInput(evt);
}

void World::Update(float secs)
{
	player_->Update(secs);

	std::vector<GameObject*>::iterator go;
	if (objects_->size() > 0)
	{
		for (go = objects_->begin(); go != objects_->end(); ++go)
		{
			if ((*go) != NULL && (*go)->IsActive())
			{
				(*go)->Update(secs);
			}
		}
	}

	Msger->SendDelayedMsg(); //send all the queued messages ready to be sent
}

void World::Draw()
{
	for (int t = 0; t < TILES_MAP_COUNT; t++)
	{
		tiles_[t]->Draw();
	}

	player_->Draw();

	std::vector<GameObject*>::iterator go;
	if (objects_->size() > 0)
	{
		for (go = objects_->begin(); go != objects_->end(); ++go)
		{
			if ((*go) != NULL && (*go)->IsActive())
			{
				(*go)->Draw();
			}
		}
	}
}

void World::SetSpriteSheetClips()
{
	clips_[TILE_NULL] = new SDL_Rect{ 0, 0, TILE_WIDTH, TILE_HEIGHT };
	clips_[TILE_WALL] = new SDL_Rect{ TILE_WIDTH, 0, TILE_WIDTH, TILE_HEIGHT };
	clips_[TILE_ENEMY] = new SDL_Rect{ TILE_WIDTH * 2, 0, TILE_WIDTH, TILE_HEIGHT };
	clips_[TILE_PLAYER1] = new SDL_Rect{ TILE_WIDTH * 3, 0, TILE_WIDTH, TILE_HEIGHT };
	clips_[TILE_PLAYER2] = new SDL_Rect{ TILE_WIDTH * 4, 0, TILE_WIDTH, TILE_HEIGHT };
}
