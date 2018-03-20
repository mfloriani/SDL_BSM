#include "LuaTiledImporter.h"

LuaTiledImporter::LuaTiledImporter(): LuaScript()
{
	
}

LuaTiledImporter* LuaTiledImporter::GetInstance()
{
	static LuaTiledImporter instance;
	return &instance;
}

void LuaTiledImporter::LoadMap(
	std::string					filename, 
	std::vector<Wall*>&			walls, 
	PatrolRoutes&				routes,
	std::vector<TiledEnemy>&	enemies,
	std::vector<TiledPlayer>&	players,
	std::vector<int>&			tiles
)
{
	if (!RunScript(filename)) return;

	walls.clear();

	std::cout << "LoadMap" << std::endl;

	LoadWalls(walls);
	LoadRoutes(routes);
	LoadPlayers(players);
	LoadEnemies(enemies);
	LoadTiles(tiles);
}

void LuaTiledImporter::LoadWalls(std::vector<Wall*>& walls)
{
	//std::cout << "Walls:" << std::endl;
	lua_getfield(L, -1, "walls");
	lua_pushnil(L);

	while (lua_next(L, -2) != 0)
	{
		if (lua_istable(L, -1))
		{
			int wall[4];

			for (int i = 0; i < 4; ++i)
			{
				lua_pushnumber(L, i + 1);
				lua_gettable(L, -2);
				wall[i] = (int)lua_tonumber(L, -1);
				lua_pop(L, 1);
			}
			//std::cout << "[" << wall[0] << "," << wall[1] << "]" << "[" << wall[2] << "," << wall[3] << "]" << std::endl;
			walls.push_back(
				new Wall(math::Vector2D(wall[0], wall[1]), math::Vector2D(wall[2], wall[3]))
			);
		}
		lua_pop(L, 1);
	}
	lua_pop(L, 1);
}

void LuaTiledImporter::LoadRoutes(PatrolRoutes& routes)
{
	std::cout << "Routes:" << std::endl;
	lua_getfield(L, -1, "routes");
	lua_pushnil(L);

	while (lua_next(L, -2) != 0)
	{
		lua_getfield(L, -1, "id");
		int id = lua_tointeger(L, -1);
		std::cout << "The route ID: " << id << std::endl;
		lua_pop(L, 1);

		lua_getfield(L, -1, "points");
		lua_pushnil(L);

		Route r;

		while (lua_next(L, -2) != 0)
		{
			int point[2];
			for (int i = 0; i < 2; ++i)
			{
				lua_pushnumber(L, i + 1);
				lua_gettable(L, -2);
				point[i] = (int)lua_tonumber(L, -1);
				lua_pop(L, 1);
			}
			std::cout << "[" << point[0] << "," << point[1] << "]" << std::endl;
			
			r.waypoints_.push_back(
				math::Vector2D(point[0], point[1])
			);

			lua_pop(L, 1);
		}
		lua_pop(L, 1);
		lua_pop(L, 1);

		routes.patrolRoutes_.insert(std::pair<int, Route>(id, r));
	}
	lua_pop(L, 1);
}

void LuaTiledImporter::LoadPlayers(std::vector<TiledPlayer>& players)
{
	std::cout << "Players:" << std::endl;
	lua_getfield(L, -1, "players");
	lua_pushnil(L);

	while (lua_next(L, -2) != 0)
	{
		lua_getfield(L, -1, "id");
		std::string id = lua_tostring(L, -1);
		std::cout << "The player ID: " << id << std::endl;
		lua_pop(L, 1);

		lua_getfield(L, -1, "point");
		int point[2];

		for (int i = 0; i < 2; ++i)
		{
			lua_pushnumber(L, i + 1);
			lua_gettable(L, -2);
			point[i] = (int)lua_tonumber(L, -1);
			lua_pop(L, 1);
		}
		lua_pop(L, 1);
		lua_pop(L, 1);

		std::cout << "[" << point[0] << "," << point[1] << "]" << std::endl;
		
		TiledPlayer p;
		p.x = point[0];
		p.y = point[1];
		players.push_back(p);
	}
	lua_pop(L, 1);
}

void LuaTiledImporter::LoadEnemies(std::vector<TiledEnemy>& enemies)
{
	std::cout << "Enemies:" << std::endl;
	lua_getfield(L, -1, "enemies");
	lua_pushnil(L);

	while (lua_next(L, -2) != 0)
	{
		lua_getfield(L, -1, "routeId");
		int id = lua_tointeger(L, -1);
		std::cout << "The enemy RouteID: " << id << std::endl;
		lua_pop(L, 1);

		lua_getfield(L, -1, "point");
		int point[2];

		for (int i = 0; i < 2; ++i)
		{
			lua_pushnumber(L, i + 1);
			lua_gettable(L, -2);
			point[i] = (int)lua_tonumber(L, -1);
			lua_pop(L, 1);
		}
		lua_pop(L, 1);

		std::cout << "[" << point[0] << "," << point[1] << "]" << std::endl;
		TiledEnemy en; 
		en.x = point[0];
		en.y = point[1];
		en.route = id;
		enemies.push_back(en);

		lua_pop(L, 1);
	}
	lua_pop(L, 1);
}

void LuaTiledImporter::LoadTiles(std::vector<int>& tiles)
{
	std::cout << "Tiles:" << std::endl;
	lua_getfield(L, -1, "tiles");

	lua_getfield(L, -1, "numTiles");
	int numTiles = lua_tointeger(L, -1);
	std::cout << "Number of tiles: " << numTiles << std::endl;
	lua_pop(L, 1);

	lua_getfield(L, -1, "data");

	//int* tiles = new int[numTiles];
	for (int i = 0; i < numTiles; ++i)
	{
		lua_pushnumber(L, i + 1);
		lua_gettable(L, -2);
		tiles.push_back( (int)lua_tonumber(L, -1) );
		lua_pop(L, 1);
	}
	lua_pop(L, 1);

	/*int n = 0;
	for (int i = 0; i < numTiles; i++)
	{
		std::cout << tiles[i];

		n++;
		if (n == 50)
		{
			n = 0;
			std::cout << std::endl;
		}
	}*/
	//delete tiles;
	lua_pop(L, 1);
}