#pragma once
#include "NodeTypeEnum.h"

class GraphEdge
{
protected:
	int		from_;
	int		to_;
	double	cost_;

public:
	GraphEdge(): from_(invalid_node_index), to_(invalid_node_index), cost_(1.0){}
	GraphEdge(int from, int to) : from_(from), to_(to), cost_(1.0) {}
	GraphEdge(int from, int to, double cost) : from_(from), to_(to), cost_(cost) {}

	virtual ~GraphEdge(){}

	int		GetFrom()const { return from_; }
	void	SetFrom(int from){ from_ = from; }

	int		GetTo()const { return to_; }
	void	SetTo(int to) { to_ = to; }

	double	GetCost()const { return cost_; }
	void	SetCost(double cost) { cost_ = cost; }

	bool operator==(const GraphEdge& g)
	{
		return	g.from_ == this->from_	&&
				g.to_	== this->to_	&&
				g.cost_ == this->cost_;
	}

	bool operator!=(const GraphEdge& e)
	{
		return !(*this == e);
	}
};

class NavGraphEdge : public GraphEdge
{
public:
	NavGraphEdge(int from, int to, double cost) : GraphEdge(from, to, cost){}

};