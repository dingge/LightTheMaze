#include "SelectLayer.h"
#include "Game/GameScene.h"
#include "Setting.h"
#include "UI/MainUI/MenuScene.h"

using namespace cocostudio;
using namespace cocos2d::ui;

USING_NS_CC;

SelectLayer::SelectLayer():
    focused_btn_index_(0),
    scroll_view_(nullptr)
{
}

SelectLayer::~SelectLayer()
{
}

bool SelectLayer::init()
{
	bool ret = false;
	do
	{
		CC_BREAK_IF(!Layer::init());
		Size winSize = Director::getInstance()->getWinSize();
		this->setContentSize(winSize);

        Sprite* bg = Sprite::create("ui/background.png");
        bg->setScaleX(winSize.width / bg->getContentSize().width);
        bg->setScaleY(winSize.height / bg->getContentSize().height);
        bg->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
        this->addChild(bg);

		const ValueVector& level_info = Setting::getInstance()->getLevelInfo();
        const int newest_level = Setting::getInstance()->getNewestLevel();

        const float cell_padding = winSize.height*0.05f;
        const Vec2 display_origin(0.0f , winSize.height / 2);
        btn_size_ = Size(winSize.width / 2 - cell_padding * 2, winSize.width / 2 - cell_padding * 2);

        scroll_view_ = ScrollView::create();
        scroll_view_->setDirection(ui::ScrollView::Direction::HORIZONTAL);
        scroll_view_->setContentSize(winSize);
        this->addChild(scroll_view_);
        
        Size cur_size = Size::ZERO;
        for (int i = 0; i < level_info.size(); ++i)
        {
            std::string normal_image = level_info[i].asValueMap().at("image_path").asString();
            Button* btn = Button::create(normal_image);
            Size size = btn->getContentSize();
            btn->setScaleX(btn_size_.width / size.width);
            btn->setScaleY(btn_size_.height / size.height);
            btn->setPosition(Vec2(display_origin.x + btn_size_.width*(i + 0.5) + cell_padding*(i+1), display_origin.y));
            btn->setZoomScale(0.0f);
            btn->addTouchEventListener(CC_CALLBACK_2(SelectLayer::levelBtnCallback, this));
            //将关卡文件序号置于Tag域，方便传递给回调函数
            btn->setTag(i);
            //锁定未解锁关卡
            if (i > newest_level)
            {
				btn->setBright(false);
				btn->setTouchEnabled(false);
            }

            cur_size.width += cell_padding + btn_size_.width;
            scroll_view_->addChild(btn);
            scroll_view_->setInnerContainerSize(cur_size);

            btn_vector_.pushBack(btn);
        }
        
		auto keyboardlistener = EventListenerKeyboard::create();
		keyboardlistener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* event){};
		keyboardlistener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event* event){
			if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE || keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE)
			{
				Director::getInstance()->replaceScene(TransitionCrossFade::create(0.5f, MenuScene::createScene()));
				event->stopPropagation();
			}
            if (keyCode == EventKeyboard::KeyCode::KEY_KP_ENTER)
            {
                auto cur_btn = btn_vector_.at(focused_btn_index_);
                if (cur_btn->isTouchEnabled())
                {
                    Director::getInstance()->replaceScene(GameScene::createScene(cur_btn->getTag(), true));
                }
            }
            if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
            {
                setFocusedButton(focused_btn_index_ - 1);
                scrollToFocusedButton();
            }
            if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
            {
                setFocusedButton(focused_btn_index_ + 1);
                scrollToFocusedButton();
            }
		};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardlistener,this);

        scheduleUpdate();

		ret = true;
	}while(0);
	return ret;
}

void SelectLayer::onEnter()
{
    Layer::onEnter();

    const int newest_level = Setting::getInstance()->getNewestLevel();
    setFocusedButton(newest_level);
    scrollToFocusedButton(true);
}

void SelectLayer::levelBtnCallback(Ref* pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
        int level_id = static_cast<Node*>(pSender)->getTag();
		Director::getInstance()->replaceScene(GameScene::createScene(level_id, true));
	}
}

void SelectLayer::scrollToFocusedButton(bool is_immediately)
{
    float percent = 100.0f*focused_btn_index_ / (btn_vector_.size() - 1);
    if (is_immediately)
    {
        scroll_view_->jumpToPercentHorizontal(percent);
    }
    else
    {
        scroll_view_->scrollToPercentHorizontal(percent, 0.5f, true);
    }
}

void SelectLayer::setFocusedButton(int focused_index)
{
    if (focused_index < 0 || focused_index >= btn_vector_.size())
        return;

    if (focused_btn_index_ >= 0 && focused_btn_index_ < btn_vector_.size())
    {
        auto last_btn = btn_vector_.at(focused_btn_index_);
        last_btn->setLocalZOrder(0);
        last_btn->stopAllActions();
        last_btn->runAction(ScaleTo::create(0.1f, btn_size_.width/ last_btn->getContentSize().width));
    }
    focused_btn_index_ = focused_index;
    auto cur_btn = btn_vector_.at(focused_btn_index_);
    cur_btn->setLocalZOrder(1);
    cur_btn->stopAllActions();
    cur_btn->runAction(ScaleTo::create(0.1f, 1.0f));
}

void SelectLayer::update(float delta)
{
    Layer::update(delta);

    float focused_btn_x = btn_vector_.at(focused_btn_index_)->getPositionX();
    float focused_btn_x_in_view = focused_btn_x + scroll_view_->getInnerContainerPosition().x;
    if (focused_btn_x_in_view < 0.0f)
    {
        setFocusedButton(focused_btn_index_ + 1);
    }
    if (focused_btn_x_in_view > scroll_view_->getContentSize().width)
    {
        setFocusedButton(focused_btn_index_ - 1);
    }
}
