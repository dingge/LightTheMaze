#include "TipLayer.h"
#include "Setting.h"

USING_NS_CC;

using namespace cocostudio;
using namespace cocos2d::ui;

bool TipLayer::init(const std::string& tip)
{
	bool ret = false;
	do 
	{
		CC_BREAK_IF(!Layer::init());

		Size winSize = Director::getInstance()->getWinSize();
		this->setContentSize(winSize);

		auto childLayer = LayerColor::create(Color4B(200,213,243,50),winSize.width,winSize.height*0.8f);
		childLayer->setPosition(0,winSize.height*0.1f);
		this->addChild(childLayer);

		this->setTip(tip);

		//设置触碰触碰事件
		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan     = [](Touch *pTouch, Event *pEvent)->bool{return true;};
		listener->onTouchMoved     = [](Touch *pTouch, Event *pEvent){};
		listener->onTouchEnded     = [&](Touch *pTouch, Event *pEvent){
			Director::getInstance()->popScene();
		};
		listener->onTouchCancelled = [](Touch *pTouch, Event *pEvent){};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);

		//添加键盘事件处理
		auto keyboardlistener = EventListenerKeyboard::create();
		keyboardlistener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* event){};
		keyboardlistener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event* event){
			if(keyCode == EventKeyboard::KeyCode::KEY_SPACE || 
                keyCode == EventKeyboard::KeyCode::KEY_ESCAPE || 
                keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE ||
                keyCode == EventKeyboard::KeyCode::KEY_KP_ENTER)
				Director::getInstance()->popScene();
		};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardlistener,this);
		
		ret = true;
	} while (0);
	return ret;
}
void TipLayer::setTip(std::string content)
{
	Size winSize = Director::getInstance()->getWinSize();

	if (m_label == nullptr)
	{
        m_label = Text::create();
        m_label->setFontName(Setting::getInstance()->getZhFontName());
        m_label->setFontSize(50.0f);
		m_label->setColor(Color3B(255,255,255));
		m_label->setPosition(Vec2(winSize.width/2,winSize.height/2));
		this->addChild(m_label);
	}
	m_label->setString(content);
}

TipLayer* TipLayer::create( const std::string& tip )
{
	TipLayer *ret = new TipLayer();
	if (ret && ret->init(tip))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return ret;
}
