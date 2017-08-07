#include "GameManage.h"
#include "Game/Light/LightSpriteNode.h"
#include "Game/Light/Action/LightColorAction.h"
#include "Game/Light/LightFragmentNode.h"
#include "Game/Mechanism/LoopMoveMechanism.h"
#include "Game/Mechanism/LoopRotateMechanism.h"
#include "Game/Mechanism/PotalMechainism.h"
#include "Game/Mechanism/DoorTriggerMechanism.h"
#include "Game/GameScene.h"
#include "Setting.h"
#include "Utils.h"
#include "UI/GameUI/PauseScene/PauseScene.h"
#include "UI/GameUI/TipLayer/TipLayer.h"
#include "UI/GameUI/ShowFragmentLayer/FragmentShow.h"
#include "UI/LevelSelectUI/SelectScene.h"

USING_NS_CC;

GameManage::GameManage():
    level_id_(0),
	m_root(nullptr),
	m_playerFirst(nullptr),
	m_playerSecond(nullptr),
	m_mapLayer(nullptr),
	m_effectLayer(nullptr),
	m_beginPosition(0,0),
	m_isBeginTipShow(false),
	m_playerType(PlayerType::FIRST)
{
	createColorMap();
}

GameManage::~GameManage()
{
}

Node* GameManage::loadLevelFromFile(const int level_id)
{
    level_id_ = level_id;
	m_levelName = Setting::getInstance()->getLevelNameById(level_id_);
	
	m_root = Node::create();

	m_mapLayer = Tiled::create(m_levelName);
	m_mapLayer->setPosition(Vec2(0,0));
	m_root->addChild(m_mapLayer);

    ShapeManage::getInstance()->initWithMap(
        Size(m_mapLayer->getMapWidth(), m_mapLayer->getMapHeight()));

	m_effectLayer = EffectLayer::create();
	m_effectLayer->setPosition(Vec2(0,0));
	//m_effectLayer->closeDark();
	m_root->addChild(m_effectLayer);
	log("create effect layer");

    loadMapObject(m_mapLayer->getObjectGroup("map"), m_root);
	log("load map");
	loadNpcObject(m_mapLayer->getObjectGroup("npc"),m_root);
	log("load npc");
	loadItemObject(m_mapLayer->getObjectGroup("item"),m_root);
	log("load item");
	loadMechanismObject(m_mapLayer->getObjectGroup("mechanism"),m_root);
	log("load mechanism");

	updateLightNodeContainer();
	
	m_needFragmentNumber = m_allFragmentVector.size();

	return m_root;
}

void GameManage::loadMapObject(const cocos2d::TMXObjectGroup* mapGroup, cocos2d::Node* root)
{
    std::vector<NodeGroupPtr> group_container;
    auto obj = mapGroup->getObjects();
    if (!obj.empty())
    {
        std::vector<Edge> edge_container;
        //读取对象层形状
        for (const auto& v : obj)
        {
            const ValueMap& dict = v.asValueMap();
            const std::vector<Line>& temp = ShapeManage::getInstance()->createShape(dict, "shape");
            Color4B color = m_colorMap.at(getDictValue(dict, "color", Value("white")).asString());
            color.a = getDictValue(dict, "alpha", Value(255)).asByte();
            for (const auto& segment : temp)
            {
                edge_container.push_back(Edge(segment, color));
            }
        }

        //预处理，分裂相交线
        ShapeManage::getInstance()->calculateCrossPos(&edge_container);

        //重排所有形状
        std::sort(edge_container.begin(), edge_container.end());

        for (const auto& it : edge_container)
        {
            NodeGroupPtr p = ShapeManage::getInstance()->convertToGroup(it, it.getColor());
            ShapeManage::getInstance()->addBody(p);
            group_container.push_back(p);
        }
    }
    m_mapLayer->setStaticShape(group_container);
    log("create all shape");
    m_mapLayer->drawStaticShape();
    log("draw all shape");
}

