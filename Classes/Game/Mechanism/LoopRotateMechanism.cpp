#include "LoopRotateMechanism.h"
#include "Game/GameManage.h"
#include "Utils.h"

USING_NS_CC;

#define PI 3.1415926535897932385f

//----------------------------------LoopRotateMechanism----------------------------------
LoopRotateMechanism* LoopRotateMechanism::create(const NodeGroupPtr& line_group, const Vec2& center, float beginAngle, float endAngle, float duration, bool isReverse /*= false*/)
{
	LoopRotateMechanism *p = new (std::nothrow) LoopRotateMechanism();
	if (p && p->init(line_group, center, beginAngle, endAngle, duration, isReverse))
	{
		p->autorelease();
		return p;
	}
	CC_SAFE_DELETE(p);
	return nullptr;
}

bool LoopRotateMechanism::init(const NodeGroupPtr& line_group, const Vec2& center, float beginAngle, float endAngle, float duration, bool isReverse /*= false*/)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!BaseMechanism::init());

        CCASSERT(beginAngle >= 0 || beginAngle <= 360, "begin angle must between 0 and 360 degrees");
        CCASSERT(endAngle >= 0 || endAngle <= 360, "end angle must between 0 and 360 degrees");
        CCASSERT(duration >= 0, "duration must > 0");

        cur_begin_angle_ = begin_angle_ = beginAngle;
        cur_end_angle_ = end_angle_ = endAngle;
        duration_ = duration;
        is_reverse_ = isReverse;

		mechanism_node_ = RotateMechanism::create(line_group, center);
		mechanism_node_->setPosition(center);
        mechanism_node_->setRotation(begin_angle_);

		this->addChild(mechanism_node_);

		m_mechanismVector.push_back(mechanism_node_);

		bRet = true;
	} while (0);
	return bRet;
}

void LoopRotateMechanism::update(float delta)
{
    float angular_velocity = (cur_end_angle_ - cur_begin_angle_) / duration_ * delta;
    float cur_angle = getFloatMod(mechanism_node_->getRotation(), CC_RADIANS_TO_DEGREES(PI * 2));
    if (cur_end_angle_ > cur_begin_angle_ && cur_end_angle_ < cur_angle + angular_velocity ||
        cur_end_angle_ < cur_begin_angle_ && cur_end_angle_ > cur_angle + angular_velocity)
    {
        if (is_reverse_)
        {
            float tmp = cur_begin_angle_;
            cur_begin_angle_ = cur_end_angle_;
            cur_end_angle_ = tmp;
            angular_velocity = (cur_end_angle_ - cur_begin_angle_) / duration_ * delta;
        }
        else
        {
            mechanism_node_->setRotation(cur_begin_angle_);
        }
    }
    
    //Physic的线速度方向是逆时针方向
    mechanism_node_->updateMechanism(-angular_velocity);

	BaseMechanism::update(delta);
}