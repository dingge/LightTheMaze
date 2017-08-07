/********************************************************************
	date  :	2016/01/30   13:18

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef STATICPHYSICCOLLISIONNODE_H_
#define STATICPHYSICCOLLISIONNODE_H_

#include "cocos2d.h"
#include "Game/GameUtils/CollisionProcessNode.h"

/**
*	静态物理障碍结点类（附加碰撞检测）
*	用于不可交互的静态障碍
*/
class StaticPhysicCollisionNode : public CollisionProcessNode
{
public:
	CREATE_FUNC(StaticPhysicCollisionNode);
public:
	virtual void processCollision(CollisionProcessNode* node, const ContactInfo& info);
};
#endif // STATICPHYSICCOLLISIONNODE_H_