void GameManage::loadNpcObject( const TMXObjectGroup* npcGroup,Node* root )
{
	auto obj = npcGroup->getObjects();
	if (!obj.empty())
	{
		for (const auto& v : obj)
		{
			const ValueMap& dict = v.asValueMap();
			if (dict.find("type") != dict.end())
			{
				if (dict.at("type").asString() == "player_begin_property")
				{
                    int x = dict.at("x").asInt();
                    int y = dict.at("y").asInt();
                    int radius = dict.at("width").asInt() / 2;
                    m_beginPosition = Vec2(x + radius, y + radius);
					Color4B color = m_colorMap.at(getDictValue(dict, "color", Value("orange")).asString());
					m_playerFirst = LightSpriteNode::create(PARTICLE_LIGHT_SPRITE,color,radius);
					m_playerFirst->setPosition(m_beginPosition);
					m_playerFirst->setTag(TAG_LIGHT_SPRITE);
					m_mapLayer->addChild(m_playerFirst,1);

// 					auto action3 = LightColorActionTo::create(255,0,0,255,1);
// 					auto action4 = LightColorActionTo::create(0,0,255,255,1);
// 					m_playerFirst->getLightNode()->runAction(
// 						RepeatForever::create(Sequence::createWithTwoActions(action3,action4)));

					//加载关卡提示
					if(dict.find("tip_text") != dict.end())
						m_levelTip = dict.at("tip_text").asString();
				}
				else if(dict.at("type").asString() == "dark_property")
				{
					int x = dict.at("x").asInt();
					int y = dict.at("y").asInt();
					int radius = dict.at("width").asInt()/2;
					auto dark = DarkSprite::create(PARTICLE_DARK_SPRITE,radius);
                    dark->setPosition(x + radius, y + radius);
					dark->setTag(TAG_DARK_SPRITE);
					m_mapLayer->addChild(dark,2);
					m_darkVector.push_back(dark);

					if (dict.find("action") != dict.end())
					{
						dark->setNormalAction(loadMoveAction(obj,dict.at("action").asInt()));
						dark->runNormalAction();
					}
				}
			}
		}
	}
}

void GameManage::loadItemObject( const TMXObjectGroup* npcGroup,Node* root )
{
	auto obj = npcGroup->getObjects();
	if (!obj.empty())
	{
		for (const auto& v : obj)
		{
			const ValueMap& dict = v.asValueMap();
			if (dict.find("type") != dict.end())
			{
				if (dict.at("type").asString() == "fragment_property")
				{
					int x = dict.at("x").asInt();
					int y = dict.at("y").asInt();
					int radius = dict.at("width").asInt()/2;
                    Color4B color = m_colorMap.at(getDictValue(dict, "color", Value("blue")).asString());
					//auto fragment = LightFragmentNode::create("item/"+getFragmentPicByColor(color),color,radius);
					auto fragment = LightFragmentNode::create("item/fragment_white.png", color, radius);
                    fragment->setPosition(Vec2(x + radius, y + radius));
					fragment->setTag(TAG_LIGHT_FRAGMENT);
					m_mapLayer->addChild(fragment);
					m_allFragmentVector.push_back(fragment);
				}
				else if(dict.at("type").asString() == "beacon_property")
				{
					m_beacon = BeaconNode::create(BEACON_BG_FILE_NAME,BEACON_PARTICLE_FILE_NAME);
					int x = dict.at("x").asInt();
					int y = dict.at("y").asInt();
					int radius = dict.at("width").asInt()/2;
					m_beacon->setPosition(Vec2(x + radius, y + radius));
					m_mapLayer->addChild(m_beacon);
				}
			}
		}
	}
}

