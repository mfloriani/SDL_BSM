#pragma once
#include "LuaScript.h"

#define params LuaParameters::GetInstance()

class LuaParameters : public LuaScript
{
private:
	LuaParameters();
	LuaParameters(const LuaParameters&);
	LuaParameters& operator=(const LuaParameters&);
	
public:	
	virtual ~LuaParameters() {};
	static	LuaParameters* GetInstance();

};

