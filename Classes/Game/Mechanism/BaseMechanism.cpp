#include "BaseMechanism.h"

USING_NS_CC;

//----------------------------------BaseMechanism----------------------------------
BaseMechanism::BaseMechanism() :
	m_drawNode(nullptr)
{
}

BaseMechanism::~BaseMechanism()
{
}

bool BaseMechanism::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Node::init());

		m_drawNode = DrawNodeSpec::create();
		m_drawNode->setPosition(Vec2::ZERO);
		this->addChild(m_drawNode);

		this->scheduleUpdate();

		bRet = true;
	} while (0);
	return bRet;
}

void BaseMechanism::update(float delta)
{
	drawMechanism();

	Node::update(delta);
}

void BaseMechanism::drawMechanism()
{
	m_drawNode->clear();
	for (auto i : m_mechanismVector)
	{
        if (i->isVisible() == false)
        {
            continue;
        }
        NodeGroupPtr group_ptr = i->getCurrentShape();
        if (group_ptr.get())
        {
            ListNode<Edge>* p = group_ptr->head_;
            while (p != nullptr)
            {
                m_drawNode->drawSegment(p->_node._p1, p->_node._p2, 3, Color4F(1, 1, 1, 1));
                p = p->_next;
            }
        }
	}
}