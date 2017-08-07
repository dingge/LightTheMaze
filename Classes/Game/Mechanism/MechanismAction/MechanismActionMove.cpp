#include "MechanismActionMove.h"

USING_NS_CC;

MechanismActionMoveBy * MechanismActionMoveBy::create(const Path & path, float duration)
{
    MechanismActionMoveBy* pAction = new MechanismActionMoveBy();
    pAction->initWithDuration(path, duration);
    pAction->autorelease();
    return pAction;
}

MechanismActionMoveBy * MechanismActionMoveBy::clone() const
{
    return MechanismActionMoveBy::create(path_, duration_);
}

MechanismActionMoveBy * MechanismActionMoveBy::reverse() const
{
    Path tmp = path_;
    tmp.reverse();
    return MechanismActionMoveBy::create(tmp, duration_);
}

void MechanismActionMoveBy::startWithTarget(cocos2d::Node * target)
{
//     ActionInterval::startWithTarget(target);
//     mechanism_node_ = static_cast<MechanismCollisionNode*>(target);
//     pre_pos_ = start_pos_ = mechanism_node_
}

MechanismActionMoveBy::MechanismActionMoveBy():
    mechanism_node_(nullptr),
    duration_(0.0f)
{
}

MechanismActionMoveBy::~MechanismActionMoveBy()
{
}
