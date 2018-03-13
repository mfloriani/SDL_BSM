#include "LuaScript.h"

LuaScript::LuaScript()
{
	L = luaL_newstate();
	if (luaL_loadfile(L, "Params.lua") || lua_pcall(L, 0, 0, 0)) {
		std::cout << "Error: failed to load Params.lua " << std::endl;
		L = 0;
	}

	if (L) luaL_openlibs(L);
}

LuaScript* LuaScript::GetInstance()
{
	static LuaScript instance;
	return &instance;
}