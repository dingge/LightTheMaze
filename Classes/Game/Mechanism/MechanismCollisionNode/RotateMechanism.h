/********************************************************************
	date  :	2016/01/30   15:34

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/
#ifndef ROTATEMECHANISM_H_
#define ROTATEMECHANISM_H_

#include "Game/Mechanism/MechanismCollisionNode/MechanismCollisionNode.h"
#include "Game/Extension/QuadTree.h"
#include "cocos2d.h"

class RotateMechanism : public MechanismCollisionNode
{
public:
	static RotateMechanism* create(const NodeGroupPtr& line_group,const cocos2d::Vec2& center);

    void updateMechanism(float angular_velocity);

    void processCollision(LightSpriteNode* node, const ContactInfo& info);
protected:
	RotateMechanism():
		center_pos_(cocos2d::Vec2::ZERO),
        pre_angle_(0.0f)
	{}
	virtual ~RotateMechanism(){}

	bool init(const NodeGroupPtr& line_group,const cocos2d::Vec2& center);

    void updatePhysicBody(float angular_velocity);

    void updateShape();
public:
	cocos2d::Vec2 center_pos_;
    float pre_angle_;
};
#endif // ROTATEMECHANISM_H_