void GameManage::loadMechanismObject( const TMXObjectGroup* mechanismGroup,Node* root )
{
	auto obj = mechanismGroup->getObjects();
	if (!obj.empty())
	{
		for (const auto& v : obj)
		{
			const ValueMap& dict = v.asValueMap();
			if (dict.find("type") != dict.end())
			{
				//移动的无交互机关
				if (dict.at("type").asString() == "move_mechanism")
				{
                    std::vector<Line> mechanismLine = ShapeManage::getInstance()->createShape(dict, "shape");
                    NodeGroupPtr mechanism_group = ShapeManage::getInstance()->convertToGroup(mechanismLine);
                    if (dict.find("action") != dict.end())
                    {
                        int actionName = dict.at("action").asInt();
                        const ValueMap& dic = findDictionaryByName(obj, actionName);
                        float begin_rate = getDictValue(dic, "begin_rate", Value(0.0)).asFloat();
                        float duration = getDictValue(dic, "duration", Value(1.0)).asFloat();
                        bool is_reverse = getDictValue(dic, "is_reverse", Value(true)).asBool();
                        auto path = Path(ShapeManage::getInstance()->createShape(dic, "shape"));
                        LoopMoveMechanism* p = LoopMoveMechanism::create(mechanism_group, begin_rate, path, duration, is_reverse);
                        p->setPosition(Vec2::ZERO);
                        m_mapLayer->addChild(p);
                    }
				}
                //旋转无交互机关
				if(dict.at("type").asString() == "rotate_mechanism")
				{
                    std::vector<Line> mechanismLine = ShapeManage::getInstance()->createShape(dict, "shape");
                    NodeGroupPtr mechanism_group = ShapeManage::getInstance()->convertToGroup(mechanismLine);
					Vec2 centerPos = mechanismLine.begin()->_p1;
					float beginAngle = getDictValue(dict, "begin_angle", Value(0.0)).asFloat();
					float endAngle = getDictValue(dict, "end_angle", Value(360.0)).asFloat();
                    float duration = getDictValue(dict, "duration", Value(1.0)).asFloat();
                    bool isReverse = getDictValue(dict, "is_reverse", Value(true)).asBool();
					
					auto p = LoopRotateMechanism::create(mechanism_group,centerPos,beginAngle,endAngle,duration,isReverse);
					p->setPosition(Vec2::ZERO);
					m_mapLayer->addChild(p);
				}
				//传送阵机关
				if (dict.at("type").asString() == "potal_mechainism_begin")
				{
					int beginName = dict.at("name").asInt();
					int beginX = dict.at("x").asInt();
					int beginY = dict.at("y").asInt();
					int radius = dict.at("width").asInt()/2;
					int endName = dict.at("end_name").asInt();
					const ValueMap& tempDic = findDictionaryByName(obj,endName);
					int endX = tempDic.at("x").asInt();
					int endY = tempDic.at("y").asInt();
					PotalMechainism* p = PotalMechainism::create(
						Vec2(beginX+radius,beginY+radius),Vec2(endX+radius,endY+radius));
					p->setPosition(Vec2::ZERO);
					m_mapLayer->addChild(p);

					m_potalPositionVec.push_back(Vec2(beginX,beginY));
					m_potalPositionVec.push_back(Vec2(endX,endY));
				}
				//一般门机关
				if (dict.at("type").asString() == "mechanism_door")
				{
					int doorName = dict.at("name").asInt();
					int keyName = dict.at("key_name").asInt();
					//找钥匙
					const ValueMap& keyDic = findDictionaryByName(obj,keyName);
					int keyX = keyDic.at("x").asInt();
					int keyY = keyDic.at("y").asInt();
					int radius = keyDic.at("width").asInt()/2;
                    bool is_repeat_trigger = getDictValue(dict, "repeat_trigger", Value(false)).asBool();

					std::vector<Line> doorShpae = ShapeManage::getInstance()->createShape(dict,"shape");
                    NodeGroupPtr door_group = ShapeManage::getInstance()->convertToGroup(doorShpae);
                    Node* door = nullptr;
                    if (is_repeat_trigger)
                    {
                        door = SingleDoorRepeatTriggerMechanism::create(Vec2(keyX + radius, keyY + radius), door_group);
                    }
                    else
                    {
					    door = SingleDoorSingleTriggerMechanism::create(Vec2(keyX+radius,keyY+radius),door_group);
                    }
					door->setPosition(Vec2::ZERO);
					m_mapLayer->addChild(door);
				}
			}
		}
	}
}

