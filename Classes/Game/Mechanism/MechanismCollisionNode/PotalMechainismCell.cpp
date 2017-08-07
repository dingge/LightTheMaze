#include "PotalMechainismCell.h"
#include "Game/Macros.h"
#include "Game/GameManage.h"

USING_NS_CC;

//----------------------------------PotalMechainismCell----------------------------------
PotalMechainismCell* PotalMechainismCell::create(const std::string& fileName)
{
	PotalMechainismCell *p = new (std::nothrow) PotalMechainismCell();
	if (p && p->init(fileName))
	{
		p->autorelease();
		return p;
	}
	CC_SAFE_DELETE(p);
	return nullptr;
}

bool PotalMechainismCell::init(const std::string& fileName)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!MechanismCollisionNode::init());

		m_particle = ParticleSystemQuad::create(fileName);
		m_particle->setPositionType(ParticleSystem::PositionType::RELATIVE);
		m_particle->setPosition(Vec2::ZERO);
		this->addChild(m_particle);

		//预设一个标准大小
		const float cParticleRadius = 10.0f;

		auto beginbody = PhysicsBody::createCircle(cParticleRadius);
		beginbody->setMass(PHYSICS_INFINITY);
		beginbody->setCollisionBitmask(POTAL_COLLISION_BITMASK);
		beginbody->setContactTestBitmask(BODY_CONTACT_TEST_BITMARK);
		this->setPhysicsBody(beginbody);

		this->scheduleUpdate();

		bRet = true;
	} while (0);
	return bRet;
}

void PotalMechainismCell::processCollision(LightSpriteNode* node, const ContactInfo& info)
{
    if (info.getType() == ContactInfo::ContactType::ContactBegin)
    {
        if (m_isLeaved == false)
        {
            m_isLeaved = true;
            m_isTrigger = true;
        }
    }
}

void PotalMechainismCell::update(float delta)
{
	Vec2 currentPos = GameManage::getInstance()->getNowPlayer()->getPosition();
	Vec2 particlePos = this->getPosition();

	if ((currentPos - particlePos).getLengthSq() > (m_prePos - particlePos).getLengthSq())
		m_isLeaved = true;
	else
		m_isLeaved = false;

	m_prePos = currentPos;

	MechanismCollisionNode::update(delta);
}

void PotalMechainismCell::resetPosition()
{
	m_prePos = GameManage::getInstance()->getNowPlayer()->getPosition();
}