/********************************************************************
	date  :	2016/01/30   15:25

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef KEYMECHANISM_H_
#define KEYMECHANISM_H_

#include "Game/Mechanism/MechanismCollisionNode/MechanismCollisionNode.h"

class LightSpriteNode;

/*机关开关状态*/
enum class KeyFlag
{
	CLOSE,
	OPEN
};

/**
*	钥匙类(开机关的)
*  包含一个粒子精灵
*/
class KeyMechanism : public MechanismCollisionNode
{
public:
	static KeyMechanism* create(const std::string& fileName);

	//开关触发
	virtual void processCollision(LightSpriteNode* node, const ContactInfo& info)override;

	const KeyFlag getKeyFlag()const { return key_flag_; }
	//是否锁机关
	void setKeyLock(bool isLock) { is_lock_ = isLock; }
	inline bool isKeyLock()const { return is_lock_; }
protected:
	KeyMechanism() :
		key_particle_(nullptr),
		key_flag_(KeyFlag::CLOSE),
		is_lock_(false)
	{}

	virtual ~KeyMechanism() {}

	bool init(const std::string& fileName);
private:
	cocos2d::ParticleSystemQuad* key_particle_;
	KeyFlag key_flag_;
	bool is_lock_;
};
#endif // KEYMECHANISM_H_