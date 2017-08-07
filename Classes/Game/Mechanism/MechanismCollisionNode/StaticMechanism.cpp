#include "StaticMechanism.h"
#include "Game/Macros.h"
#include "Game/GameUtils/ShapeManage.h"
#include "Game/Light/LightSpriteNode.h"

USING_NS_CC;

//----------------------------------StaticMechanism----------------------------------
StaticMechanism::StaticMechanism()
{

}

StaticMechanism::~StaticMechanism()
{
    ShapeManage::getInstance()->removeBody(shape_);
}

StaticMechanism* StaticMechanism::create(const NodeGroupPtr& line_group)
{
	StaticMechanism *p = new (std::nothrow) StaticMechanism();
	if (p && p->init(line_group))
	{
		p->autorelease();
		return p;
	}
	CC_SAFE_DELETE(p);
	return nullptr;
}

void StaticMechanism::processCollision(LightSpriteNode * node, const ContactInfo & info)
{
    if (info.getType() == ContactInfo::ContactType::ContactPostSolve)
    {
        float impulse = info.getImpulse().length();
        CCLOG("impulse : %f, hp : %f", impulse,node->getHp());
        if (impulse > 100.0f)
        {
            node->setHp(0.0f);
        }
        else if(impulse > 30.0f)
        {
            node->setHp(node->getHp() - (impulse)*0.5f);
        }
    }
}

bool StaticMechanism::init(const NodeGroupPtr& line_group)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!MechanismCollisionNode::init());

        shape_ = line_group;
        ShapeManage::getInstance()->addBody(shape_);

		//Ω®¡¢’œ∞≠
		auto edgebody = PhysicsBody::create();
        ListNode<Edge>* p = shape_->head_;
        while (p != nullptr)
        {
            auto tshape = PhysicsShapeEdgeSegment::create(p->_node._p1, p->_node._p2);
            tshape->setTag(p->_node._id);
            tshape->setMass(PHYSICS_INFINITY);
            edgebody->addShape(tshape);
            p = p->_next;
        }
		edgebody->setMass(PHYSICS_INFINITY);
		edgebody->setContactTestBitmask(BODY_CONTACT_TEST_BITMARK);
		this->setPhysicsBody(edgebody);

		bRet = true;
	} while (0);
	return bRet;
}

cocos2d::Vec2 StaticMechanism::getCenterPos(const NodeGroupPtr & line_group)
{
    Vec2 center_pos = Vec2::ZERO;
    int point_num = 0;
    ListNode<Edge>* p = line_group->head_;
    while (p != nullptr)
    {
        center_pos += p->_node._p1 + p->_node._p2;
        point_num += 2;
        p = p->_next;
    }
    return center_pos / point_num;
}
