/********************************************************************
	date  :	2016/01/30   15:35

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "Game/GameUtils/Joystick.h"

#define GAME_SCENE_TAG 6

/**
 *	游戏主场景
 *	主要功能：流程控制，事件的接收和处理
 */
class GameScene :public cocos2d::Scene
{
public:
	GameScene(void);
	~GameScene(void);
	static GameScene* create(int level_id);

	//创建场景，根据参数是否开启物理引擎
    static GameScene* createScene(int level_id, bool isUsePhysic = false);

	//键盘事件回调
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	//触摸时间回调
	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	void onTouchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);

	//物理碰撞事件回调
	bool onContactBegin(cocos2d::PhysicsContact& contact);
	bool onContactPreSolve(cocos2d::PhysicsContact& contact, cocos2d::PhysicsContactPreSolve& solve);
	void onContactPostSolve(cocos2d::PhysicsContact& contact, const cocos2d::PhysicsContactPostSolve& solve);
	void onContactSeperate(cocos2d::PhysicsContact& contact);

	//用于处理按钮按住事件
	virtual void update(float delta)override;
protected:
	virtual bool init(int level_id);

	void processPlayerMove(const cocos2d::Vec2& impulse);
    std::string getSystemTime();
private:
	std::vector<cocos2d::EventKeyboard::KeyCode>	m_pressKeyVec;	///<模拟按键按住事件
	Joystick* m_joyStick; ///<摇杆
};
#endif // !GAME_SCENE_H