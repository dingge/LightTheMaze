#include "DarkSprite.h"
#include "Game/GameUtils/GeneralCircleBody.h"
#include "Game/Light/LightSpriteNode.h"
#include "Game/Mechanism/MechanismCollisionNode/MechanismCollisionNode.h"
#include "Setting.h"

USING_NS_CC;

//----------------------------------DarkSprite----------------------------------
DarkSprite::DarkSprite():
    partticle_node_(nullptr),
	m_flag(DarkSpriteFlag::Normal),
	m_pNormalAction(nullptr),
	m_currentAction(nullptr),
	m_rate(5.0f),
	m_darkSpriteStandradScale(1.0f),
	m_maxHP(DARK_SPRITE_STANDARD_HP),
	m_HP(DARK_SPRITE_STANDARD_HP),
	m_isShined(false)
{
}

DarkSprite::~DarkSprite()
{
	setNormalAction(nullptr);
}

DarkSprite* DarkSprite::create( const std::string& filename,float radius )
{
	DarkSprite *ret = new DarkSprite();
	if (ret && ret->init(filename,radius))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return ret;
}

bool DarkSprite::init( const std::string& filename,float radius )
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!CollisionProcessNode::init());
		//根据半径调整精灵缩放大小，以32像素为单位
		m_darkSpriteStandradScale = radius*2.0f/32;
		this->setScale(m_darkSpriteStandradScale);

		//调整正确的HP
		m_maxHP = m_HP *= m_darkSpriteStandradScale;

        partticle_node_ = ParticleSystemQuad::create(filename);
        //相对位移时才拖尾
        partticle_node_->setPositionType(ParticleSystem::PositionType::RELATIVE);
        partticle_node_->setPosition(Vec2::ZERO);
        this->addChild(partticle_node_);

		PhysicsBody* pBody = GeneralCircleBody::createCircleBody(
			radius, 0.2f, 0.0f, 0.3f, 0xffffffff, DARK_BODY_CONTACT_TEST_BITMARK, 0xffffffff);
		pBody->setVelocityLimit(Setting::getInstance()->getDarkSpriteRateLimit());
		this->setPhysicsBody(pBody);

        scheduleUpdate();

		bRet = true;
	} while (0);
	return bRet;
}

void DarkSprite::update( float delta )
{
	//优先执行逃离动作
    PhysicsBody* body = this->getPhysicsBody();
	if (body && 0 != (m_flag & (int)DarkSpriteFlag::Escape))
	{
		body->applyImpulse(-(m_escapeTargetPos - this->getPosition()).getNormalized()*m_rate*500.0f);
	}
	else if (body && 0 != (m_flag & (int)DarkSpriteFlag::Attack))
	{
        body->applyImpulse((m_attackTargetPos - this->getPosition()).getNormalized()*m_rate*500.0f);
	}
	
	//根据HP更新暗精灵半径
	if (m_HP <= 0)
	{
		//生命值清空则消失
		this->removeFromParent();
		return;
	}
	if(m_isShined == false)
		this->setScale(m_HP/m_maxHP*m_darkSpriteStandradScale);

	CollisionProcessNode::update(delta);
}

void DarkSprite::setNormalAction(cocos2d::Action * action)
{
	if (action)
	{
		action->retain();
	}
	if (m_pNormalAction)
	{
		m_pNormalAction->release();
	}
	m_pNormalAction = action;
}

void DarkSprite::stopNormalAction()
{
	if (m_currentAction)
	{
		this->stopAction(m_currentAction);
		m_currentAction = nullptr;
	}
}

void DarkSprite::runNormalAction()
{
	if (m_currentAction && m_currentAction != m_pNormalAction)
	{
		this->stopAction(m_currentAction);
	}
	else if (m_pNormalAction && m_currentAction != m_pNormalAction)
	{
		this->runAction(m_pNormalAction);
	}
	m_currentAction = m_pNormalAction;
}

void DarkSprite::attackTarget(const cocos2d::Vec2& targetPos)
{
 	m_flag = m_flag | DarkSpriteFlag::Attack;
	m_attackTargetPos = targetPos;
	stopNormalAction();
}

void DarkSprite::lostAttackTarget()
{
	m_flag = m_flag & (~DarkSpriteFlag::Attack);
}

void DarkSprite::escapeTarget(const cocos2d::Vec2 & targetPos)
{
	m_flag = m_flag | DarkSpriteFlag::Escape;
	m_escapeTargetPos = targetPos;
	stopNormalAction();
}

void DarkSprite::lostEscapeTarrget(const cocos2d::Vec2& targetPos)
{
	if ((targetPos - m_escapeTargetPos).getLengthSq() < 0.1f)
	{
		m_flag = m_flag & (~DarkSpriteFlag::Escape);
	}
}

void DarkSprite::processCollision(CollisionProcessNode* node, const ContactInfo& info)
{
    node->processCollision(this, info);
}

void DarkSprite::processCollision(LightSpriteNode* node, const ContactInfo& info)
{
    //log("DarkParticleCollisionNode...LightSpriteNode collision!");
    //暂时交给光精灵处理。。。
    node->processCollision(this, info);
}

void DarkSprite::processCollision(MechanismCollisionNode* node, const ContactInfo& info)
{
    //log("DarkParticleCollisionNode...MechanismCollisionNode collision!");
    //交给机关处理
    node->processCollision(this, info);
}
