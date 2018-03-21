#include "World.h"


World::World(): cellWidth_(0),
				cellHeight_(0),
				numCellsX_(0),
				numCellsY_(0)
{
	gameObjects_ = new std::vector<GameObject*>();
	collidableObjects_ = new std::vector<GameObject*>();
	projectiles_ = new std::vector<GameObject*>();
	playerSprite_ = new Texture();
	enemySprite_ = new Texture();
	spriteSheet_ = new Texture();
	player_ = nullptr;
	navGraph_ = new NavGraph();	
}

World::~World()
{
	delete navGraph_;
	delete player_;

	playerSprite_->Free();
	enemySprite_->Free();
	spriteSheet_->Free();

	std::vector<GameObject*>::iterator it;

	if (gameObjects_->size() > 0)
	{
		for (it = gameObjects_->begin(); it != gameObjects_->end(); ++it)
		{
			if ((*it) != NULL) delete (*it);
		}
	}
	delete gameObjects_;

	if (projectiles_->size() > 0)
	{
		for (it = projectiles_->begin(); it != projectiles_->end(); ++it)
		{
			if ((*it) != NULL) delete (*it);
		}
	}
	delete projectiles_;

	for (int i=0; i < TILES_MAP_COUNT; i++)
	{
		delete tiles_[i];
	}

	for (int i=0; i < TILES_SPRITESHEET; i++)
	{
		delete clips_[i];
	}

	std::vector<Wall*>::const_iterator itWall;
	if (walls_.size() > 0)
	{
		for (itWall = walls_.begin(); itWall != walls_.end(); ++itWall)
		{
			delete (*itWall);
		}
	}
}

bool World::Initialize()
{
	if (!LoadAssets()) return false;
	
	if(!LoadTiledScene()) return false;

	return true;
}

bool World::LoadAssets()
{
	if (!spriteSheet_->LoadFromFile(Game->GetRenderer(), params->Get<std::string>("spritesheet"))) return false;

	if (!playerSprite_->LoadFromFile(Game->GetRenderer(), params->Get<std::string>("player1_sprite"))) return false;

	if (!enemySprite_->LoadFromFile(Game->GetRenderer(), params->Get<std::string>("enemy_sprite"))) return false;

	SetSpriteSheetClips();

	return true;
}

bool World::LoadTiledScene()
{
	std::vector<TiledEnemy>		enemies;
	std::vector<TiledPlayer>	players;
	std::vector<int>			tiles;

	tiled->LoadMap(
		params->Get<std::string>("tiled_firstmap"),
		walls_,
		routes_,
		enemies,
		players,
		tiles
	);

	CreateNavGraph();

	int x = 0;
	int y = 0;
	int id = 0;

	for (int t = 0; t < tiles.size(); t++)
	{
		int tileType = tiles[t];

		if (tileType == TILE_NULL || tileType == TILE_EMPTY || tileType == TILE_WALL || tileType == TILE_ENEMY || tileType == TILE_PLAYER1 || tileType == TILE_PLAYER2)
		{
			bool isCollidable = false;
			if (tileType == TILE_WALL)
			{
				isCollidable = true;
				UpdateNavGraph(t, false);
			}

			tiles_[t] = new Tile(new SDL_Rect{ x, y, TILE_WIDTH, TILE_HEIGHT }, tileType, id++, spriteSheet_, clips_[tileType], isCollidable);
		}

		x += TILE_WIDTH;
		if (x >= SCREEN_WIDTH)
		{
			x = 0;
			y += TILE_HEIGHT;
		}
	}

	for (TiledEnemy e : enemies)
	{
		AddNewEnemy(math::Vector2D(e.x, e.y), e.route);
	}

	for (TiledPlayer p : players)
	{
		AddNewPlayer(math::Vector2D(p.x, p.y));
	}

	return true;
}


void World::AddNewEnemy(math::Vector2D pos, int route)
{
	Enemy* en = new Enemy(this, pos, enemySprite_, route);
	gameObjects_->push_back(en);
	GoManager->AddGameObject(en);
	collidableObjects_->push_back(en);
}

void World::AddNewPlayer(math::Vector2D pos)
{
	player_ = new Player(this, pos, playerSprite_);
	collidableObjects_->push_back(player_);
	GoManager->AddGameObject(player_);
}

void World::AddNewBullet(int id, math::Vector2D pos, math::Vector2D	dir)
{
	Bullet* bullet = new Bullet(this, id, pos, dir);
	projectiles_->push_back(bullet);
}

void World::HandleInput(SDL_Event* evt)
{
	if (evt->type == SDL_KEYDOWN)
	{
		switch (evt->key.keysym.sym)
		{
		case SDLK_F1:
			debugOn_ = !debugOn_;
			break;
		case SDLK_F2:
			debugGraphOn_ = !debugGraphOn_;
			break;
		}
	}
	player_->HandleInput(evt);
}

