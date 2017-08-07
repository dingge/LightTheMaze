#include "MenuLayer.h"
#include "UI/LevelSelectUI/SelectScene.h"
#include "Game/GameScene.h"
#include "Setting.h"

using namespace cocostudio;
using namespace cocos2d::ui;
USING_NS_CC;

MenuLayer::MenuLayer():
    focused_btn_index_(0),
    btn_layer_(nullptr),
    information_layer_(nullptr)
{

}
MenuLayer::~MenuLayer()
{

}
bool MenuLayer::init()
{
   bool ret = false;
   do 
   {
	   CC_BREAK_IF(!Layer::init());
	   Size winSize = Director::getInstance()->getWinSize();
	   this->setContentSize(winSize);

       const Size c_btn_size = Size(204.0f, 76.0f);
       const Size c_title_size = Size(640.0f, 60.0f);
       const Vec2 c_title_pos = Vec2(63.0f, 698.0f);
       const Vec2 c_play_btn_pos = Vec2(752.0f, 395.0f);
       const Vec2 c_continue_btn_pos = Vec2(752.0f, 297.0f);
       const Vec2 c_information_btn_pos = Vec2(752.0f, 199.0f);
       const Vec2 c_exit_btn_pos = Vec2(752.0f, 101.0f);

       Sprite* bg = Sprite::create("ui/background.png");
       bg->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
       bg->setScaleX(winSize.width / bg->getContentSize().width);
       bg->setScaleY(winSize.height / bg->getContentSize().height);
       this->addChild(bg);

       Sprite* title = Sprite::create("ui/title.png");
       title->setScaleX(c_title_size.width / title->getContentSize().width);
       title->setScaleY(c_title_size.height / title->getContentSize().height);
       title->setPosition(c_title_pos + Size(c_title_size.width / 2, -c_title_size.height / 2));
       this->addChild(title);

       btn_layer_ = Layer::create();
       this->addChild(btn_layer_);

       Button* play_btn = Button::create("ui/play_N.png");
       play_btn->setPosition(c_play_btn_pos + Size(c_btn_size.width / 2, -c_btn_size.height / 2));
       play_btn->setScaleX(c_btn_size.width / play_btn->getContentSize().width);
       play_btn->setScaleY(c_btn_size.height / play_btn->getContentSize().height);
       play_btn->addTouchEventListener(CC_CALLBACK_2(MenuLayer::playCallBack, this));
       btn_layer_->addChild(play_btn, 1);

       Button* continue_btn = Button::create("ui/continue_N.png");
       continue_btn->setPosition(c_continue_btn_pos + Size(c_btn_size.width / 2, -c_btn_size.height / 2));
       continue_btn->setScaleX(c_btn_size.width / continue_btn->getContentSize().width);
       continue_btn->setScaleY(c_btn_size.height / continue_btn->getContentSize().height);
       continue_btn->addTouchEventListener(CC_CALLBACK_2(MenuLayer::continueCallBack, this));
       btn_layer_->addChild(continue_btn, 2);

       Button* information_btn = Button::create("ui/information_N.png");
       information_btn->setPosition(c_information_btn_pos + Size(c_btn_size.width / 2, -c_btn_size.height / 2));
       information_btn->setScaleX(c_btn_size.width / information_btn->getContentSize().width);
       information_btn->setScaleY(c_btn_size.height / information_btn->getContentSize().height);
       information_btn->addTouchEventListener(CC_CALLBACK_2(MenuLayer::informationCallBack, this));
       btn_layer_->addChild(information_btn, 3);

       Button* exit_btn = Button::create("ui/exit_N.png");
       exit_btn->setPosition(c_exit_btn_pos + Size(c_btn_size.width / 2, -c_btn_size.height / 2));
       exit_btn->setScaleX(c_btn_size.width / exit_btn->getContentSize().width);
       exit_btn->setScaleY(c_btn_size.height / exit_btn->getContentSize().height);
       exit_btn->addTouchEventListener(CC_CALLBACK_2(MenuLayer::exitCallBack, this));
       btn_layer_->addChild(exit_btn, 4);

       //在没有通关进度时，禁掉continue按钮
       if (0 == Setting::getInstance()->getNewestLevel())
       {
           continue_btn->setBright(false);
           continue_btn->setTouchEnabled(false);
       }

	   //添加键盘事件处理
	   auto keyboardlistener = EventListenerKeyboard::create();
	   keyboardlistener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* event){};
	   keyboardlistener->onKeyReleased = [&, play_btn](EventKeyboard::KeyCode keyCode, Event* event){
		   if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE || keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE)
		   {
               if (isInformationOpen())
               {
                   closeInformation();
               }
               else
               {
                   Director::getInstance()->end();
               }
		   }
           if (keyCode == EventKeyboard::KeyCode::KEY_KP_ENTER)
           {
               if (play_btn->isTouchEnabled())
               {
                   auto scene = TransitionCrossFade::create(0.5f, SelectScene::createScene());
                   Director::getInstance()->replaceScene(scene);
               }
           }
	   };
	   _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardlistener,this);

	   ret = true;
   } while (0);
   return ret;
}
void MenuLayer::playCallBack(Ref* pSender,Widget::TouchEventType type)
{
	if(type == Widget::TouchEventType::ENDED)
	{
		auto scene = TransitionCrossFade::create(0.5f,SelectScene::createScene());
		Director::getInstance()->replaceScene(scene);
	}
}

