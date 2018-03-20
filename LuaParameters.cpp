#include "LuaParameters.h"

LuaParameters::LuaParameters() : LuaScript("Params.lua")
{
	
}

LuaParameters* LuaParameters::GetInstance()
{
	static LuaParameters instance;
	return &instance;
}