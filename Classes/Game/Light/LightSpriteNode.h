/********************************************************************
	date  :	2016/01/30   13:00

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef LIGHTSPRITENODE_H_
#define LIGHTSPRITENODE_H_

#include "cocos2d.h"
#include "Game/GameUtils/CollisionProcessNode.h"
#include "Game/Light/LightNode.h"
#include "Game/Macros.h"

enum class LightSpriteFlag
{
	General,     //����״̬
	SkillFirst,  //��һ����
	SkillSecond, //�ڶ�����
	SkillThird,  //��������
	Running	     //�ڷż���
};
/**
*	��֮����ڵ��ࣨ������ײ��⣩
*/
class LightSpriteNode : public CollisionProcessNode
{
public:
	static LightSpriteNode* create(const std::string& pFileName, cocos2d::Color4B& lightColor, float bodyRadius);
	virtual void update(float delta)override;
	virtual void onEnter()override;
	virtual void onExit()override;

	//��Դ�ƶ�
	void moveLight(const cocos2d::Vec2& delta);
	
	//��дsetPosition��Ϊ����ȷ���¹��������Ϣ
	virtual void setPosition(const cocos2d::Vec2& pos)override;
	virtual void setPosition(float x, float y)override;
    virtual void setScale(float scale)override;

	//��д��Ϊ��ʹ����ͼ�����ӱ���ͬ����tag�Է��㴦��
	virtual void setTag(int tag)override;

	//��ȡ�󶨵Ĺ���
	LightNode* getLightNode() { return m_LightNode; }

	void setLightSpriteFlag(LightSpriteFlag flag) { m_flag = flag; }
	LightSpriteFlag getLightSpriteFlag()const { return m_flag; }

    float getHp()const { return m_HP; }
    void setHp(float hp) { m_HP = hp; }
public:
	//��ײ����
	virtual void processCollision(CollisionProcessNode* node, const ContactInfo& info);
	virtual void processCollision(LightSpriteNode* node, const ContactInfo& info);
	virtual void processCollision(DarkSprite* node, const ContactInfo& info);
	virtual void processCollision(LightFragmentNode* node, const ContactInfo& info);
	virtual void processCollision(MechanismCollisionNode* node, const ContactInfo& info);
protected:
	enum SkillConsume
	{
		FirstSkill = 30,
		SecondSkill = 10,
		ThirdSkill = 20
	};
	LightSpriteNode() :
		m_LightNode(nullptr),
		m_flag(LightSpriteFlag::General),
		m_lightSpriteStandradScale(1),
		m_HP(LIGHT_SPRITE_MAX_HP),
		m_isReplaceing(false)
	{}
	virtual ~LightSpriteNode() {}
    
	bool init(const std::string& pFileName, cocos2d::Color4B& lightColor, float bodyRadius);
    //����LightNode����
    void adjustLightNode();
private:
	LightNode* m_LightNode;                         //�󶨵Ĺ���
    cocos2d::ParticleSystemQuad* light_particle_;   //�⾫�����ӽ��
	LightSpriteFlag m_flag;                         //״̬��־
	float m_lightSpriteStandradScale;               //ԭʼ���ű�
	float m_HP;                                     //����ֵ

	bool m_isReplaceing;                            //���������ؿ��ķ�������־
};

#endif // LIGHTSPRITENODE_H_