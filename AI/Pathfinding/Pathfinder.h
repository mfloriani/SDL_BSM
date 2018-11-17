#pragma once
#include "Graph.h"
#include "GraphEdgeTypes.h"
#include "GraphNodeTypes.h"
#include <list>
#include "../../Utils/Config.h"
#include "SDL.h"
#include "AStar.h"
#include "AStarHeuristics.h"

class Pathfinder
{
private:
	enum 
	{
		invalid_cell = -1
	};
	typedef Graph<NavGraphNode<>, NavGraphEdge> NavGraph;
	typedef std::list<int>						PathList;
	typedef std::list<math::Vector2D>			WaypointsList;
	
	NavGraph&		graph_;
	PathList		path_;
	WaypointsList	pathWaypoints_;
	int				source_;
    int				target_;
	
private:
	int FindCell(math::Vector2D pos);

public:
	Pathfinder(NavGraph& g) :	graph_(g),
								source_(invalid_cell),
								target_(invalid_cell)
	{}
	virtual ~Pathfinder() { }

	void Draw(SDL_Renderer* r);	
	void CreateAStarPath();
	void ChangeSource(const int cell) { source_ = cell; }
	void ChangeTarget(const int cell) { target_ = cell; }
	void ChangeSource(const math::Vector2D pos) { source_ = FindCell(pos); }
	void ChangeTarget(const math::Vector2D pos) { target_ = FindCell(pos); }
	WaypointsList& GetPathWaypoints(){ return pathWaypoints_; }

};

