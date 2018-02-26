#pragma once
#include "Graph.h"
#include "GraphEdgeTypes.h"
#include "GraphNodeTypes.h"
#include <list>
#include "Config.h"
#include "SDL.h"
#include "AStar.h"
#include "AStarHeuristics.h"

class Pathfinder
{
private:
	typedef Graph<NavGraphNode<>, NavGraphEdge> NavGraph;
	typedef std::list<int>						PathList;
	
	NavGraph*		graph_;
	PathList		path_;
	int				source_;
    int				target_;
	double			cellWidth_;
	double			cellHeight_;
	int				numCellsX_;
	int				numCellsY_;

private:
	//create the nodes based on a grid
	void CreateGrid();

	//create the edges linking the nodes by NW, N, NE, W, E, SW, S, SE
	void LinkingEdges(int row, int col);

	//check if the node is inside the grid
	bool IsValidNeighbour(int x, int y);

	

public:
	Pathfinder() :	graph_(NULL), 
					source_(0), 
					target_(0), 
					cellWidth_(0), 
					cellHeight_(0), 
					numCellsX_(0), 
					numCellsY_(0) 
	{};
	virtual ~Pathfinder() { delete graph_; };

	void Draw(SDL_Renderer* renderer);
	void CreateGraph();
	void CreateAStarPath();
	void ChangeSource(const int cell) { source_ = cell; }
	void ChangeTarget(const int cell) { target_ = cell; }
	void UpdateGraph(int cell, bool available);
};

