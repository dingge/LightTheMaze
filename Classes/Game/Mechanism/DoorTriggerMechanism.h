/********************************************************************
	date  :	2016/01/30   14:52

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef DOORTRIGGERMECHANISM_H_
#define DOORTRIGGERMECHANISM_H_

#include "Game/Mechanism/TriggerMechanism.h"
#include "Game/Mechanism/MechanismCollisionNode/StaticMechanism.h"
#include "Game/Extension/QuadTree.h"

/**
*	���Ŵ�������
*/
class SingleDoorTriggerMechanism : public TriggerMechanism
{
public:
    static SingleDoorTriggerMechanism* create(const cocos2d::Vec2& key_pos, const NodeGroupPtr& line_group);
protected:
    SingleDoorTriggerMechanism() :
        door_(nullptr),
        line_group_(nullptr)
    {}
    virtual ~SingleDoorTriggerMechanism() {}

    bool init(const cocos2d::Vec2& key_pos, const NodeGroupPtr& line_group);

    void createDoor();

    void removeDoor();
protected:
    StaticMechanism* door_;
    NodeGroupPtr line_group_;
};

/**
*	���������Ŵ�������
*   �������غ��ſ���
*	����������
*/
class SingleDoorSingleTriggerMechanism : public SingleDoorTriggerMechanism
{
public:
	static SingleDoorSingleTriggerMechanism* create(const cocos2d::Vec2& key_pos, const NodeGroupPtr& line_group);
protected:
	virtual bool openMechanism()override;
};

/**
 *	ѭ���������Ż���
 *  �������غ���򿪣������
 */
class SingleDoorRepeatTriggerMechanism : public SingleDoorTriggerMechanism
{
public:
    static SingleDoorRepeatTriggerMechanism* create(const cocos2d::Vec2& key_pos, const NodeGroupPtr& line_group);
protected:
    SingleDoorRepeatTriggerMechanism() :
        keylock_delay_action_(nullptr) 
    {}
    
    virtual bool openMechanism()override;
    virtual bool closeMechanism()override;

    void runKeyLockDelayAction();
private:
    cocos2d::Action* keylock_delay_action_;
};

#endif // DOORTRIGGERMECHANISM_H_