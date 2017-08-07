/********************************************************************
	date  :	2016/01/30   15:23

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef MOVEMECHANISM_H_
#define MOVEMECHANISM_H_

#include "Game/Mechanism/MechanismCollisionNode/StaticMechanism.h"
#include "Game/Extension/QuadTree.h"
#include "Game/GameUtils/Path.h"

/**
*  ��ͨ�ƶ�������Ϣ��
*  ��ʱֻ�������ѭ���ƶ�
*  �޽�����
*/
class MoveMechanism : public StaticMechanism
{
public:
	static MoveMechanism* create(const NodeGroupPtr& line_group);
	
    void updateMechanism(const cocos2d::Vec2& velocity);
protected:
	MoveMechanism() {}
	~MoveMechanism() {}

    void updatePhysicBody(const cocos2d::Vec2& velocity);

    void updateShape();
private:
    cocos2d::Vec2 m_preNodePos;
};
#endif // MOVEMECHANISM_H_