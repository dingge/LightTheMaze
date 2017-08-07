#include "MirrorNode.h"

USING_NS_CC;

MirrorNode::MirrorNode( Line mirrorPos ):
	m_mirrorPosition(mirrorPos)
{
}

MirrorNode::~MirrorNode()
{
}

MirrorNode* MirrorNode::create( Line mirrorPos )
{
	MirrorNode * ret = new MirrorNode(mirrorPos);
	if (ret && ret->init())
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

bool MirrorNode::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Node::init());

		bRet = true;
	} while (0);
	return bRet;
}
