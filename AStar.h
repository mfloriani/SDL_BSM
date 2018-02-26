#pragma once
#include <vector>
#include <list>
#include "PriorityQueue.h"

template <class graph_type, class heuristic>
class AStar
{
private:
	typedef typename graph_type::EdgeType Edge;

	const graph_type&			graph_;
	std::vector<double>			GCosts_;
	std::vector<double>			FCosts_;
	std::vector<const Edge*>	shortestPathTree_;
	std::vector<const Edge*>	searchFrontier_;
	int							source_;
	int							target_;

private:
	void Search();

public:
	AStar(graph_type& g,
		  int s,
		  int t) :	graph_(g),
					source_(s),
					target_(t),
					shortestPathTree_(g.GetNodesCount()),
					searchFrontier_(g.GetNodesCount()),
					GCosts_(g.GetNodesCount(), 0.0),
					FCosts_(g.GetNodesCount(), 0.0)
	{
		Search();
	}
	
	std::vector<Edge*>	GetSPT()const { return shortestPathTree_; }
	std::list<int>		GetPathToTarget()const;
};

template <class graph_type, class heuristic>
void AStar<graph_type, heuristic>::Search()
{
	//create an indexed priority queue of nodes. The nodes with the
	//lowest overall F cost (G+H) are positioned at the front.
	IndexedPriorityQLow<double> pq(FCosts_, graph_.GetNodesCount());

	//put the source node on the queue
	pq.insert(source_);

	//while the queue is not empty
	while (!pq.empty())
	{
		//get lowest cost node from the queue
		int NextClosestNode = pq.Pop();

		//move this node from the frontier to the spanning tree
		shortestPathTree_[NextClosestNode] = searchFrontier_[NextClosestNode];

		//if the target has been found exit
		if (NextClosestNode == target_) return;

		//now to test all the edges attached to this node
		graph_type::ConstEdgeIterator ConstEdgeItr(graph_, NextClosestNode);

		for (const Edge* e = ConstEdgeItr.begin();
			!ConstEdgeItr.end();
			e = ConstEdgeItr.next())
		{
			//calculate the heuristic cost from this node to the target (H)                       
			double HCost = heuristic::Calculate(graph_, target_, e->GetTo());

			//calculate the 'real' cost to this node from the source (G)
			double GCost = GCosts_[NextClosestNode] + e->GetCost();

			//if the node has not been added to the frontier, add it and update the G and F costs
			if (searchFrontier_[e->GetTo()] == NULL)
			{
				FCosts_[e->GetTo()] = GCost + HCost;
				GCosts_[e->GetTo()] = GCost;

				pq.insert(e->GetTo());

				searchFrontier_[e->GetTo()] = e;
			}

			//if this node is already on the frontier but the cost to get here
			//is cheaper than has been found previously, update the node
			//costs and frontier accordingly.
			else if ((GCost < GCosts_[e->GetTo()]) && (shortestPathTree_[e->GetTo()] == NULL))
			{
				FCosts_[e->GetTo()] = GCost + HCost;
				GCosts_[e->GetTo()] = GCost;

				pq.ChangePriority(e->GetTo());

				searchFrontier_[e->GetTo()] = e;
			}
		}
	}
}

template <class graph_type, class heuristic>
std::list<int> AStar<graph_type, heuristic>::GetPathToTarget()const
{
	std::list<int> path;
	
	int node = target_;
	path.push_back(node);

	while (node != source_ && shortestPathTree_[node] != 0)
	{
		node = shortestPathTree_[node]->GetFrom();
		path.push_back(node);
	}

	return path;
}
