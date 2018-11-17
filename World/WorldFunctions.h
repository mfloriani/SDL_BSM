#pragma once
#include "../Math/Vector2D.h"
#include "Wall.h"
#include "../Math/Geometry.h"

template <class WallList>
inline bool DoWallsObstructLineSegment(math::Vector2D from, math::Vector2D to, const WallList& walls)
{
	WallList::const_iterator curWall = walls.begin();
	for (curWall; curWall != walls.end(); ++curWall)
	{
		if (HasLineIntersection2D(from, to, (*curWall)->GetFrom(), (*curWall)->GetTo())) return true;
	}
	return false;
}

/*
Check if the target position is inside the field of view of the origin based on the origin current direction
*/
inline bool HasOriginFOVonTarget(math::Vector2D originPos, math::Vector2D originDir, math::Vector2D targetPos, double fov)
{
	math::Vector2D toTarget = (targetPos - originPos).normalize();

	return originDir.dot(toTarget) >= cos(fov / 2.0);
}