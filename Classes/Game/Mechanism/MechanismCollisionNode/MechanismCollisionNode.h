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
*  ���ؽ����
*  ���¼�����
*/
class MechanismCollisionNode :public CollisionProcessNode
{
public:
	virtual void processCollision(CollisionProcessNode* node, const ContactInfo& info);
	virtual void processCollision(LightSpriteNode* node, const ContactInfo& info);
	virtual void processCollision(DarkSprite* node, const ContactInfo& info);

	//��ȡ��ǰ���ص�body�������ϰ���״�����ڹ��մ���
	const NodeGroupPtr& getCurrentShape()const { return shape_; }

protected:
	MechanismCollisionNode():
        shape_(nullptr)
	{}
	virtual ~MechanismCollisionNode() {}

	virtual bool init()override;
protected:
	//���ڱ���͸��µ�ǰ��body��������״
	NodeGroupPtr      shape_;
};
#endif // MECHANISMCOLLISIONNODE_H_