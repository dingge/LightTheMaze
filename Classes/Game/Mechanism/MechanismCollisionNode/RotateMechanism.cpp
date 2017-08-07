#include "RotateMechanism.h"
#include "Game/GameUtils/ShapeManage.h"
#include "Game/Light/LightSpriteNode.h"

#define PI 3.1415926535897932385f

USING_NS_CC;

//----------------------------------RotateMechanism----------------------------------
RotateMechanism* RotateMechanism::create( const NodeGroupPtr& line_group,const Vec2& center)
{
	RotateMechanism *p = new (std::nothrow) RotateMechanism();
	if (p && p->init(line_group,center))
	{
		p->autorelease();
		return p;
	}
	CC_SAFE_DELETE(p);
	return nullptr;
}

bool RotateMechanism::init( const NodeGroupPtr& line_group,const Vec2& center)
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!MechanismCollisionNode::init());

		shape_ = line_group;
        center_pos_ = center;

        ShapeManage::getInstance()->addBody(line_group);

		//建立障碍
        auto edgebody = PhysicsBody::create();
        ListNode<Edge>* p = line_group->head_;
        while (p != nullptr)
        {
            auto tshape = PhysicsShapeEdgeSegment::create(p->_node._p1 - center, p->_node._p2 - center);
            tshape->setTag(p->_node._id);
            tshape->setMass(PHYSICS_INFINITY);
            edgebody->addShape(tshape);
            p = p->_next;
        }
 		edgebody->setMass(PHYSICS_INFINITY);
 		edgebody->setContactTestBitmask(BODY_CONTACT_TEST_BITMARK);
		this->setPhysicsBody(edgebody);

		this->scheduleUpdate();

		bRet  = true;
	} while (0);
	return bRet;
}

void RotateMechanism::updatePhysicBody(float angular_velocity)
{
    this->getPhysicsBody()->setAngularVelocity(angular_velocity);
}

void RotateMechanism::updateShape()
{
	float deltaAngle = this->getRotation() - pre_angle_;
	float rad = CC_DEGREES_TO_RADIANS(deltaAngle);
    ListNode<Edge>* p = shape_->head_;
    while (p != nullptr)
    {
        p->_node._p1.rotate(center_pos_, -rad);
        p->_node._p2.rotate(center_pos_, -rad);
        p = p->_next;
    }

    pre_angle_ = this->getRotation();

	//更新障碍
	ShapeManage::getInstance()->alterBody(shape_);
}

void RotateMechanism::updateMechanism(float angular_velocity)
{
    updatePhysicBody(angular_velocity);
    updateShape();
}

void RotateMechanism::processCollision(LightSpriteNode * node, const ContactInfo & info)
{
    if (info.getType() == ContactInfo::ContactType::ContactPostSolve)
    {
        float impulse = info.getImpulse().length();
        if (impulse > 50.0f)
        {
            node->setHp(node->getHp() - 30.0f);
        }
    }
}
