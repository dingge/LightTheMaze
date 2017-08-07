/********************************************************************
	date  :	2016/01/30   18:40

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/
#ifndef SELECTSCENE_H_
#define SELECTSCENE_H_

#include "cocos2d.h"
#include "SelectLayer.h"

class SelectScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	static SelectScene* create();
protected:
	SelectScene(){}
	virtual ~SelectScene();
};
#endif // SELECTSCENE_H_