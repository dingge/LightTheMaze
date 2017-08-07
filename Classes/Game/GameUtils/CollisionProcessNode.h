/********************************************************************
	date  :	2016/01/30   16:02

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef COLLISION_PROCESS_NODE_H
#define COLLISION_PROCESS_NODE_H

#include "cocos2d.h"

class LightSpriteNode;
class DarkSprite;
class LightFragmentNode;
class StaticPhysicCollisionNode;
class MechanismCollisionNode;

/**
 *	��ײ��Ϣ
 *  ������ײʱ�����ı�Ҫ��Ϣ��Ϣ
 */
class ContactInfo
{
public:
    enum ContactType
    {
        ContactBegin,       //��ײ��ʼʱ
        ContactPreSolve,    //ÿһ�νӴ�ʱ
        ContactPostSolve,   //ÿһ�νӴ����Ѿ���ײʱ
        ContactSeparate     //��ײ����ʱ
    };
    ContactInfo(const ContactType& type) :type_(type) {}
    ContactInfo(const ContactType& type, const cocos2d::Vec2& impulse) :type_(type), impulse_(impulse) {}
    cocos2d::Vec2 getImpulse()const { return impulse_; }
    ContactType getType() const{ return type_; }
private:
    cocos2d::Vec2 impulse_; //��ײ����
    ContactType type_;      //��ײ����
};

/**
 *	��ײ����ࣨ�����ࣩ
 *	�Ե��̳з�ʽ���ӵ���Ҫ�����ײ��Node��
 *	��Node��Ҫʵ�ֵ�һ�����󷽷�
 *  ����Node��Ҫ���������Y����ײ�¼�ʱ������д�ýӿ�
 *	ʵ��N-N��ײ���ʹ���ʹ���麯�����ص��ȣ���ʹ��RTTI��
 *	ȱ�㣺ÿһ����Ҫ�����඼��Ӧ���µ�һ����ӿڣ���ӱȽ��鷳
 *	�÷�ʽԴ��More Effective C++
 */
class CollisionProcessNode :public cocos2d::Node
{
public:
    virtual void processCollision(CollisionProcessNode* node, const ContactInfo& info) = 0;      //������ص���
    virtual void processCollision(LightSpriteNode* node, const ContactInfo& info) {}             //X��⾫����ײ
    virtual void processCollision(DarkSprite* node, const ContactInfo& info) {}                  //X�밵������ײ
    virtual void processCollision(LightFragmentNode* node, const ContactInfo& info) {}           //X���֮��Ƭ��ײ
    virtual void processCollision(StaticPhysicCollisionNode* node, const ContactInfo& info) {}   //X�뾲̬������ײ
    virtual void processCollision(MechanismCollisionNode* node, const ContactInfo& info) {}      //X�������ײ
};
#endif // !COLLISION_PROCESS_NODE_H
