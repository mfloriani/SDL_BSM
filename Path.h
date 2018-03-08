#pragma once
#include <list>
#include "Vector2D.h"

typedef std::list<math::Vector2D> WaypointsList;

class Path
{
private:
	WaypointsList&	waypoints_;
	math::Vector2D	curWaypoint_;
	int				curStatus_;
	bool			pathEnded_;

public:
	Path(WaypointsList& wp) : waypoints_(wp), pathEnded_(false)//, curWaypoint_(math::Vector2D(0,0))
	{ 
		SetNextWaypoint();
	}
	virtual ~Path(){}

	math::Vector2D	GetCurrentWaypoint()const { return curWaypoint_; }
	bool			IsPathEnded()const { return pathEnded_; }
	
	bool IsCurWaypointClose(math::Vector2D pos)const
	{
		if (pathEnded_) return false;

		if (math::distanceSqr(pos, curWaypoint_) < 150) return true;

		return false;
	}

	void SetNextWaypoint()
	{
		if (waypoints_.size() > 0)
		{
			curWaypoint_ = waypoints_.back();
			waypoints_.pop_back();
		}

		if (waypoints_.size() == 0) pathEnded_ = true;
	}

};