#include "Pathfinder.h"

void Pathfinder::CreateGraph()
{
	numCellsX_	= TILES_MAP_X;
	numCellsY_	= TILES_MAP_Y;
	cellWidth_	= TILE_WIDTH;
	cellHeight_ = TILE_HEIGHT;
	
	path_.clear();

	delete graph_;
	graph_ = new NavGraph();

	CreateGrid();
}

void Pathfinder::CreateGrid()
{
	double midX = cellWidth_ / 2;
	double midY = cellHeight_ / 2;

	for (int row = 0; row < numCellsY_; ++row)
	{
		for (int col = 0; col < numCellsX_; ++col)
		{
			graph_->AddNode(
				NavGraphNode<>(graph_->GetNextFreeNodeIndex(), 
				math::Vector2D(midX + (col * cellWidth_),				
							   midY + (row * cellHeight_))

			));
		}
	}

	for (int row = 0; row < numCellsY_; ++row)
	{
		for (int col = 0; col < numCellsX_; ++col)
		{
			LinkingEdges(row, col);	//add the edges...
		}
	}
}

bool Pathfinder::IsValidNeighbour(int x, int y)
{
	return !((x < 0) || (x >= numCellsX_) || (y < 0) || (y >= numCellsY_));
}

void Pathfinder::LinkingEdges(int row, int col)
{
	for (int i = -1; i < 2; ++i)		//goes through the Y
	{
		for (int j = -1; j < 2; ++j)	//goes through the X
		{
			int nodeX = col + j;
			int nodeY = row + i;
			
			if ((i == 0) && (j == 0)) continue; //skip if equal to the current node

			if (IsValidNeighbour(nodeX, nodeY))	//check if the node is inside the grid
			{
				int curNode = row * numCellsX_ + col;
				int neighbourNode = nodeY * numCellsX_ + nodeX;

				math::Vector2D nodePos = graph_->GetNode(curNode).GetPosition();
				math::Vector2D neighbourPos = graph_->GetNode(neighbourNode).GetPosition();

				double dist = math::distance(neighbourPos, nodePos);

				NavGraphEdge edge(curNode, neighbourNode, dist);
				graph_->AddEdge(edge);

				NavGraphEdge inverseEdge(neighbourNode, curNode, dist);
				graph_->AddEdge(inverseEdge);
			}
		}
	}
}

void Pathfinder::Draw(SDL_Renderer* r)
{
	//graph_->Draw(renderer);

	if (path_.size() == 0) return;

	SDL_SetRenderDrawColor(r, 0xFF, 0x00, 0x00, 0xFF);

	int count = 0;

	PathList::const_iterator curNode = path_.begin();
	for (curNode; curNode != path_.end(); ++curNode)
	{
		count++;
		int nodeId = (*curNode);
		int x = graph_->GetNode(nodeId).GetPosition().x;
		int y = graph_->GetNode(nodeId).GetPosition().y;
		SDL_RenderDrawPoint(r, x, y);


		if (count < path_.size())
		{
			auto nxt = std::next(curNode);
			int nxtId = (*nxt);
			int nxtX = graph_->GetNode(nxtId).GetPosition().x;
			int nxtY = graph_->GetNode(nxtId).GetPosition().y;

			SDL_RenderDrawLine(r, x, y, nxtX, nxtY);
		}
	}
}

void Pathfinder::CreateAStarPath()
{
	AStar<NavGraph, EuclideanDistance> aStar(*graph_, source_, target_);
	path_ = aStar.GetPathToTarget();
}

void Pathfinder::UpdateGraph(int cell, bool available)
{
	if (!available)
	{
		graph_->RemoveNode(cell);
	}
}