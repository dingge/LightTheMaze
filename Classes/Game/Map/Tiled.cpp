#include "Tiled.h"
#include "Game/GameUtils/ShapeManage.h"
#include "Game/Map/StaticPhysicCollisionNode.h"
#include "Game/Effect/FullMapEffect.h"
#include "Game/GameManage.h"

USING_NS_CC;

Tiled::Tiled():
	m_map(nullptr),
	m_wall(nullptr),
	m_staticShapeDraw(nullptr),
#ifdef GAME_DEBUG
	m_debugDraw(nullptr),
#endif // GAME_DEBUG
	m_shineLayer(nullptr)
{
}

Tiled::~Tiled()
{
}

Tiled* Tiled::create( const std::string& tmxName )
{
	Tiled *pRet = new Tiled(); 
	pRet->m_tmxName = tmxName;
	if (pRet && pRet->init()) 
	{ 
		pRet->autorelease(); 
		return pRet; 
	} 
	else 
	{ 
		delete pRet; 
		pRet = NULL; 
		return NULL; 
	} 
}

bool Tiled::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Layer::init());
		m_map = TMXTiledMap::create(m_tmxName);
		//this->addChild(m_map);

		auto floor = m_map->getLayer("floor");
		floor->getTexture()->setAliasTexParameters();

		m_mapWidth = (int)(m_map->getMapSize().width * m_map->getTileSize().width);
		m_mapHeight = (int)(m_map->getMapSize().height * m_map->getTileSize().height);

		m_shineLayer = Layer::create();
		//m_shineLayer = LayerColor::create(Color4B(80,160,170,255), m_mapWidth, m_mapHeight);
		this->addChild(m_shineLayer);

		m_wall = StaticPhysicCollisionNode::create();
		m_wall->setPosition(Vec2(0,0));
		auto edgeBody = PhysicsBody::create();
		edgeBody->setContactTestBitmask(BODY_CONTACT_TEST_BITMARK);
		//不能设为静态body，发现在此版本中静态body无法动态添加shape
		//edgeBody->setDynamic(false);
		m_wall->setPhysicsBody(edgeBody);
		this->addChild(m_wall);

		m_staticShapeDraw = DrawNodeSpec::create();
		this->addChild(m_staticShapeDraw);

#ifdef GAME_DEBUG
		m_debugDraw = DrawNode::create();
		this->addChild(m_debugDraw);
#endif // GAME_DEBUG

		bRet = true;
	} while (0);
	return bRet;
}

TMXObjectGroup* Tiled::getObjectGroup( const std::string& groupName )
{
	if (m_map != nullptr)
		return m_map->getObjectGroup(groupName);
	return nullptr;
}

void Tiled::updatePhysicBody()
{
// 	//用set防止重复元素包含
// 	std::set<Line> shape;
// 	//获取所有非地形body周围的障碍形状
// 	auto childarray = this->getChildren();
// 	for (auto it:childarray)
// 	{
// 		if(it == m_wall)
// 			continue;
// 		auto pBody = it->getPhysicsBody();
// 		if (pBody && pBody->isResting() == false)
// 		{
// 			Vec2 pos = it->getPosition();
// 			auto pCircleShape = dynamic_cast<PhysicsShapeCircle*>(pBody->getShape(0));
// 			if(pCircleShape)
// 			{
// 				float radius = pCircleShape->getRadius();
//                 auto temp = ShapeManage::getInstance()->getNearbyLine(pos, radius + 10);
// 				for (auto j:temp)
// 				{
// 					shape.insert(j);
// 				}
// 			}
// 		}
// 	}
// 
// 	//这里不重建障碍，而是修改，因为障碍在每一帧的变化极小
// 	auto edgebody = m_wall->getPhysicsBody();
// 	//删除已失效障碍
// 	auto tempShapesVector = edgebody->getShapes();
// 	for (auto it = tempShapesVector.begin();it != tempShapesVector.end();++it)
// 	{
// 		if (std::find_if(shape.begin(), shape.end(), [&it](const Line& l){return l._id == (*it)->getTag(); }) == shape.end())
// 		{
// 			edgebody->removeShape(*it);
// 		}
// 	}
// 	//添加新入障碍
// 	auto& shapesVectorRef = edgebody->getShapes();
// 	for (auto it:shape)
// 	{
// 		if ((it._id>>16) >= MECHANISM_NAME_MIN)
// 			continue;
// 		if(std::find_if(shapesVectorRef.begin(),shapesVectorRef.end(),[&it](PhysicsShape* s){return s->getTag() == it._id;}) != shapesVectorRef.end())
// 			continue;
// 		auto tshape = PhysicsShapeEdgeSegment::create(it._p1,it._p2);
// 		tshape->setTag(it._id);
// 		edgebody->addShape(tshape);
// 	}
// 
// 	m_wall->setPhysicsBody(edgebody);
}

void Tiled::drawStaticShape()
{
	m_staticShapeDraw->clear();
	for (auto it: m_group_container)
	{
        ListNode<Edge>* p = it->head_;
        while (p != nullptr)
        {
            m_staticShapeDraw->drawSegment(p->_node._p1, p->_node._p2, 2, Color4F(p->_node.getColor()));

            auto tshape = PhysicsShapeEdgeSegment::create(p->_node._p1, p->_node._p2);
            tshape->setTag(p->_node._id);
            m_wall->getPhysicsBody()->addShape(tshape);

            p = p->_next;
        }
	} 
}

void Tiled::setStaticShape(const std::vector<NodeGroupPtr>& group_container)
{
    m_group_container = group_container;
}

PhysicsBody* Tiled::getMapPhysicsBody()
{
	return m_wall->getPhysicsBody();
}

void Tiled::shineTheMap()
{
	auto p = Sprite::create(GameManage::getInstance()->getLevelShineMapFileName());
	if (p)
	{
		p->setPosition(Vec2(m_mapWidth / 2.0f, m_mapHeight / 2.0f));
		m_shineLayer->addChild(p);
	}

// 	auto p = FullMapEffect::create(m_mapWidth,m_mapHeight);
// 	//auto p  =Sprite::create("mappath.png");
// 	p->setAnchorPoint(Vec2::ZERO);
// 	p->setPosition(Vec2::ZERO);
// 	this->addChild(p);
}

#ifdef GAME_DEBUG
void Tiled::drawDebugShapeOnMap(const std::vector<Line>& lineVec)
{
	m_debugDraw->clear();
	for (const auto& it : lineVec)
	{
		m_debugDraw->drawSegment(it._p1, it._p2, 5, Color4F::GREEN);
	}
}
#endif // GAME_DEBUG