/********************************************************************
	date  :	2016/01/30   18:41

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef MENULAYER_H_
#define MENULAYER_H_

#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class MenuLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(MenuLayer);

protected:
	MenuLayer();
	virtual ~MenuLayer();

	virtual bool init();

	void playCallBack(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);

    void continueCallBack(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);

	void informationCallBack(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);

    void exitCallBack(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    
    void openInformation();
    void closeInformation();
    bool isInformationOpen();
private:
    cocos2d::Layer* btn_layer_;
    cocos2d::Layer* information_layer_;
    int focused_btn_index_;
};
#endif // MENULAYER_H_