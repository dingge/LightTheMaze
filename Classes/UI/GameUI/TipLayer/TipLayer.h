/********************************************************************
	date  :	2016/01/30   18:29

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef TIPLAYER_H_
#define TIPLAYER_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

/**
 *	提示标签类
 *  只用于PauseScene中
 */
class TipLayer : public cocos2d::Layer
{
public:
	bool init(const std::string& tip);
	static TipLayer* create(const std::string& tip);
protected:
	TipLayer():
		m_label(nullptr)
	{}
	virtual ~TipLayer() {}

	void setTip(std::string);
private:
	cocos2d::ui::Text* m_label;
};
#endif // TIPLAYER_H_