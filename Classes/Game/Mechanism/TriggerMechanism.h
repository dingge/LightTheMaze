/********************************************************************
	date  :	2016/01/30   14:50

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef TRIGGERMECHANISM_H_
#define TRIGGERMECHANISM_H_

#include "Game/Mechanism/BaseMechanism.h"

class KeyMechanism;

/*钥匙精灵粒子文件名*/
#define KEY_PARTICLE_FILE_NAME "particle/keyMechanism.plist"

/**
*  单触发机关类
*  触发机关后开启机关
*/
class TriggerMechanism : public BaseMechanism
{
public:
	virtual void update(float delta)override;
protected:
	TriggerMechanism() :
		key_(nullptr),
		is_opened_(false)
	{}
	virtual ~TriggerMechanism() {}

	bool init(const cocos2d::Vec2& keyPos);
    /**
     *	开启机关，重写以响应该接口
     *  返回值，是否开启成功
     */
    virtual bool openMechanism() { return true; }
    /**
    *	关闭机关，重写以响应该接口
    *   返回值，是否关闭成功
    */
    virtual bool closeMechanism() { return true; }
protected:
	KeyMechanism* key_;
	bool is_opened_; //机关开启标志，当机关完全响应之后设置此状态
};
#endif // TRIGGERMECHANISM_H_