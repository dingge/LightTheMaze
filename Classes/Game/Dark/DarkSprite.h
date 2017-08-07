/********************************************************************
	date  :	2016/01/30   13:27

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef DARK_SPRITE_H
#define DARK_SPRITE_H

#include "Game/Macros.h"
#include "Game/GameUtils/Line.h"
#include "Game/GameUtils/ShapeManage.h"
#include "Game/GameUtils/CollisionProcessNode.h"
#include "cocos2d.h"

/**
 *	暗精灵类
 *	附加了物理属性,碰撞检测
 *	TODO:其他属性待加
 */
class DarkSprite : public CollisionProcessNode
{
public:
	enum DarkSpriteFlag
	{
		Normal = 0x0001, //常规状态
		Attack = 0x0010, //攻击状态
		Escape = 0x0100  //逃离状态
	};
public:
	static DarkSprite* create(const std::string& filename,float radius);
	bool init(const std::string& filename,float radius);
	void update(float delta)override;

	//常规动作
	void setNormalAction(cocos2d::Action* action);
	void runNormalAction();
	void stopNormalAction();

	float getHP()const {return m_HP;}
	void setHP(float hp) {m_HP = hp;}

	//追踪动作
	void attackTarget(const cocos2d::Vec2& targetPos);
	void lostAttackTarget();

	//逃避动作
	void escapeTarget(const cocos2d::Vec2& targetPos);
	void lostEscapeTarrget(const cocos2d::Vec2& targetPos);
	
	void setShined(bool isshine) {m_isShined = isshine;}
public:
    virtual void processCollision(CollisionProcessNode* node, const ContactInfo& info);
    virtual void processCollision(LightSpriteNode* node, const ContactInfo& info);
    virtual void processCollision(MechanismCollisionNode* node, const ContactInfo& info);
protected:
    DarkSprite();
	virtual ~DarkSprite();

private:
    cocos2d::ParticleSystemQuad* partticle_node_;
	unsigned int     m_flag;
	cocos2d::Action* m_pNormalAction;       ///<常规动作
	cocos2d::Action* m_currentAction;       ///<当前动作
	cocos2d::Vec2    m_originPos;           ///<起点(预留)
	cocos2d::Vec2    m_attackTargetPos;     ///<攻击目标点
	cocos2d::Vec2    m_escapeTargetPos;     ///<逃避目标点(往后可能添加多个目标点)
	float m_rate;
	float m_darkSpriteStandradScale; //原始缩放比
	float m_maxHP;
	float m_HP;
	bool  m_isShined;	//标记是否灯塔已经点亮
};
#endif // !DARK_SPRITE_H
