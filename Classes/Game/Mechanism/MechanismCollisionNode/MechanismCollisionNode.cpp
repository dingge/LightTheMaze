#include "MechanismCollisionNode.h"

USING_NS_CC;

//----------------------------------MechanismCollisionNode----------------------------------
void MechanismCollisionNode::processCollision(CollisionProcessNode* node, const ContactInfo& info)
{
	node->processCollision(this, info);
}

void MechanismCollisionNode::processCollision(LightSpriteNode* node, const ContactInfo& info)
{
	//log("MechanismCollisionNode...LightSpriteNode collision!");
	//自己处理。。。
}

void MechanismCollisionNode::processCollision(DarkSprite* node, const ContactInfo& info)
{
	//log("MechanismCollisionNode...DarkParticleCollisionNode collision!");
	//自己处理。。。
}

bool MechanismCollisionNode::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!CollisionProcessNode::init());

		bRet = true;
	} while (0);
	return bRet;
}