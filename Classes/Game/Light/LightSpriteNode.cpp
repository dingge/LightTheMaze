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

		//���ݰ뾶������Ƭ���Ŵ�С��������ߴ�Ϊ��λ����Ϊ���ӽ���޴�С��Ԥ����һ����׼��С32
		m_lightSpriteStandradScale = bodyRadius*2.0f / 32;
		this->setScale(m_lightSpriteStandradScale);

		m_LightNode = LightNode::create();
        m_LightNode->setLightRadius(bodyRadius);
		m_LightNode->setLightColor(Color4F(lightColor));
		m_LightNode->setScale(1 / this->getScale());
		this->addChild(m_LightNode);

        light_particle_ = ParticleSystemQuad::create(pFileName);
        //�þ����˶�ʱ����β
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
	//���塣����
}

void LightSpriteNode::processCollision(LightFragmentNode* node, const ContactInfo& info)
{
	if (node->isFragmentCollected() == false)
	{
		//�˲���ʱ
		//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/CrystalRing.mp3");
		//����Ƭ��Ѫ����
		float add = LIGHT_SPRITE_MAX_HP / Setting::getInstance()->getDifficulty();
		m_HP += add;
		if (m_HP > LIGHT_SPRITE_MAX_HP)
			m_HP = LIGHT_SPRITE_MAX_HP;
	}
	//�������ռ�
	node->setFragmentCollected(true);
}

void LightSpriteNode::processCollision(DarkSprite* node, const ContactInfo& info)
{
	//���������
	const float loss = Setting::getInstance()->getLossVelocity();
	m_HP -= loss;
	auto pDark = node;
	pDark->setHP(pDark->getHP() - loss);
}

void LightSpriteNode::processCollision(MechanismCollisionNode* node, const ContactInfo& info)
{
	//�ɻ��ش�������
	node->processCollision(this, info);
}

void LightSpriteNode::moveLight(const Vec2& delta)
{
	//���ٶ�
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

	//״̬����
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
	case LightSpriteFlag::General: //����״̬������HP�Ĵ�Сʵʱ���������հ뾶
		m_LightNode->setLightMaxRadius(m_HP / LIGHT_SPRITE_MAX_HP*LIGHT_STANDRAD_RADIUS);
		this->setScale((m_HP / LIGHT_SPRITE_MAX_HP)*m_lightSpriteStandradScale);
		break;
	case LightSpriteFlag::SkillFirst://����һ��������շ�Χ�����30HP
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
	case LightSpriteFlag::SkillSecond://���ܶ�
		break;
	case LightSpriteFlag::SkillThird://������
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
        //������������������λ��
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
        //���ǰ���Ⱦ���յĻ���һֱ�̶������������ԭ��
        Vec2 lightNodePos = this->convertToNodeSpace(Vec2::ZERO);
        m_LightNode->setPosition(lightNodePos);
    }
}
