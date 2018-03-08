#pragma once
#include <vector>
#include <list>
#include <cassert>
#include "NodeTypeEnum.h"
#include "SDL.h"

template <class node_type, class edge_type>
class Graph
{
public:
	typedef edge_type               EdgeType;
	typedef node_type               NodeType;

	typedef std::vector<node_type>	NodeVector;
	typedef std::list<edge_type>	EdgeList;
	typedef std::vector<EdgeList>	EdgeListVector;

private:
	NodeVector		nodes_;
	EdgeListVector	edges_;
	int				nextNodeIndex_;

	bool			IsUniqueEdge(int from, int to)const;

public:
	Graph() : nextNodeIndex_(0){}

	const node_type&	GetNode(int id)const { return nodes_[id]; }
	node_type&			GetNode(int id) { return nodes_[id]; }
	int					AddNode(node_type node);
	void				RemoveNode(int node);
	bool				IsNodePresent(int node)const;
	int					GetNodesCount()const { return nodes_.size(); }
	int					GetActiveNodesCount()const;
	

	const edge_type&	GetEdge(int from, int to)const;
	edge_type&			GetEdge(int from, int to);
	void				AddEdge(edge_type edge);
	void				RemoveEdge(int from, int to);
	void				SetEdgeCost(int from, int to, double cost);
	bool				IsEdgePresent(int from, int to)const;
	int					GetEdgesCount()const;
	void				RemoveEdges();

	int					GetNextFreeNodeIndex()const { return nextNodeIndex_; }
	bool				IsEmpty()const { return nodes_.empty(); }
	void				Clear() { nextNodeIndex_ = 0; nodes_.clear(); edges_.clear(); }
	void				Draw(SDL_Renderer* r);

	//const class used to iterate through the nodes in the graph
	class ConstNodeIterator
	{
	private:

		typename NodeVector::const_iterator	curNode;
		const Graph<node_type, edge_type>&	G;

		//if a graph node is removed or switched off, it is not removed from the 
		//vector of nodes (because that would mean changing all the indices of 
		//all the nodes that have a higher index. This method takes a node
		//iterator as a parameter and assigns the next valid element to it.
		void GetNextValidNode(typename NodeVector::const_iterator& it)
		{
			if (curNode == G.nodes_.end() || it->Index() != invalid_node_index) return;

			while ((it->Index() == invalid_node_index))
			{
				++it;
				if (curNode == G.nodes_.end()) break;
			}
		}

	public:

		ConstNodeIterator(const Graph<node_type, edge_type> &graph) :G(graph)
		{
			curNode = G.nodes_.begin();
		}


		const node_type* begin()
		{
			curNode = G.nodes_.begin();

			GetNextValidNode(curNode);

			return &(*curNode);
		}

		const node_type* next()
		{
			++curNode;

			if (end())
			{
				return NULL;
			}
			else
			{
				GetNextValidNode(curNode);

				return &(*curNode);
			}
		}

		bool end()
		{
			return (curNode == G.nodes_.end());
		}
	};

	friend class ConstNodeIterator;

	//const class used to iterate through all the edges connected to a specific node. 
	class ConstEdgeIterator
	{
	private:
		typename EdgeList::const_iterator   curEdge;
		const Graph<node_type, edge_type>&	G;
		const int                           NodeIndex;

	public:
		ConstEdgeIterator(const Graph<node_type, edge_type>& graph, int node) : G(graph), NodeIndex(node)
		{
			/* we don't need to check for an invalid node index since if the node is
			invalid there will be no associated edges
			*/
			curEdge = G.edges_[NodeIndex].begin();
		}

		const EdgeType*  begin()
		{
			curEdge = G.edges_[NodeIndex].begin();

			if (end()) return 0;

			return &(*curEdge);
		}

		const EdgeType*  next()
		{
			++curEdge;

			if (end())
			{
				return NULL;
			}
			else
			{
				return &(*curEdge);
			}
		}

		//return true if we are at the end of the edge list
		bool end()
		{
			return (curEdge == G.edges_[NodeIndex].end());
		}
	};

	friend class ConstEdgeIterator;

};

template <class node_type, class edge_type>
bool Graph<node_type, edge_type>::IsNodePresent(int node)const
{
	if (node >= (int)nodes_.size() || nodes_[node].GetId() == invalid_node_index)
		return false;
	else
		return true;
}

template <class node_type, class edge_type>
bool Graph<node_type, edge_type>::IsEdgePresent(int from, int to)const
{
	if (IsNodePresent(from) && IsNodePresent(to))
	{
		for (EdgeList::const_iterator curEdge = edges_[from].begin();
			curEdge != edges_[from].end();
			++curEdge;)
		{
			if (curEdge->GetTo() == to) return true;
		}
		return false;
	}
	else
		return false;
}

template <class node_type, class edge_type>
int	Graph<node_type, edge_type>::GetActiveNodesCount()const
{
	int count = 0;
	for (unsigned int i = 0; i < nodes_.size(); i++)
	{
		if (nodes_[i].GetId() != invalid_node_index) ++count;
	}
	return count;
}

template <class node_type, class edge_type>
int	Graph<node_type, edge_type>::GetEdgesCount()const
{
	int count = 0;

	for (EdgeListVector::const_iterator curEdge = edges_.begin();
		curEdge != edges_.end();
		++curEdge)
	{
		count += curEdge.size();
	}

	return count;
}

