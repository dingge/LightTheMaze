#include "LoopMoveMechanism.h"
#include "Game/Macros.h"
#include "Utils.h"

USING_NS_CC;

//----------------------------------LoopMoveMechanism----------------------------------
LoopMoveMechanism * LoopMoveMechanism::create(const NodeGroupPtr & line_group, const float begin_rate, const Path & path, float duration, bool is_reverse)
{
    LoopMoveMechanism *p = new (std::nothrow) LoopMoveMechanism();
    if (p && p->init(line_group, begin_rate, path, duration, is_reverse))
    {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool LoopMoveMechanism::init(const NodeGroupPtr & line_group, const float begin_rate, const Path & path, float duration, bool is_reverse)
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!BaseMechanism::init());

        path_ = path;
        duration_ = duration;
        is_reverse_ = is_reverse;
        escape_time_ = begin_rate*duration_;

        mechanism_node_ = MoveMechanism::create(line_group);
        mechanism_node_->setPosition(Vec2::ZERO);

        this->addChild(mechanism_node_);

        m_mechanismVector.push_back(mechanism_node_);

        bRet = true;
    } while (0);
    return bRet;
}

void LoopMoveMechanism::update(float delta)
{
     //计算速率，此处速度以秒为单位
    float speed = path_.getPathLength() / duration_;
    float cur_time_delta = delta*(is_positive_ ? 1 : -1);
    //边界处理
    if (escape_time_ + cur_time_delta > duration_ || escape_time_ + cur_time_delta < 0.0f)
    {
        if (is_reverse_)
        {
            is_positive_ = !is_positive_;
            cur_time_delta = delta*(is_positive_ ? 1 : -1);
        }
        else
        {
            escape_time_ = 0.0;
            mechanism_node_->setPosition(Vec2::ZERO);
        }
    }
    float rate = getFloatMod(escape_time_ + cur_time_delta, duration_) / duration_;
    Vec2 next_pos = path_.getLinearLerpPos(rate);
    
    //由速率及切向方向计算速度
    Vec2 velocity = (next_pos - pre_pos_).getNormalized()*speed;
    mechanism_node_->updateMechanism(velocity);

    pre_pos_ = next_pos;
    escape_time_ += cur_time_delta;

	BaseMechanism::update(delta);
}
