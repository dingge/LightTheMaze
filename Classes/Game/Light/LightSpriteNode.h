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
	General,     //常规状态
	SkillFirst,  //第一技能
	SkillSecond, //第二技能
	SkillThird,  //第三技能
	Running	     //在放技能
};
/**
*	光之精灵节点类（附加碰撞检测）
*/
class LightSpriteNode : public CollisionProcessNode
{
public:
	static LightSpriteNode* create(const std::string& pFileName, cocos2d::Color4B& lightColor, float bodyRadius);
	virtual void update(float delta)override;
	virtual void onEnter()override;
	virtual void onExit()override;

	//光源移动
	void moveLight(const cocos2d::Vec2& delta);
	
	//重写setPosition，为了正确更新光照相关信息
	virtual void setPosition(const cocos2d::Vec2& pos)override;
	virtual void setPosition(float x, float y)override;
    virtual void setScale(float scale)override;

	//重写，为了使光照图与粒子保持同样的tag以方便处理
	virtual void setTag(int tag)override;

	//获取绑定的光结点
	LightNode* getLightNode() { return m_LightNode; }

	void setLightSpriteFlag(LightSpriteFlag flag) { m_flag = flag; }
	LightSpriteFlag getLightSpriteFlag()const { return m_flag; }

    float getHp()const { return m_HP; }
    void setHp(float hp) { m_HP = hp; }
public:
	//碰撞处理
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
    //修正LightNode坐标
    void adjustLightNode();
private:
	LightNode* m_LightNode;                         //绑定的光结点
    cocos2d::ParticleSystemQuad* light_particle_;   //光精灵粒子结点
	LightSpriteFlag m_flag;                         //状态标志
	float m_lightSpriteStandradScale;               //原始缩放比
	float m_HP;                                     //生命值

	bool m_isReplaceing;                            //用于重启关卡的防崩溃标志
};

#endif // LIGHTSPRITENODE_H_