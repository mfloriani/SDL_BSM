#include "LuaParameters.h"

LuaParameters::LuaParameters() : LuaScript("Scripting/Params.lua")
{
	
}

LuaParameters* LuaParameters::GetInstance()
{
	static LuaParameters instance;
	return &instance;
}