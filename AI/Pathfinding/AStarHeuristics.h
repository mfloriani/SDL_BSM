#pragma once
#include "../../Math/Vector2D.h"

class EuclideanDistance
{
public:
	EuclideanDistance() {}

	//calculate the straight line distance from node nd1 to node nd2
	template <class graph_type>
	static double Calculate(const graph_type& G, int nd1, int nd2)
	{
		return math::distance(G.GetNode(nd1).GetPosition(), G.GetNode(nd2).GetPosition());
	}
};