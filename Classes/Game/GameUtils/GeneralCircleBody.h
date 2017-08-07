/********************************************************************
	date  :	2016/01/30   12:58

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef GENERALCIRCLEBODY_H_
#define GENERALCIRCLEBODY_H_

#include "cocos2d.h"
#include "Game/Macros.h"

/**
*	����һ��ͨ�õ�Բ�������ϰ�
*   ��Բ��bodyͨ����������ײ����������Ҫ��д��ȷ����������
*/
class GeneralCircleBody
{
public:
	static cocos2d::PhysicsBody* createCircleBody(float radius,
		float density = 1.0f,
		float friction = 0.0f,
		float restitution = 0.3f,
		int categoryBitmask = 0xffffffff,
		int contacttestBitmask = BODY_CONTACT_TEST_BITMARK,
		int collisionBitmask = 0xffffffff)
	{
		auto circleBody = cocos2d::PhysicsBody::createCircle(radius);
		circleBody->getShape(0)->setDensity(density);		   //�ܶ�
		circleBody->getShape(0)->setFriction(friction);	       //Ħ��
		circleBody->getShape(0)->setRestitution(restitution);  //���Իָ�
		circleBody->setCategoryBitmask(categoryBitmask);
		circleBody->setContactTestBitmask(contacttestBitmask);
		circleBody->setCollisionBitmask(collisionBitmask);
		return circleBody;
	}
};

#endif // GENERALCIRCLEBODY_H_