Action* GameManage::loadMoveAction( const ValueVector& obj,const int actionName )
{
	for (const auto& v : obj)
	{
		const ValueMap& dict = v.asValueMap();
		if(dict.find("type") != dict.end() && dict.at("type").asString() == "action_property")
		{
			std::string actionType = dict.at("action_type").asString();
			float duration = dict.at("duration").asFloat();
			if (actionType == "move")
			{
				int originX = dict.at("x").asInt();
				int originY = dict.at("y").asInt();
				int lastX = originX,lastY = originY;
				int size = dict.at("polylinePoints").asValueVector().size();
				ActionInterval* move = MoveBy::create(0,Vec2(0,0));
				for(const auto& points : dict.at("polylinePoints").asValueVector())
				{
					const auto vm = points.asValueMap();
					int ix = originX + vm.at("x").asInt() - lastX;
					int iy = originY - vm.at("y").asInt() - lastY;
					move = Sequence::createWithTwoActions(move,MoveBy::create(duration/size,Vec2(ix,iy)));
					lastX = originX + vm.at("x").asInt();
					lastY = originY - vm.at("y").asInt();
				}
				
				return RepeatForever::create(Sequence::createWithTwoActions(move,move->reverse()));
			}
		}
	}
	return nullptr;
}

LightSpriteNode* GameManage::getNowPlayer()
{
	switch (m_playerType)
	{
	case GameManage::FIRST:
		return m_playerFirst;
		break;
	case GameManage::SECOND:
		return m_playerSecond;
		break;
	default:
		break;
	}
	return nullptr;
}

void GameManage::updateMapPositionForLightNode( const Vec2& mappos )
{
	updateLightNodeContainer();
	for (auto it:m_lightNodeVec)
	{
		it->updateMapPosition(mappos);
	}
}

void GameManage::updateLightNodes()
{
	updateLightNodeContainer();
	for (auto it:m_lightNodeVec)
	{
        if (it->isLightInScreen())
        {
		    it->updateLight();
        }
	}
}

void GameManage::createColorMap()
{
	m_colorMap.clear();
	m_colorMap.insert(std::make_pair("red",Color4B::RED));
	m_colorMap.insert(std::make_pair("orange",Color4B::ORANGE));
	m_colorMap.insert(std::make_pair("yellow",Color4B::YELLOW));
	m_colorMap.insert(std::make_pair("green",Color4B::GREEN));
	m_colorMap.insert(std::make_pair("blue",Color4B::BLUE));
	m_colorMap.insert(std::make_pair("magenta",Color4B::MAGENTA));
	m_colorMap.insert(std::make_pair("white",Color4B::WHITE));
	m_colorMap.insert(std::make_pair("gray",Color4B::GRAY));
	m_colorMap.insert(std::make_pair("black",Color4B::BLACK));
}

std::vector<DarkSprite*>& GameManage::getDarkVector()
{
	updateDarkSpriteContainer();
	return m_darkVector;
}

std::vector<LightFragmentNode*>& GameManage::getFragmentVector()
{
	updateFragmentContainer();
	return m_allFragmentVector;
}

std::vector<LightNode*>& GameManage::getLightNodesVector()
{
	updateLightNodeContainer();
	return m_lightNodeVec;
}

void GameManage::updateLightNodeContainer()
{
	m_lightNodeVec.clear();
	for (auto it:m_mapLayer->getChildren())
	{
		if(it->getTag() == TAG_LIGHT_SPRITE)
		{
			m_lightNodeVec.push_back(static_cast<LightSpriteNode*>(it)->getLightNode());
		}
		if ( it->getTag() == TAG_LIGHT_FRAGMENT)
		{
			m_lightNodeVec.push_back(static_cast<LightFragmentNode*>(it)->getLightNode());
		}
	}
}

void GameManage::updateDarkSpriteContainer()
{
	m_darkVector.clear();
	for (auto it:m_mapLayer->getChildren())
	{
		if (it->getTag() == TAG_DARK_SPRITE)
		{
			m_darkVector.push_back(static_cast<DarkSprite*>(it));
		}
	}
}

void GameManage::updateFragmentContainer()
{
	m_collectedFragmentVector.clear();
	for (auto it:m_mapLayer->getChildren())
	{
		if (it->getTag() == TAG_LIGHT_FRAGMENT)
		{
			auto p = static_cast<LightFragmentNode*>(it);
			if (p->isFragmentCollected() == true)
			{
				m_collectedFragmentVector.push_back(p);
			}
			m_allFragmentVector.push_back(p);
		}
	}
}

