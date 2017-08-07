/********************************************************************
	date  :	2016/01/30   13:32

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef LOOPROTATEMECHANISM_H_
#define LOOPROTATEMECHANISM_H_

#include "Game/Mechanism/BaseMechanism.h"
#include "Game/Mechanism/MechanismCollisionNode/RotateMechanism.h"
#include "Game/Extension/QuadTree.h"

/**
 *	循环旋转机关（角度增大为顺时针方向）
 */
class LoopRotateMechanism : public BaseMechanism
{
public:
	static LoopRotateMechanism* create(const NodeGroupPtr& line_group, const cocos2d::Vec2& center, float beginAngle, float endAngle, float duration, bool isReverse = false);
	
    virtual void update(float delta)override;
protected:
	LoopRotateMechanism() :
        mechanism_node_(nullptr),
        begin_angle_(0),
        end_angle_(0),
        duration_(0),
        is_reverse_(false),
        cur_begin_angle_(0.0f),
        cur_end_angle_(0.0f)
	{}
	virtual ~LoopRotateMechanism() {}

	bool init(const NodeGroupPtr& line_group, const cocos2d::Vec2& center, float beginAngle, float endAngle, float duration, bool isReverse = false);
private:
	RotateMechanism* mechanism_node_;
    float begin_angle_;
    float end_angle_;
    float duration_;
    bool  is_reverse_;
    float cur_begin_angle_;
    float cur_end_angle_;
};
#endif // LOOPROTATEMECHANISM_H_
