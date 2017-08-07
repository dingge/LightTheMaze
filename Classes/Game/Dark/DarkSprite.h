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
 *	��������
 *	��������������,��ײ���
 *	TODO:�������Դ���
 */
class DarkSprite : public CollisionProcessNode
{
public:
	enum DarkSpriteFlag
	{
		Normal = 0x0001, //����״̬
		Attack = 0x0010, //����״̬
		Escape = 0x0100  //����״̬
	};
public:
	static DarkSprite* create(const std::string& filename,float radius);
	bool init(const std::string& filename,float radius);
	void update(float delta)override;

	//���涯��
	void setNormalAction(cocos2d::Action* action);
	void runNormalAction();
	void stopNormalAction();

	float getHP()const {return m_HP;}
	void setHP(float hp) {m_HP = hp;}

	//׷�ٶ���
	void attackTarget(const cocos2d::Vec2& targetPos);
	void lostAttackTarget();

	//�ӱܶ���
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
	cocos2d::Action* m_pNormalAction;       ///<���涯��
	cocos2d::Action* m_currentAction;       ///<��ǰ����
	cocos2d::Vec2    m_originPos;           ///<���(Ԥ��)
	cocos2d::Vec2    m_attackTargetPos;     ///<����Ŀ���
	cocos2d::Vec2    m_escapeTargetPos;     ///<�ӱ�Ŀ���(���������Ӷ��Ŀ���)
	float m_rate;
	float m_darkSpriteStandradScale; //ԭʼ���ű�
	float m_maxHP;
	float m_HP;
	bool  m_isShined;	//����Ƿ�����Ѿ�����
};
#endif // !DARK_SPRITE_H
