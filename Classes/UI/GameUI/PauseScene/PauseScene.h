/********************************************************************
	date  :	2016/01/30   18:29

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef PAUSE_SCENE_H
#define PAUSE_SCENE_H

#include "cocos2d.h"

/**
 *	‘›Õ£≥°æ∞¿‡
 */
class PauseScene : public cocos2d::Scene
{
public:
	static PauseScene* create(cocos2d::Node* root);
protected:
	PauseScene():
		m_rt(nullptr)
	{}
	virtual ~PauseScene();

	bool init(cocos2d::Node* root);
private:
	cocos2d::RenderTexture* m_rt;
};

#endif // !PAUSE_SCENE_H