const ValueMap GameManage::findDictionaryByName( const ValueVector& obj,int name )
{
	for (auto it:obj)
	{
		const ValueMap& dic = it.asValueMap();
		if(dic.at("name").asInt() == name)
		{
			return dic;
		}
	}
	CCAssert(false, "can not find object");
	return ValueMap();
}

void GameManage::onLostLevel()
{
	//Director::getInstance()->purgeCachedData();
	clearResourse();
    Director::getInstance()->replaceScene(GameScene::createScene(level_id_, true));
}

void GameManage::clearResourse()
{
	m_root = nullptr;
	m_playerFirst = nullptr;
	m_playerSecond = nullptr;
	m_darkVector.clear();
	m_collectedFragmentVector.clear();
	m_allFragmentVector.clear();
	m_effectLayer = nullptr;
	m_mapLayer = nullptr;
	m_physicsWorld = nullptr;
	m_beginPosition = Vec2(0,0);
	m_playerType = FIRST;
	m_lightNodeVec.clear();
	m_needFragmentNumber = 0;
	m_isBeginTipShow = false;
	m_potalPositionVec.clear();
	m_levelTip = "";
}

int GameManage::getRemainFragmentNumber()
{
	updateFragmentContainer();
	return m_needFragmentNumber - m_collectedFragmentVector.size();
}

void GameManage::pauseScene()
{
	//Director::getInstance()->stopAnimation();
	Director::getInstance()->pushScene(PauseScene::create(m_root));
}

void GameManage::showTip( const std::string& tip )
{
	auto pauseScene = PauseScene::create(m_root);
	pauseScene->addChild(TipLayer::create(tip));
	Director::getInstance()->pushScene(pauseScene);
}

void GameManage::showTip()
{
	if (m_isBeginTipShow == false)
	{
		auto pauseScene = PauseScene::create(m_root);
		pauseScene->addChild(TipLayer::create(m_levelTip));
		Director::getInstance()->pushScene(pauseScene);
		m_isBeginTipShow = true;
	}
}

void GameManage::showAtlas()
{
	auto pauseScene = PauseScene::create(m_root);
	pauseScene->addChild(FragmentShow::create(m_allFragmentVector));
	Director::getInstance()->pushScene(pauseScene);
}

void GameManage::unlockNextLevel()
{
    int newest_level = Setting::getInstance()->getNewestLevel();
    if (level_id_ == newest_level)
    {
        Setting::getInstance()->setNewestLevel(newest_level + 1);
    }
}

void GameManage::returnToSelectScene()
{
 	Director::getInstance()->replaceScene(TransitionFade::create(1.0f,SelectScene::createScene()));
}

std::string GameManage::getFragmentPicByColor( const Color4B& color )
{
	if (color == Color4B::RED)
		return "fragment_red.png";
	if (color == Color4B::BLUE)
		return "fragment_blue.png";
	if (color == Color4B::GREEN)
		return "fragment_green.png";
	if (color == Color4B::MAGENTA)
		return "fragment_magenta.png";
	if (color == Color4B::YELLOW)
		return "fragment_yellow.png";
	return "fragment_white.png";
}

void GameManage::shineTheMap()
{
	m_mapLayer->shineTheMap();
	m_effectLayer->closeDark();
	for (auto it:m_darkVector)
	{
		it->setCascadeOpacityEnabled(true);
        it->setPhysicsBody(nullptr);
		it->runAction(Sequence::create(
			CallFunc::create([it](){it->setShined(true);}),
			ScaleTo::create(3.0f,0.01f),
			CallFunc::create([it](){it->removeFromParent();}),
			nullptr)
			);
	}
}

void GameManage::onPassLevel()
{
    Action* passAction = Sequence::create(
        DelayTime::create(3.0f),
        CallFunc::create([&]() {showTip("pass!"); }),
        DelayTime::create(1.0f),
        CallFunc::create([&]() {unlockNextLevel(); returnToSelectScene(); }),
        nullptr
        );
    m_root->runAction(passAction);

    shineTheMap();
}

std::string GameManage::getLevelShineMapFileName()
{
	std::string levelShineMapFile = m_levelName;
	int dotpos = levelShineMapFile.find('.');
	levelShineMapFile.erase(dotpos,levelShineMapFile.length()-dotpos);
	levelShineMapFile += "ShineMap.png";
	return levelShineMapFile;
}
