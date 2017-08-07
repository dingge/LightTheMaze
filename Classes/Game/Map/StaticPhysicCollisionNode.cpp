#include "StaticPhysicCollisionNode.h"

//----------------------------------StaticPhysicCollisionNode----------------------------------

void StaticPhysicCollisionNode::processCollision(CollisionProcessNode* node, const ContactInfo& info)
{
	node->processCollision(this, info);
}