template <class node_type, class edge_type>
void Graph<node_type, edge_type>::RemoveEdges()
{
	for (EdgeListVector::iterator it = edges_.begin(); it != edges_.end(); ++it) it->clear();
}

template <class node_type, class edge_type>
const edge_type& Graph<node_type, edge_type>::GetEdge(int from, int to)const
{
	for (EdgeList::const_iterator curEdge = edges_[from].begin();
		curEdge != edges_[from].end();
		++curEdge)
	{
		if (curEdge->GetTo() == to) return *curEdge;
	}
	assert(0 && "<Graph::GetEdge>: edge does not exists");
}

template <class node_type, class edge_type>
edge_type& Graph<node_type, edge_type>::GetEdge(int from, int to)
{
	for (EdgeList::iterator curEdge = edges_[from].begin();
		curEdge != edges_[from].end();
		++curEdge)
	{
		if (curEdge->GetTo() == to) return *curEdge;
	}
	assert(0 && "<Graph::GetEdge>: edge does not exists");
}

template <class node_type, class edge_type>
int Graph<node_type, edge_type>::AddNode(node_type node)
{
	if (node.GetId() < (int)nodes_.size())	//trying to put a node with ID already used?
	{
		assert(nodes_[node.GetId()].GetId() == invalid_node_index 
			&& "<Graph::AddNode>: Attempting to add a duplicated ID node");

		nodes_[node.GetId()] = node;
		return nextNodeIndex_;
	}
	else
	{
		assert(node.GetId() == nextNodeIndex_
			&& "<Graph::AddNode>: Invalid index");

		nodes_.push_back(node);
		edges_.push_back(EdgeList());
		return nextNodeIndex_++;
	}
}

template <class node_type, class edge_type>
void Graph<node_type, edge_type>::RemoveNode(int node)
{
	nodes_[node].SetId(invalid_node_index);

	//remove all edges that point to this node
	for (EdgeList::iterator curEdge = edges_[node].begin();
		curEdge != edges_[node].end();
		++curEdge)
	{
		for (EdgeList::iterator curE = edges_[curEdge->GetTo()].begin();
			curE != edges_[curEdge->GetTo()].end();
			++curE)
		{
			if (curE->GetTo() == node)
			{
				curE = edges_[curEdge->GetTo()].erase(curE);
				break;
			}
		}
	}
	edges_[node].clear();
}


template <class node_type, class edge_type>
void Graph<node_type, edge_type>::AddEdge(edge_type edge)
{
	if (nodes_[edge.GetFrom()].GetId() != invalid_node_index
		&& nodes_[edge.GetTo()].GetId() != invalid_node_index)
	{
		if (IsUniqueEdge(edge.GetFrom(), edge.GetTo()))
		{
			edges_[edge.GetFrom()].push_back(edge);
		}

		if (IsUniqueEdge(edge.GetTo(), edge.GetFrom()))
		{
			edge_type newEdge = edge;
			newEdge.SetTo(edge.GetFrom());
			newEdge.SetFrom(edge.GetTo());
			edges_[edge.GetTo()].push_back(newEdge);
		}
	}
}

template <class node_type, class edge_type>
void Graph<node_type, edge_type>::RemoveEdge(int from, int to)
{
	EdgeList::iterator curEdge;

	for (curEdge = edges_[to].begin();
		curEdge != edges_[to].end();
		++curEdge)
	{
		if (curEdge->GetTo() == from)
		{
			curEdge = edges_[to].erase(curEdge);
			break;
		}
	}

	for (curEdge = edges_[from].begin();
		curEdge != edges_[from].end();
		++curEdge)
	{
		if (curEdge->GetTo() == to)
		{
			curEdge = edges_[from].erase(curEdge);
			break;
		}
	}
}

template <class node_type, class edge_type>
void Graph<node_type, edge_type>::SetEdgeCost(int from, int to, double NewCost)
{
	for (EdgeList::iterator curEdge = edges_[from].begin();
		curEdge != edges_[from].end();
		++curEdge)
	{
		if (curEdge->GetTo() == to)
		{
			curEdge->SetCost(NewCost);
			break;
		}
	}
}

template <class node_type, class edge_type>
bool Graph<node_type, edge_type>::IsUniqueEdge(int from, int to)const
{
	for (EdgeList::const_iterator curEdge = edges_[from].begin();
		curEdge != edges_[from].end();
		++curEdge)
	{
		if (curEdge->GetTo() == to) return false;
	}
	return true;
}

template <class node_type, class edge_type>
void Graph<node_type, edge_type>::Draw(SDL_Renderer* r)
{
	if (GetNodesCount() == 0) return;
	
	SDL_SetRenderDrawColor(r, 0xFF, 0x00, 0x00, 0xFF);

	NodeVector::const_iterator curNode = nodes_.begin();
	for (curNode; curNode != nodes_.end(); ++curNode)
	{
		SDL_RenderDrawPoint(r, curNode->GetPosition().x, curNode->GetPosition().y);

		EdgeList::const_iterator curEdge = edges_[curNode->GetId()].begin();
		for (curEdge; curEdge != edges_[curNode->GetId()].end(); ++curEdge)
		{
			SDL_RenderDrawLine(
				r,
				curNode->GetPosition().x,
				curNode->GetPosition().y,
				GetNode(curEdge->GetTo()).GetPosition().x,
				GetNode(curEdge->GetTo()).GetPosition().y
			);
		}	
	}
}