void World::Update(float secs)
{
	std::vector<GameObject*>::iterator projIT;
	if (projectiles_->size() > 0)
	{
		for (projIT = projectiles_->begin(); projIT != projectiles_->end(); ++projIT)
		{
			if ((*projIT) != NULL && (*projIT)->IsActive())
			{
				(*projIT)->Update(secs);
			}
		}
	}

	player_->Update(secs);

	std::vector<GameObject*>::iterator go = gameObjects_->begin();
	if (gameObjects_->size() > 0)
	{
		for (go; go != gameObjects_->end(); go++)
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

	if (IsDebugOn())
	{
		std::vector<Wall*>::const_iterator itWall;
		if (walls_.size() > 0)
		{
			for (itWall = walls_.begin(); itWall != walls_.end(); ++itWall)
			{
				SDL_SetRenderDrawColor(Game->GetRenderer(), 0, 0, 255, SDL_ALPHA_OPAQUE);
				SDL_RenderDrawLine(
					Game->GetRenderer(),
					(*itWall)->GetFrom().x,
					(*itWall)->GetFrom().y,
					(*itWall)->GetTo().x,
					(*itWall)->GetTo().y
				);
			}
		}
		if(debugGraphOn_) navGraph_->Draw(Game->GetRenderer());
	}

	player_->Draw();

	std::vector<GameObject*>::iterator go;
	if (gameObjects_->size() > 0)
	{
		for (go = gameObjects_->begin(); go != gameObjects_->end(); ++go)
		{
			if ((*go) != NULL && (*go)->IsActive())
			{
				(*go)->Draw();
			}
		}
	}

	std::vector<GameObject*>::iterator projIT;
	if (projectiles_->size() > 0)
	{
		for (projIT = projectiles_->begin(); projIT != projectiles_->end(); ++projIT)
		{
			if ((*projIT) != NULL && (*projIT)->IsActive())
			{
				(*projIT)->Draw();
			}
		}
	}
}

void World::SetSpriteSheetClips()
{
	clips_[TILE_NULL] = new SDL_Rect{ 0, 0, TILE_WIDTH, TILE_HEIGHT };
	clips_[TILE_EMPTY] = new SDL_Rect{ 0, 0, TILE_WIDTH, TILE_HEIGHT };
	clips_[TILE_WALL] = new SDL_Rect{ TILE_WIDTH , 0, TILE_WIDTH, TILE_HEIGHT };
	clips_[TILE_ENEMY] = new SDL_Rect{ TILE_WIDTH * 2, 0, TILE_WIDTH, TILE_HEIGHT };
	clips_[TILE_PLAYER1] = new SDL_Rect{ TILE_WIDTH * 3, 0, TILE_WIDTH, TILE_HEIGHT };
	clips_[TILE_PLAYER2] = new SDL_Rect{ TILE_WIDTH * 4, 0, TILE_WIDTH, TILE_HEIGHT };
}

void World::CreateNavGraph()
{
	numCellsX_ = TILES_MAP_X;
	numCellsY_ = TILES_MAP_Y;
	cellWidth_ = TILE_WIDTH;
	cellHeight_ = TILE_HEIGHT;
	
	CreateGrid();
}

void World::UpdateNavGraph(int cell, bool available)
{
	if (!available) navGraph_->RemoveNode(cell);
}

void World::CreateGrid()
{
	double midX = cellWidth_ / 2;
	double midY = cellHeight_ / 2;

	for (int row = 0; row < numCellsY_; ++row)
	{
		for (int col = 0; col < numCellsX_; ++col)
		{
			navGraph_->AddNode(
				NavGraphNode<>(navGraph_->GetNextFreeNodeIndex(),
					math::Vector2D(midX + (col * cellWidth_),
						midY + (row * cellHeight_))
					));
		}
	}

	for (int row = 0; row < numCellsY_; ++row)
	{
		for (int col = 0; col < numCellsX_; ++col)
		{
			LinkingEdges(row, col);	//add the edges...
		}
	}
}

//check if the node is inside the grid
bool World::IsValidNeighbour(int x, int y)
{
	return !((x < 0) || (x >= numCellsX_) || (y < 0) || (y >= numCellsY_));
}

//create the edges linking the nodes by NW, N, NE, W, E, SW, S, SE
void World::LinkingEdges(int row, int col)
{
	for (int i = -1; i < 2; ++i)		//goes through the Y
	{
		for (int j = -1; j < 2; ++j)	//goes through the X
		{
			int nodeX = col + j;
			int nodeY = row + i;

			if ((i == 0) && (j == 0)) continue; //skip if equal to the current node

			if (IsValidNeighbour(nodeX, nodeY))	//check if the node is inside the grid
			{
				int curNode = row * numCellsX_ + col;
				int neighbourNode = nodeY * numCellsX_ + nodeX;

				math::Vector2D nodePos = navGraph_->GetNode(curNode).GetPosition();
				math::Vector2D neighbourPos = navGraph_->GetNode(neighbourNode).GetPosition();

				double dist = math::distance(neighbourPos, nodePos);

				NavGraphEdge edge(curNode, neighbourNode, dist);
				navGraph_->AddEdge(edge);

				NavGraphEdge inverseEdge(neighbourNode, curNode, dist);
				navGraph_->AddEdge(inverseEdge);
			}
		}
	}
}

bool World::HasLOS(math::Vector2D origin, math::Vector2D target)const
{
	return !DoWallsObstructLineSegment(origin, target, walls_);
}

bool World::HasFOV(math::Vector2D origin, math::Vector2D direction, math::Vector2D target, double fov)const
{
	if (HasOriginFOVonTarget(origin, direction, target, fov))
	{
		if (HasLOS(origin, target))
		{
			return true;
		}
	}
	return false;
}