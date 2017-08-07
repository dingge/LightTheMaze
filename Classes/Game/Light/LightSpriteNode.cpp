#include "LightSpriteNode.h"
#include "SimpleAudioEngine.h"
#include "Game/GameUtils/GeneralCircleBody.h"
#include "Game/Light/LightFragmentNode.h"
#include "Game/Light/Action/LightRadiusAction.h"
#include "Game/Dark/DarkSprite.h"
#include "Game/GameManage.h"
#include "Setting.h"

USING_NS_CC;

LightSpriteNode* LightSpriteNode::create(const std::string& pFileName, Color4B& lightColor, float bodyRadius)
{
	LightSpriteNode *ret = new LightSpriteNode();
	if (ret && ret->init(pFileName, lightColor, bodyRadius))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return ret;
}

bool LightSpriteNode::init(const std::string& pFileName, Color4B& lightColor, float bodyRadius)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!CollisionProcessNode::init());

		//根据半径调整碎片缩放大小，以自身尺寸为单位，因为粒子结点无大小，预定义一个标准大小32
		m_lightSpriteStandradScale = bodyRadius*2.0f / 32;
		this->setScale(m_lightSpriteStandradScale);

		m_LightNode = LightNode::create();
        m_LightNode->setLightRadius(bodyRadius);
		m_LightNode->setLightColor(Color4F(lightColor));
		m_LightNode->setScale(1 / this->getScale());
		this->addChild(m_LightNode);

        light_particle_ = ParticleSystemQuad::create(pFileName);
        //让精灵运动时都拖尾
        light_particle_->setPositionType(ParticleSystem::PositionType::RELATIVE);
        light_particle_->setPosition(Vec2::ZERO);
        this->addChild(light_particle_);

		auto pBody = GeneralCircleBody::createCircleBody(bodyRadius);
		pBody->setVelocityLimit(LIGHT_SPRITE_RATE_LIMIT);
		pBody->setMass(m_HP * 2);
		this->setPhysicsBody(pBody);

        scheduleUpdate();
		bRet = true;
	} while (0);
	return bRet;
}

void LightSpriteNode::processCollision(CollisionProcessNode* node, const ContactInfo& info)
{
    node->processCollision(this, info);
}

void LightSpriteNode::processCollision(LightSpriteNode* node, const ContactInfo& info)
{
	//log("LightSpriteNode...LightSpriteNode collision!");
	//合体。。。
}

void LightSpriteNode::processCollision(LightFragmentNode* node, const ContactInfo& info)
{
	if (node->isFragmentCollected() == false)
	{
		//此步耗时
		//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/CrystalRing.mp3");
		//吃碎片满血复活
		float add = LIGHT_SPRITE_MAX_HP / Setting::getInstance()->getDifficulty();
		m_HP += add;
		if (m_HP > LIGHT_SPRITE_MAX_HP)
			m_HP = LIGHT_SPRITE_MAX_HP;
	}
	//设置已收集
	node->setFragmentCollected(true);
}

void LightSpriteNode::processCollision(DarkSprite* node, const ContactInfo& info)
{
	//公共的损耗
	const float loss = Setting::getInstance()->getLossVelocity();
	m_HP -= loss;
	auto pDark = node;
	pDark->setHP(pDark->getHP() - loss);
}

void LightSpriteNode::processCollision(MechanismCollisionNode* node, const ContactInfo& info)
{
	//由机关处理。。。
	node->processCollision(this, info);
}

void LightSpriteNode::moveLight(const Vec2& delta)
{
	//加速度
	this->getPhysicsBody()->applyImpulse((delta)* 5000);
}

void LightSpriteNode::setTag(int tag)
{
	Node::setTag(tag);
	m_LightNode->setTag(tag);
}

void LightSpriteNode::update(float delta)
{
	if (m_isReplaceing)
		return;

	//状态处理
	if (m_HP <= 20)
	{
		//Game Over...
		GameManage::getInstance()->onLostLevel();
		m_isReplaceing = true;
		return;
	}

	Action* action = nullptr;

	switch (m_flag)
	{
	case LightSpriteFlag::General: //常规状态，根据HP的大小实时更新最大光照半径
		m_LightNode->setLightMaxRadius(m_HP / LIGHT_SPRITE_MAX_HP*LIGHT_STANDRAD_RADIUS);
		this->setScale((m_HP / LIGHT_SPRITE_MAX_HP)*m_lightSpriteStandradScale);
		break;
	case LightSpriteFlag::SkillFirst://技能一，扩大光照范围，损耗30HP
		m_flag = LightSpriteFlag::Running;

		action = Sequence::create(
			LightRadiusScaleBy::create(100, 1),
			DelayTime::create(3.0f),
			LightRadiusScaleBy::create(-100 - SkillConsume::FirstSkill, 1),
			CallFunc::create([&]() {
			    m_flag = LightSpriteFlag::General;
			    m_HP -= SkillConsume::FirstSkill;
		    }),
			nullptr);
		m_LightNode->runAction(action);
		break;
	case LightSpriteFlag::SkillSecond://技能二
		break;
	case LightSpriteFlag::SkillThird://技能三
		break;
	default:
		break;
	}

	Node::update(delta);
}

void LightSpriteNode::onEnter()
{
	this->scheduleUpdate();

    Node::onEnter();
}

void LightSpriteNode::onExit()
{
	this->unscheduleUpdate();

    Node::onExit();
}

void LightSpriteNode::setPosition(float x, float y)
{
    Node::setPosition(x, y);

	if (m_LightNode)
	{
        //把物理运算结果赋予光的位置
        m_LightNode->setLightPos(Vec2(x, y));
	}
    adjustLightNode();
}

void LightSpriteNode::setPosition(const cocos2d::Vec2& pos)
{
	setPosition(pos.x, pos.y);
}

void LightSpriteNode::setScale(float scale)
{
    Node::setScale(scale);
    adjustLightNode();
}

void LightSpriteNode::adjustLightNode()
{
    if (m_LightNode)
    {
        m_LightNode->setScale(1.0f / this->getScale());
        //这是把渲染光照的画布一直固定在世界坐标的原点
        Vec2 lightNodePos = this->convertToNodeSpace(Vec2::ZERO);
        m_LightNode->setPosition(lightNodePos);
    }
}