void MenuLayer::continueCallBack(cocos2d::Ref * pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        const int newest_level = Setting::getInstance()->getNewestLevel();
        Director::getInstance()->replaceScene(
            GameScene::createScene(newest_level, true));
    }
}

void MenuLayer::informationCallBack(Ref* pSender,Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        openInformation();
    }
}

void MenuLayer::exitCallBack(Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        Director::getInstance()->end();
    }
}

void MenuLayer::openInformation()
{
    if (isInformationOpen())
    {
        return;
    }

    btn_layer_->setVisible(false);
    information_layer_ = Layer::create();
    this->addChild(information_layer_);

    Size win_size = Director::getInstance()->getWinSize();

    Scale9Sprite* edge = Scale9Sprite::create("ui/edge.png",Rect(0,0,751,323),Rect(15,15,721,293));
    edge->setContentSize(Size(win_size.width * 0.75, win_size.height / 2));
    Size edge_size = edge->getContentSize();
    edge->setPosition(Vec2(win_size.width / 2, win_size.height / 2));
    information_layer_->addChild(edge);

    const std::string info_text = Setting::getInstance()->getInfomationText();
    const std::string font_name = Setting::getInstance()->getZhFontName();

    Text* text = Text::create(info_text, font_name, 30.0f);
    text->setTextHorizontalAlignment(TextHAlignment::CENTER);
    static_cast<Label*>(text->getVirtualRenderer())->setLineHeight(40.0f);
    text->setTextColor(Color4B::BLACK);
    text->setPosition(Vec2(edge_size.width / 2, edge_size.height / 5*3));

    const Size c_btn_size = Size(204.0f, 76.0f);
    Button* close_btn = Button::create("ui/close_N.png");
    close_btn->setPosition(Vec2(edge_size.width / 2, edge_size.height / 5));
    close_btn->setScaleX(c_btn_size.width / close_btn->getContentSize().width);
    close_btn->setScaleY(c_btn_size.height / close_btn->getContentSize().height);
    close_btn->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType type) {
        closeInformation();
    });

    edge->addChild(text);
    edge->addChild(close_btn);
}

void MenuLayer::closeInformation()
{
    if (isInformationOpen() == false)
    {
        return;
    }

    information_layer_->removeFromParent();
    information_layer_ = nullptr;
    btn_layer_->setVisible(true);
}

bool MenuLayer::isInformationOpen()
{
    return information_layer_ != nullptr;
}
