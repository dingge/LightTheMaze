#include "PotalMechainism.h"
#include "Game/GameManage.h"

USING_NS_CC;

//----------------------------------PotalMechainism----------------------------------
PotalMechainism* PotalMechainism::create( const Vec2& beginPos,const Vec2& endPos )
{
	PotalMechainism *p = new (std::nothrow) PotalMechainism();
	if (p && p->init(beginPos,endPos))
	{
		p->autorelease();
		return p;
	}
	CC_SAFE_DELETE(p);
	return nullptr;
}

bool PotalMechainism::init( const Vec2& beginPos,const Vec2& endPos )
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Node::init());

		m_begin = PotalMechainismCell::create(POTAL_PARTICLE_FILE_NAME);
		m_end = PotalMechainismCell::create(POTAL_PARTICLE_FILE_NAME);
		m_begin->setPosition(beginPos);
		m_end->setPosition(endPos);

		this->addChild(m_begin);
		this->addChild(m_end);

		this->scheduleUpdate();

		bRet  = true;
	} while (0);
	return bRet;
}

void PotalMechainism::update( float delta )
{
	if (m_begin->m_isTrigger == true && m_begin->m_isLeaved == false)
		m_flag = PotalFlag::TRIGGER_BEGIN;
	else if(m_end->m_isTrigger == true && m_end->m_isLeaved == false)
		m_flag = PotalFlag::TRIGGER_END;
	else
		m_flag = PotalFlag::NONE;

	auto player = GameManage::getInstance()->getNowPlayer();
	//获取当前速度，过传送阵要减速
	Vec2 preVelocity = player->getPhysicsBody()->getVelocity();
	preVelocity.normalize();

	switch (m_flag)
	{
	case PotalFlag::NONE:
		break;
	case PotalFlag::TRIGGER_BEGIN:
		player->setPosition(m_end->getPosition());
		player->getPhysicsBody()->setVelocity(preVelocity);
		m_end->resetPosition();
		break;
	case PotalFlag::TRIGGER_END:
		player->setPosition(m_begin->getPosition());
		player->getPhysicsBody()->setVelocity(preVelocity);
		m_begin->resetPosition();
		break;
	default:
		break;
	}

	m_begin->m_isTrigger = m_end->m_isTrigger = false;

	Node::update(delta);
}
