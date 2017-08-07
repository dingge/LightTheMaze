/********************************************************************
	date  :	2016/01/30   13:29

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef LOOPMOVEMECHANISM_H_
#define LOOPMOVEMECHANISM_H_

#include "Game/Mechanism/BaseMechanism.h"
#include "Game/Mechanism/MechanismCollisionNode/MoveMechanism.h"
#include "Game/Extension/QuadTree.h"
#include "Game/GameUtils/Path.h"

/**
*   循环移动机关（无交互）
*   根据指定运动路径运动
*   质量无穷大，可以拿来挡路
*/
class LoopMoveMechanism : public BaseMechanism
{
public:
	static LoopMoveMechanism* create(const NodeGroupPtr& line_group, const float begin_rate, const Path& path, float duration, bool is_reverse = false);

	virtual void update(float delta)override;
protected:
	LoopMoveMechanism() :
        mechanism_node_(nullptr),
        duration_(0.0f),
        escape_time_(0.0f),
        is_reverse_(false),
        is_positive_(true)
	{}
	virtual ~LoopMoveMechanism() {}

	bool init(const NodeGroupPtr& line_group, const float begin_rate, const Path& path, float duration, bool is_reverse);
private:
	MoveMechanism* mechanism_node_;
    cocos2d::Vec2 pre_pos_; //上一路径点
    Path path_;             //运动路径
    float duration_;        //运动总时长
    float escape_time_;     //这个时间不代表整体时间流逝，而是标记了当前起点路径与总路径的长度比
    bool is_reverse_;       //到边界是否反向运动
    bool is_positive_;      //标志当前是否正向移动
};
#endif // LOOPMOVEMECHANISM_H_
