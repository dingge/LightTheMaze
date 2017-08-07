#include "PauseScene.h"

USING_NS_CC;

PauseScene::~PauseScene()
{
	m_rt->retain();
	m_rt->autorelease();//延迟释放，防止RenderCommand野指针
}

PauseScene* PauseScene::create( Node* root )
{
	PauseScene *ret = new PauseScene();
	if (ret && ret->init(root))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return ret;
}

bool PauseScene::init( Node* root )
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Scene::init());

		auto winsize = Director::getInstance()->getWinSize();
		m_rt = RenderTexture::create((int)winsize.width, (int)winsize.height);
		m_rt->getSprite()->setAnchorPoint(Vec2::ZERO);
		m_rt->getSprite()->getTexture()->setAntiAliasTexParameters();
		m_rt->setPosition(Vec2::ZERO);
		this->addChild(m_rt);

		m_rt->beginWithClear(0,0,0,0);
		root->visit();
		m_rt->end();

		Director::getInstance()->getRenderer()->render();
		
		//添加键盘事件处理
		auto keyboardlistener = EventListenerKeyboard::create();
		keyboardlistener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* event){};
		keyboardlistener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event* event) {};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardlistener,this);

		bRet = true;
	} while (0);
	return bRet;
}

