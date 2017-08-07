/********************************************************************
	date  :	2016/01/30   18:32

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef SELECTLAYER_H_
#define SELECTLAYER_H_

#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class SelectLayer : public cocos2d::Layer
{
public:	
	CREATE_FUNC(SelectLayer);

    void onEnter()override;

    void update(float delta)override;

protected:
	SelectLayer();
	~SelectLayer();

	virtual bool init();

	void levelBtnCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);

    void setFocusedButton(int focused_index);

    void scrollToFocusedButton(bool is_immediately = false);
private:
    cocos2d::ui::ScrollView* scroll_view_;
    cocos2d::Vector<cocos2d::ui::Button*> btn_vector_;
    cocos2d::Size btn_size_;
    int focused_btn_index_;
};
#endif // SELECTLAYER_H_