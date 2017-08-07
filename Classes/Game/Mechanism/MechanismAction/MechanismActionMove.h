/********************************************************************
	date  :	2016/03/29   23:21

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef MECHANISMMOVEACTION_H_
#define MECHANISMMOVEACTION_H_

#include "cocos2d.h"
#include "Game/GameUtils/Path.h"
#include "Game/Mechanism/MechanismCollisionNode/MechanismCollisionNode.h"

class MechanismActionMoveBy : public cocos2d::ActionInterval
{
public:
    static MechanismActionMoveBy* create(const Path& path, float duration);

    virtual MechanismActionMoveBy* clone()const override;
    virtual MechanismActionMoveBy* reverse()const override;
    virtual void startWithTarget(cocos2d::Node *target)override;
    virtual void update(float time)override;

    void setMechanismNode(const MechanismCollisionNode& node);
protected:
    MechanismActionMoveBy();
    virtual ~MechanismActionMoveBy();

    bool initWithDuration(const Path& path, float duration);

    float duration_;
    Path path_;
    cocos2d::Vec2 start_pos_;
    cocos2d::Vec2 end_pos_;
    cocos2d::Vec2 pre_pos_;
    MechanismCollisionNode* mechanism_node_;
private:
    MechanismActionMoveBy(const MechanismActionMoveBy &) = delete;
    MechanismActionMoveBy &operator =(const MechanismActionMoveBy &) = delete;
};
#endif // MECHANISMMOVEACTION_H_
