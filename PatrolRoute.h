#pragma once
#include <list>
#include <map>
#include "Vector2D.h"

struct Route
{
	std::list<math::Vector2D> waypoints_;
};

struct PatrolRoutes
{
	std::map<int, Route> patrolRoutes_;
};