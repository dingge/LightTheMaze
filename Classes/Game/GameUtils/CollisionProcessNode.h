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
 *	碰撞信息
 *  包含碰撞时产生的必要信息信息
 */
class ContactInfo
{
public:
    enum ContactType
    {
        ContactBegin,       //碰撞开始时
        ContactPreSolve,    //每一次接触时
        ContactPostSolve,   //每一次接触且已经碰撞时
        ContactSeparate     //碰撞分离时
    };
    ContactInfo(const ContactType& type) :type_(type) {}
    ContactInfo(const ContactType& type, const cocos2d::Vec2& impulse) :type_(type), impulse_(impulse) {}
    cocos2d::Vec2 getImpulse()const { return impulse_; }
    ContactType getType() const{ return type_; }
private:
    cocos2d::Vec2 impulse_; //碰撞冲量
    ContactType type_;      //碰撞类型
};

/**
 *	碰撞检测类（抽象类）
 *	以单继承方式附加到需要检测碰撞的Node上
 *	该Node需要实现第一个抽象方法
 *  当该Node需要处理与对象Y的碰撞事件时，需重写该接口
 *	实现N-N碰撞检测和处理（使用虚函数二重调度，不使用RTTI）
 *	缺点：每一个需要检测的类都对应以下的一个虚接口，添加比较麻烦
 *	该方式源自More Effective C++
 */
class CollisionProcessNode :public cocos2d::Node
{
public:
    virtual void processCollision(CollisionProcessNode* node, const ContactInfo& info) = 0;      //处理二重调度
    virtual void processCollision(LightSpriteNode* node, const ContactInfo& info) {}             //X与光精灵碰撞
    virtual void processCollision(DarkSprite* node, const ContactInfo& info) {}                  //X与暗精灵碰撞
    virtual void processCollision(LightFragmentNode* node, const ContactInfo& info) {}           //X与光之碎片碰撞
    virtual void processCollision(StaticPhysicCollisionNode* node, const ContactInfo& info) {}   //X与静态物体碰撞
    virtual void processCollision(MechanismCollisionNode* node, const ContactInfo& info) {}      //X与机关碰撞
};
#endif // !COLLISION_PROCESS_NODE_H
