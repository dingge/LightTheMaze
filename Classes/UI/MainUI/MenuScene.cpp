#include "MenuScene.h"

USING_NS_CC;

Scene* MenuScene::createScene()
{
	auto scene = Scene::create();
	auto gamelayer = MenuLayer::create(); 
	scene->addChild(gamelayer,0);

	return scene;
}