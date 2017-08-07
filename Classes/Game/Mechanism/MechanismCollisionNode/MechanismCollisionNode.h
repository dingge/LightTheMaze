/********************************************************************
	date  :	2016/01/30   15:19

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef MECHANISMCOLLISIONNODE_H_
#define MECHANISMCOLLISIONNODE_H_

#include "cocos2d.h"
#include "Game/GameUtils/CollisionProcessNode.h"
#include "Game/GameUtils/Line.h"
#include "Game/Extension/QuadTree.h"

/**
*  机关结点类
*  带事件触发
*/
class MechanismCollisionNode :public CollisionProcessNode
{
public:
	virtual void processCollision(CollisionProcessNode* node, const ContactInfo& info);
	virtual void processCollision(LightSpriteNode* node, const ContactInfo& info);
	virtual void processCollision(DarkSprite* node, const ContactInfo& info);

	//获取当前机关的body产生的障碍形状，用于光照处理
	const NodeGroupPtr& getCurrentShape()const { return shape_; }

protected:
	MechanismCollisionNode():
        shape_(nullptr)
	{}
	virtual ~MechanismCollisionNode() {}

	virtual bool init()override;
protected:
	//用于保存和更新当前的body产生的形状
	NodeGroupPtr      shape_;
};
#endif // MECHANISMCOLLISIONNODE_H_