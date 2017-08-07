/********************************************************************
	date  :	2016/01/30   15:22

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef STATICMECHANISM_H_
#define STATICMECHANISM_H_

#include "Game/Mechanism/MechanismCollisionNode/MechanismCollisionNode.h"
#include "Game/Extension/QuadTree.h"

/**
*   静态机关类
*   不会动的，可以由其他机关触发进行某种处理
*/
class StaticMechanism : public MechanismCollisionNode
{
public:
	static StaticMechanism* create(const NodeGroupPtr& line_group);

    virtual void processCollision(LightSpriteNode* node, const ContactInfo& info);
    
    cocos2d::Vec2 getCenterPos(const NodeGroupPtr& line_group);
protected:
	StaticMechanism();
	virtual ~StaticMechanism();

	bool init(const NodeGroupPtr& line_group);

private:
};
#endif // STATICMECHANISM_H_