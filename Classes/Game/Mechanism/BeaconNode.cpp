#include "BeaconNode.h"
#include "Game/Macros.h"
#include "Game/GameManage.h"
#include "Game/GameUtils/GeneralCircleBody.h"

USING_NS_CC;

BeaconNode::BeaconNode():
	m_bg(nullptr),
	m_particle(nullptr),
	m_isLitted(false)
{
}

BeaconNode::~BeaconNode()
{
}

BeaconNode* BeaconNode::create(const std::string& bgFileName,const std::string& particleFileName)
{
	BeaconNode *ret = new BeaconNode();
	if (ret && ret->init(bgFileName,particleFileName))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return ret;
}

bool BeaconNode::init(const std::string& bgFileName,const std::string& particleFileName)
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!MechanismCollisionNode::init());

		m_particleFileName = particleFileName;

		//只预加载背景图
		m_bg = Sprite::create(bgFileName);
		m_bg->setPosition(Vec2::ZERO);
		this->addChild(m_bg);

		//预设一个标准大小
		const float cParticleRadius = m_bg->getContentSize().width/2;

		auto beginbody = GeneralCircleBody::createCircleBody(
			cParticleRadius, PHYSICS_INFINITY, 0.0f, 0.3f, 0xffffffff, BODY_CONTACT_TEST_BITMARK, KEY_COLLISION_BITMASK);
		this->setPhysicsBody(beginbody);

		bRet = true;
	} while (0);
	return bRet;
}

void BeaconNode::lit()
{
	m_particle = ParticleSystemQuad::create(m_particleFileName);
	m_particle->setScale(0.1f);
	m_particle->setPosition(Vec2(0.0f,m_bg->getContentSize().height*0.4f));
	m_particle->setPositionType(ParticleSystem::PositionType::RELATIVE);
	this->addChild(m_particle);

    //渐渐放大
    ActionInterval* litAction = ScaleTo::create(2.0f, 1.0f);
    m_particle->runAction(litAction);

    GameManage::getInstance()->onPassLevel();
}

void BeaconNode::extinguish()
{
	//缩小后粒子消失
	Action* extinguishAction = Sequence::createWithTwoActions(
		ScaleTo::create(2.0f,0.1f),
		CallFunc::create([&](){
			m_particle->removeFromParent();
		})
	);
	this->runAction(extinguishAction);
}

void BeaconNode::processCollision( LightSpriteNode* node, const ContactInfo& info )
{
	//光精灵与灯塔碰撞
	//自己处理
	if(GameManage::getInstance()->getRemainFragmentNumber() == 0 && m_isLitted == false)
	{
		lit();
		m_isLitted = true;
	}
}
