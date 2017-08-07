#include "TriggerMechanism.h"
#include "Game/Mechanism/MechanismCollisionNode/KeyMechanism.h"

USING_NS_CC;

//----------------------------------TriggerMechanism----------------------------------
bool TriggerMechanism::init(const Vec2& keyPos)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!BaseMechanism::init());

		key_ = KeyMechanism::create(KEY_PARTICLE_FILE_NAME);
		key_->setPosition(keyPos);

		this->addChild(key_);

		m_mechanismVector.push_back(key_);

		this->scheduleUpdate();

		bRet = true;
	} while (0);
	return bRet;
}

void TriggerMechanism::update(float delta)
{
	BaseMechanism::update(delta);

	if (is_opened_ == true && key_->getKeyFlag() == KeyFlag::OPEN ||
		is_opened_ == false && key_->getKeyFlag() == KeyFlag::CLOSE ||
		key_->isKeyLock() == true)
		return;

	switch (key_->getKeyFlag())
	{
	case KeyFlag::CLOSE:
		//机关关闭
        if (closeMechanism())
        {
		    is_opened_ = false;
        }
		break;
	case KeyFlag::OPEN:
		//机关开启
        if (openMechanism())
        {
		    is_opened_ = true;
        }
		break;
	default:
		break;
	}
}