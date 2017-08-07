#include "DoorTriggerMechanism.h"
#include "Game/GameUtils/ShapeManage.h"
#include "Game/Mechanism/MechanismCollisionNode/KeyMechanism.h"

USING_NS_CC;

SingleDoorTriggerMechanism * SingleDoorTriggerMechanism::create(const cocos2d::Vec2 & key_pos, const NodeGroupPtr & line_group)
{
    SingleDoorTriggerMechanism *p = new (std::nothrow) SingleDoorTriggerMechanism();
    if (p && p->init(key_pos, line_group))
    {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool SingleDoorTriggerMechanism::init(const cocos2d::Vec2 & key_pos, const NodeGroupPtr & line_group)
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!TriggerMechanism::init(key_pos));

        line_group_ = line_group;

        createDoor();

        bRet = true;
    } while (0);
    return bRet;
}

void SingleDoorTriggerMechanism::createDoor()
{
    if (door_ || line_group_ == nullptr)
    {
        return;
    }

    door_ = StaticMechanism::create(line_group_);
    door_->setPosition(Vec2::ZERO);

    this->addChild(door_);

    m_mechanismVector.push_back(door_);
}

void SingleDoorTriggerMechanism::removeDoor()
{
    if (door_ == nullptr)
        return;

    m_mechanismVector.erase(std::remove(m_mechanismVector.begin(), m_mechanismVector.end(), door_));
    door_->removeFromParent();
    door_ = nullptr;
}

SingleDoorSingleTriggerMechanism* SingleDoorSingleTriggerMechanism::create(const Vec2& key_pos, const NodeGroupPtr& line_group)
{
    SingleDoorSingleTriggerMechanism *p = new (std::nothrow) SingleDoorSingleTriggerMechanism();
    if (p && p->init(key_pos, line_group))
    {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool SingleDoorSingleTriggerMechanism::openMechanism()
{
    removeDoor();

    //ËøËÀ»ú¹Ø
    key_->setKeyLock(true);

    return true;
}

SingleDoorRepeatTriggerMechanism * SingleDoorRepeatTriggerMechanism::create(const cocos2d::Vec2 & key_pos, const NodeGroupPtr & line_group)
{
    SingleDoorRepeatTriggerMechanism *p = new (std::nothrow) SingleDoorRepeatTriggerMechanism();
    if (p && p->init(key_pos, line_group))
    {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool SingleDoorRepeatTriggerMechanism::openMechanism()
{
    removeDoor();

    runKeyLockDelayAction();

    return true;
}

bool SingleDoorRepeatTriggerMechanism::closeMechanism()
{
    createDoor();

    runKeyLockDelayAction();

    return true;
}

void SingleDoorRepeatTriggerMechanism::runKeyLockDelayAction()
{
    if (keylock_delay_action_)
    {
        this->stopAction(keylock_delay_action_);
        keylock_delay_action_ = nullptr;
    }

    key_->setKeyLock(true);
    keylock_delay_action_ = Sequence::createWithTwoActions(
        DelayTime::create(1.0f),
        CallFunc::create([this]() { 
        key_->setKeyLock(false); 
        keylock_delay_action_ = nullptr; 
    })
    );
    this->runAction(keylock_delay_action_);
}
