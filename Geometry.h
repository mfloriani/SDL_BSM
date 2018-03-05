#pragma once
#include "Vector2D.h"

//--------------------LineIntersection2D-------------------------
//
//	Given 2 lines in 2D space AB, CD this returns true if an 
//	intersection occurs.
//
//----------------------------------------------------------------- 

inline bool HasLineIntersection2D(math::Vector2D A, math::Vector2D B, math::Vector2D C, math::Vector2D D)
{
	double rTop = (A.y - C.y)*(D.x - C.x) - (A.x - C.x)*(D.y - C.y);
	double sTop = (A.y - C.y)*(B.x - A.x) - (A.x - C.x)*(B.y - A.y);

	double Bot = (B.x - A.x)*(D.y - C.y) - (B.y - A.y)*(D.x - C.x);

	if (Bot == 0) return false;	//parallel

	double invBot = 1.0 / Bot;
	double r = rTop * invBot;
	double s = sTop * invBot;

	if ((r > 0) && (r < 1) && (s > 0) && (s < 1)) return true;	//lines intersect
	
	return false;	//lines do not intersect
}