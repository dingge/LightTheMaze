/********************************************************************
	date  :	2016/01/30   13:27

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef BASEMECHANISM_H_
#define BASEMECHANISM_H_

#include "Game/Mechanism/MechanismCollisionNode/MechanismCollisionNode.h"
#include "Game/GameUtils/DrawNodeSpec.h"
#include "cocos2d.h"

/**
*   ���ػ���
*   �������л��ز���
*/
class BaseMechanism : public cocos2d::Node
{
protected:
	virtual bool init()override;
	virtual void update(float delta)override;

	//���ƻ���
	virtual void drawMechanism();
protected:
	BaseMechanism();
	virtual ~BaseMechanism();
protected:
	DrawNodeSpec* m_drawNode;
	//����������Ҫ���Ƶ�DrawNode�Ļ��ؽ��
	std::vector<MechanismCollisionNode*> m_mechanismVector;
};
#endif // BASEMECHANISM_H_
