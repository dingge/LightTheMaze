#include "SelectScene.h"

USING_NS_CC;

Scene* SelectScene::createScene()
{
	auto scene = SelectScene::create();
	auto selectlayer = SelectLayer::create();
	scene->addChild(selectlayer,0);

	return scene;
}

SelectScene::~SelectScene()
{
}

SelectScene* SelectScene::create()
{
	SelectScene *ret = new SelectScene();
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}
	else
	{
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
}
