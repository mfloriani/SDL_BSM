#include "Pathfinder.h"

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
		int x = graph_.GetNode(nodeId).GetPosition().x;
		int y = graph_.GetNode(nodeId).GetPosition().y;
		SDL_RenderDrawPoint(r, x, y);

		if (count < path_.size())
		{
			auto nxt = std::next(curNode);
			int nxtId = (*nxt);
			int nxtX = graph_.GetNode(nxtId).GetPosition().x;
			int nxtY = graph_.GetNode(nxtId).GetPosition().y;

			SDL_RenderDrawLine(r, x, y, nxtX, nxtY);
		}
	}
}

void Pathfinder::CreateAStarPath()
{
	if (target_ == invalid_cell || source_ == invalid_cell) return;

	AStar<NavGraph, EuclideanDistance> aStar(graph_, source_, target_);
	path_ = aStar.GetPathToTarget();
	pathWaypoints_ = aStar.GetWaypointsToTarget();
}

int Pathfinder::FindCell(math::Vector2D pos)
{
	int x = pos.x / TILE_WIDTH;
	int y = pos.y / TILE_HEIGHT;

	int node = y * TILES_MAP_X + x;

	return node;
}
