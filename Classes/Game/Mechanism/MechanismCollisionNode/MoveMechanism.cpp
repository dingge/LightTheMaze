#include "MoveMechanism.h"
#include "Game/GameUtils/ShapeManage.h"

USING_NS_CC;

//----------------------------------MoveMechanism----------------------------------
MoveMechanism* MoveMechanism::create(const NodeGroupPtr& line_group)
{
	MoveMechanism *p = new (std::nothrow) MoveMechanism();
	if (p && p->init(line_group))
	{
		p->autorelease();
		return p;
	}
	CC_SAFE_DELETE(p);
	return nullptr;
}

void MoveMechanism::updatePhysicBody(const cocos2d::Vec2 & velocity)
{
    this->getPhysicsBody()->setVelocity(velocity);
}

void MoveMechanism::updateShape()
{
    Vec2 delta_pos = this->getPosition() - m_preNodePos;

    ListNode<Edge>* p = shape_->head_;
    while (p != nullptr)
    {
        p->_node._p1 += delta_pos;
        p->_node._p2 += delta_pos;
        p = p->_next;
    }
    m_preNodePos = this->getPosition();
    
    shape_->fixUpRect();

    //¸üÐÂÕÏ°­
    ShapeManage::getInstance()->alterBody(shape_);
}

void MoveMechanism::updateMechanism(const cocos2d::Vec2& velocity)
{
    updatePhysicBody(velocity);
    updateShape();
}