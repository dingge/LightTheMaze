#include "GameScene.h"
#include "Game/GameManage.h"
#include "UI/LevelSelectUI/SelectScene.h"
#include "Setting.h"

#ifdef GAME_DEBUG && CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include "Test/TestTimeElapsed.h"
#endif

USING_NS_CC;

GameScene::GameScene(void):
	m_joyStick(nullptr)
{
}

GameScene::~GameScene(void)
{
}

bool GameScene::init(int level_id)
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Scene::init());
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();
		
		//��Ӽ����¼�����
		auto keyboardlistener = EventListenerKeyboard::create();
		keyboardlistener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed,this);
		keyboardlistener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased,this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardlistener,this);

		//���ô��������¼�
		auto touchListener = EventListenerTouchOneByOne::create();
		touchListener->onTouchBegan     = CC_CALLBACK_2(GameScene::onTouchBegan,this);
		touchListener->onTouchMoved     = CC_CALLBACK_2(GameScene::onTouchMoved,this);
		touchListener->onTouchEnded     = CC_CALLBACK_2(GameScene::onTouchEnded,this);
		touchListener->onTouchCancelled = CC_CALLBACK_2(GameScene::onTouchCancelled,this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener,this);

		//����������¼��������ԣ�
// 		auto mouselistener = EventListenerMouse::create();
// 		mouselistener->onMouseScroll = [&](Event* event){
// 			auto e = static_cast<EventMouse*>(event);
// 			CCLOG("scrollX:%f,scrollY:%f",e->getScrollX(),e->getScrollY());
// 			float rate = GameManage::getInstance()->getMap()->getScale()+(-0.1)*e->getScrollY();
// 			GameManage::getInstance()->getMap()->setScale(rate);
// 		};
// 		_eventDispatcher->addEventListenerWithSceneGraphPriority(mouselistener,this);

		//�����ײ�¼�����
		auto contactlistener = EventListenerPhysicsContact::create();
		contactlistener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin,this);
		contactlistener->onContactPreSolve = CC_CALLBACK_2(GameScene::onContactPreSolve,this);
		contactlistener->onContactPostSolve = CC_CALLBACK_2(GameScene::onContactPostSolve,this);
		contactlistener->onContactSeparate = CC_CALLBACK_1(GameScene::onContactSeperate, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(contactlistener,this);

		this->setTag(GAME_SCENE_TAG);

		//��յ���
		GameManage::getInstance()->clearResourse();

		//������������ָ��
		GameManage::getInstance()->setPhysicsWorld(this->getPhysicsWorld());

		this->addChild(GameManage::getInstance()->loadLevelFromFile(level_id));

		//����ҡ��
		m_joyStick = Joystick::create("joystick_bg.png",100,"joystick_handle.png",50);
		m_joyStick->setHandleEventListener([&](float interval,float x, float y,JoystickEventType type){
			processPlayerMove(Vec2(x,y));
		});
		m_joyStick->setPosition(200,200);
		m_joyStick->setGlobalZOrder(GLOBAL_Z_ORDER_JOYSTICK);
		m_joyStick->setVisible(false);
		this->addChild(m_joyStick);

		//����update��ⰴ����ס�¼�
		this->scheduleUpdate();

		//��������̨����
		//Director::getInstance()->getConsole()->listenOnTCP(5678);
		Action* showTip = Sequence::createWithTwoActions(
			DelayTime::create(0.0f),
			CallFunc::create([&](){
				GameManage::getInstance()->showTip();
		})
			);
		this->runAction(showTip);

		bRet = true;
	} while (0);
	return bRet;
}

void GameScene::onKeyReleased( EventKeyboard::KeyCode keyCode, Event* event )
{
	//�ͷŰ�ť����vector��ɾ��
	m_pressKeyVec.erase(std::remove(m_pressKeyVec.begin(),m_pressKeyVec.end(),keyCode),m_pressKeyVec.end());

	//�˳�
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE || keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE)
	{
		Director::getInstance()->replaceScene(SelectScene::createScene());
#ifdef GAME_DEBUG && CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
        TestTimeElapsed::getInstance()->dump();
#endif
		event->stopPropagation();
	}
	//1����һ��������շ�Χ
	if (keyCode == EventKeyboard::KeyCode::KEY_1)
	{
		//GameManage::getInstance()->getNowPlayer()->getLightNode()->scaleLightRadiusBy(100,1.0f);
		GameManage::getInstance()->getNowPlayer()->setLightSpriteFlag(LightSpriteFlag::SkillFirst);
	}
	//2���ܶ�
	if (keyCode == EventKeyboard::KeyCode::KEY_2)
	{
		//GameManage::getInstance()->getNowPlayer()->getLightNode()->scaleLightRadiusBy(-100,1.0f);
		GameManage::getInstance()->getNowPlayer()->setLightSpriteFlag(LightSpriteFlag::SkillSecond);
	}
	//3������
	if (keyCode == EventKeyboard::KeyCode::KEY_3)
	{
		GameManage::getInstance()->getNowPlayer()->setLightSpriteFlag(LightSpriteFlag::SkillThird);
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_R)
	{
		GameManage::getInstance()->onLostLevel();
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_P)
	{
		//GameManage::getInstance()->showTip();
		utils::captureScreen([](bool b,const std::string& name){},"screenshot"+getSystemTime()+".png");
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_M)
	{
		GameManage::getInstance()->showAtlas();
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_F2)
	{
		GameManage::getInstance()->shineTheMap();
	}
	//F1�������������Debugģʽ
	if (keyCode == EventKeyboard::KeyCode::KEY_F1)
	{
		auto physicWorld = this->getPhysicsWorld();
		if(physicWorld->getDebugDrawMask() == PhysicsWorld::DEBUGDRAW_NONE)
			this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
		else if(physicWorld->getDebugDrawMask() == PhysicsWorld::DEBUGDRAW_ALL)
			this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
	}
}

