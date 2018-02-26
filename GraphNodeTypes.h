#pragma once
#include "NodeTypeEnum.h"
#include "Vector2D.h"

class GraphNode
{
protected:
	int id_;

public:
	GraphNode() : id_(invalid_node_index) {}
	GraphNode(int id) : id_(id){}
	
	virtual ~GraphNode() {}

	void SetId(int id) { id_ = id; }
	int GetId()const { return id_; }

};

template <class extra_info = void*>
class NavGraphNode : public GraphNode
{
protected:
	math::Vector2D	position_;
	extra_info		extraInfo_;

public:
	NavGraphNode() : extraInfo_(extra_info()){}
	NavGraphNode(int id, 
				 math::Vector2D pos) :	GraphNode(id), 
										position_(pos), 
										extraInfo_(extra_info()){}

	virtual ~NavGraphNode(){}

	math::Vector2D	GetPosition()const { return position_; }
	void			SetPosition(math::Vector2D pos) { position_ = pos; }

	extra_info		GetExtraInfo()const { return extraInfo_; }
	void			SetExtraInfo(extra_info info) { extraInfo_ = info; }
};