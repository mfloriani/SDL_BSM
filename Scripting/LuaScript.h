#pragma once

#include <string>
#include <vector>
#include <iostream>

extern "C" {
	# include "lua.h"
	# include "lauxlib.h"
	# include "lualib.h"
}

#pragma comment(lib, "lua53.lib")

class LuaScript
{
protected:
	lua_State *L;

	LuaScript()
	{
		L = luaL_newstate();
		if (L) luaL_openlibs(L);
	}

	LuaScript(std::string filename)
	{
		L = luaL_newstate();
		if (L) luaL_openlibs(L);

		RunScript(filename);
	}

	bool RunScript(std::string filename)
	{
		if (luaL_loadfile(L, filename.c_str()) != LUA_OK)
		{
			const char* msg = lua_tostring(L, -1);
			lua_pop(L, -1);
			std::cout << "Error: failed to load " << filename << " -> " << msg << std::endl;
			L = 0;
			return false;
		}

		if (lua_pcall(L, 0, LUA_MULTRET, 0) != LUA_OK)
		{
			const char* msg = lua_tostring(L, -1);
			lua_pop(L, -1);
			std::cout << "Error: failed to call " << filename << " -> " << msg << std::endl;
			L = 0;
			return false;
		}

		return true;
	}

	// Generic get
	template<typename T>
	T lua_get(const std::string& variableName)
	{
		return 0;
	}

	template<typename T>
	T lua_getdefault()
	{
		return 0;
	}

public:
	virtual ~LuaScript() { if (L) lua_close(L); }
	
	template<typename T>
	T Get(const std::string& variableName) 
	{
		if (!L) 
		{
			//printError(variableName, "Script is not loaded");
			return lua_getdefault<T>();
		}

		T result;

		lua_getglobal(L, variableName.c_str());
		if (lua_isnil(L, -1)) {
			//printError(variableName, var + " is not defined");
			result = lua_getdefault<T>();
			return result;
		}
		result = lua_get<T>(variableName);
		
		int n = lua_gettop(L);
		lua_pop(L, n);

		return result;
	}




};

// Specializations

template <>
inline bool LuaScript::lua_get<bool>(const std::string& variableName) {
	return (bool)lua_toboolean(L, -1);
}

template <>
inline float LuaScript::lua_get<float>(const std::string& variableName) {
	if (!lua_isnumber(L, -1)) {
		//printError(variableName, "Not a number");
	}
	return (float)lua_tonumber(L, -1);
}

template <>
inline int LuaScript::lua_get<int>(const std::string& variableName) {
	if (!lua_isnumber(L, -1)) {
		//printError(variableName, "Not a number");
	}
	return (int)lua_tonumber(L, -1);
}

template <>
inline std::string LuaScript::lua_get<std::string>(const std::string& variableName) {
	std::string s = "null";
	if (lua_isstring(L, -1)) {
		s = std::string(lua_tostring(L, -1));
	}
	else {
		//printError(variableName, "Not a string");
	}
	return s;
}

template<>
inline std::string LuaScript::lua_getdefault<std::string>() {
	return "null";
}