void GameScene::onKeyPressed( EventKeyboard::KeyCode keyCode, Event* event )
{
	//���°�ť,ѹ��vector
	m_pressKeyVec.push_back(keyCode);
}

void GameScene::update( float delta )
{
	auto* nowPlayer = GameManage::getInstance()->getNowPlayer();
	EffectLayer* effectLayer = GameManage::getInstance()->getEffectLayer();
	Tiled* mapLayer = GameManage::getInstance()->getMap();

	//�����¼����������ס
	for(auto it : m_pressKeyVec)
	{
		float moveRate = 1.0;
		if(it == EventKeyboard::KeyCode::KEY_UP_ARROW)
		{
			processPlayerMove(Vec2(0,moveRate));
		}
		if(it == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
		{
			processPlayerMove(Vec2(0,-moveRate));
		}
		if(it == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
		{
			processPlayerMove(Vec2(-moveRate,0));
		}
		if(it == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
		{
			processPlayerMove(Vec2(moveRate,0));
		}
		if (it == EventKeyboard::KeyCode::KEY_SPACE)
		{
			GameManage::getInstance()->getNowPlayer()->getPhysicsBody()->setVelocity(Vec2::ZERO);
		}
	}
	
	//���¹��մ����ϰ�
	GameManage::getInstance()->updateMapPositionForLightNode(mapLayer->getPosition());
	GameManage::getInstance()->updateLightNodes();

	//���µ�ͼ��ײ�ϰ�
	mapLayer->updatePhysicBody();
	
	//���¹��պ�����
	effectLayer->drawEffectBegin();
    for (auto it : GameManage::getInstance()->getLightNodesVector())
    {
        if (it->isLightInScreen())
        {
		    effectLayer->addLightEffect(it->getRenderTexture());
        }
    }
	effectLayer->drawEffectEnd();

	//�ƶ���ͼ��ģ�⸩�Ӿ�ͷ�����߽����ƶ���ͼ��
	Vec2 nowLightPos = nowPlayer->getLightNode()->getLightPos();
	Size winsize = Director::getInstance()->getWinSize();
	Vec2 map_current_pos = mapLayer->getPosition();
	Vec2 map_target_pos = Vec2::ZERO;
	if (nowLightPos.x < winsize.width / 2)
		map_target_pos.x = 0.0f;
	else if (nowLightPos.x > mapLayer->getMapWidth() - winsize.width / 2)
		map_target_pos.x = winsize.width - mapLayer->getMapWidth();
	else
		map_target_pos.x = winsize.width / 2 - nowLightPos.x;
	
	if (nowLightPos.y < winsize.height / 2)
		map_target_pos.y = 0.0f;
	else if (nowLightPos.y > mapLayer->getMapHeight() - winsize.height / 2)
		map_target_pos.y = winsize.height - mapLayer->getMapHeight();
	else
		map_target_pos.y = winsize.height / 2 - nowLightPos.y;
	//ƽ���ƶ���ͷ
	map_current_pos.smooth(map_target_pos, delta, 0.5f);
	mapLayer->setPosition(map_current_pos);

	//������׷��
	auto& darkVector = GameManage::getInstance()->getDarkVector();
	auto& lightVector = GameManage::getInstance()->getLightNodesVector();
	int darkVectorLen = darkVector.size();
	int lightVectorLen = lightVector.size();
	for (int i = 0;i < lightVectorLen;++i)
	{
		if (lightVector[i]->getTag() == TAG_LIGHT_SPRITE)
		{
			for (int j = 0; j < darkVectorLen; ++j)
			{
				if (lightVector[i]->isPointInLight(mapLayer->convertToWorldSpace(darkVector[j]->getPosition()),
					mapLayer->convertToWorldSpace(lightVector[i]->getLightPos())))
				{
					darkVector[j]->attackTarget(lightVector[i]->getLightPos());
				}
				else
				{
					darkVector[j]->lostAttackTarget();
				}
			}
		}
		else
		{
			for (int j = 0; j < darkVectorLen; ++j)
			{
				if (lightVector[i]->isPointInLight(mapLayer->convertToWorldSpace(darkVector[j]->getPosition()),
					mapLayer->convertToWorldSpace(lightVector[i]->getLightPos())))
				{
					darkVector[j]->escapeTarget(lightVector[i]->getLightPos());
				}
				else
				{
					darkVector[j]->lostEscapeTarrget(lightVector[i]->getLightPos());
				}
			}
		}
	}

	Scene::update(delta);
}

GameScene* GameScene::createScene( int level_id, bool isUsePhysic /*= false*/ )
{
	if (isUsePhysic == true)
	{
		GameScene *ret = new GameScene();
		if (ret && ret->initWithPhysics())
		{
			ret->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
			ret->getPhysicsWorld()->setGravity(Vect(0,0));
			if (ret->init(level_id))
			{
				ret->autorelease();
				return ret;
			}
			return nullptr;
		}
		else
		{
			CC_SAFE_DELETE(ret);
			return nullptr;
		}
	}
	else
	{
		return GameScene::create(level_id);
	}
}

bool GameScene::onContactBegin( PhysicsContact& contact )
{
	//log("contact begin...");
	Node* nodeA = contact.getShapeA()->getBody()->getNode();
	Node* nodeB = contact.getShapeB()->getBody()->getNode();

	//�ڰ�������ڰ����鲻������ײ���Ҳ������κνӴ��ź�
	if ( (nodeA->getTag() | nodeB->getTag()) == TAG_DARK_SPRITE)
	{
		return false;
	}
	else
	{
		CollisionProcessNode* shapeA = static_cast<CollisionProcessNode*>(nodeA);
		CollisionProcessNode* shapeB = static_cast<CollisionProcessNode*>(nodeB);
		shapeA->processCollision(shapeB, ContactInfo(ContactInfo::ContactType::ContactBegin));
		return true;
	}
}

bool GameScene::onContactPreSolve( PhysicsContact& contact, PhysicsContactPreSolve& solve )
{
	//log("contact pre...");
	Node* nodeA = contact.getShapeA()->getBody()->getNode();
	Node* nodeB = contact.getShapeB()->getBody()->getNode();

	if(nodeA == nullptr || nodeB == nullptr)
		return false;

	int tag = nodeA->getTag() | nodeB->getTag();
	//�ڰ�������player������ʵ��������ײ�����ǻ��Ⲣ���������Ӵ��ź�
	if(tag == (TAG_DARK_SPRITE | TAG_LIGHT_SPRITE))
	{
		CollisionProcessNode* shapeA = static_cast<CollisionProcessNode*>(nodeA);
		CollisionProcessNode* shapeB = static_cast<CollisionProcessNode*>(nodeB);
		shapeA->processCollision(shapeB, ContactInfo(ContactInfo::ContactType::ContactPreSolve));
		return false;
	}
	return true;
}

void GameScene::onContactPostSolve( PhysicsContact& contact, const PhysicsContactPostSolve& solve )
{
    Node* nodeA = contact.getShapeA()->getBody()->getNode();
    Node* nodeB = contact.getShapeB()->getBody()->getNode();


    Vec2 impulse = solve.getImpulse()/ 5000.0f;

    CollisionProcessNode* shapeA = static_cast<CollisionProcessNode*>(nodeA);
    CollisionProcessNode* shapeB = static_cast<CollisionProcessNode*>(nodeB);
    shapeA->processCollision(shapeB, ContactInfo(ContactInfo::ContactType::ContactPostSolve, impulse));
}

void GameScene::onContactSeperate( PhysicsContact& contact )
{
	//log("contact seperate...");
}

GameScene* GameScene::create(int level_id)
{
	GameScene *p = new (std::nothrow) GameScene();
	if (p && p->init(level_id))
	{
		p->autorelease();
		return p;
	}
	CC_SAFE_DELETE(p);
	return nullptr;
}

void GameScene::processPlayerMove( const Vec2& impulse )
{
	auto* nowPlayer = GameManage::getInstance()->getNowPlayer();
	nowPlayer->moveLight(impulse);
}

bool GameScene::onTouchBegan( Touch *pTouch, Event *pEvent )
{
	return true;
}

void GameScene::onTouchMoved( Touch *pTouch, Event *pEvent )
{
}

void GameScene::onTouchEnded( Touch *pTouch, Event *pEvent )
{
}

void GameScene::onTouchCancelled( Touch *pTouch, Event *pEvent )
{
}

std::string GameScene::getSystemTime()
{
	tm* thistime;
	time_t timep;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
	time(&timep);
#else
	timeval tv;
	gettimeofday(&tv,NULL);
	timep = tv.tv_sec;
#endif
	thistime = localtime(&timep);
	std::stringstream result;
	result<<thistime->tm_year+1900<<thistime->tm_mon+1<<thistime->tm_mday<<thistime->tm_hour<<thistime->tm_min<<thistime->tm_sec;
	return result.str();
}