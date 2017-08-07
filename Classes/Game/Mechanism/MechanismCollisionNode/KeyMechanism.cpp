#include "KeyMechanism.h"
#include "Game/Light/LightSpriteNode.h"
#include "Game/Macros.h"

USING_NS_CC;

//----------------------------------KeyMechanism----------------------------------
KeyMechanism* KeyMechanism::create(const std::string& fileName)
{
	KeyMechanism *p = new (std::nothrow) KeyMechanism();
	if (p && p->init(fileName))
	{
		p->autorelease();
		return p;
	}
	CC_SAFE_DELETE(p);
	return nullptr;
}

bool KeyMechanism::init(const std::string& fileName)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!MechanismCollisionNode::init());

		key_particle_ = ParticleSystemQuad::create(fileName);
		//相对位移时才拖尾
		key_particle_->setPositionType(ParticleSystem::PositionType::RELATIVE);
		key_particle_->setPosition(Vec2::ZERO);
		this->addChild(key_particle_);

		//预设一个标准大小
		const float cParticleRadius = 10.0f;

		auto beginbody = PhysicsBody::createCircle(cParticleRadius);
		beginbody->setMass(PHYSICS_INFINITY);
		beginbody->setCollisionBitmask(KEY_COLLISION_BITMASK);
		beginbody->setContactTestBitmask(BODY_CONTACT_TEST_BITMARK);
		this->setPhysicsBody(beginbody);

		this->scheduleUpdate();

		bRet = true;
	} while (0);
	return bRet;
}

void KeyMechanism::processCollision(LightSpriteNode* node, const ContactInfo& info)
{
	if (is_lock_ == true || info.getType() != ContactInfo::ContactBegin)
		return;

	Color4F preColor = key_particle_->getStartColor();

	key_flag_ = (key_flag_ == KeyFlag::OPEN) ? KeyFlag::CLOSE : KeyFlag::OPEN;

	switch (key_flag_)
	{
	case KeyFlag::CLOSE:
		key_particle_->setStartColor(Color4F(
			1 - preColor.r,
			1 - preColor.g,
			1 - preColor.b,
			1 - preColor.a));
		break;
	case KeyFlag::OPEN:
		key_particle_->setStartColor(Color4F(
			1 - preColor.r,
			1 - preColor.g,
			1 - preColor.b,
			1 - preColor.a));
		break;
	default:
		break;
	}
}