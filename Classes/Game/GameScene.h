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
 *	��Ϸ������
 *	��Ҫ���ܣ����̿��ƣ��¼��Ľ��պʹ���
 */
class GameScene :public cocos2d::Scene
{
public:
	GameScene(void);
	~GameScene(void);
	static GameScene* create(int level_id);

	//�������������ݲ����Ƿ�����������
    static GameScene* createScene(int level_id, bool isUsePhysic = false);

	//�����¼��ص�
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	//����ʱ��ص�
	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	void onTouchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);

	//������ײ�¼��ص�
	bool onContactBegin(cocos2d::PhysicsContact& contact);
	bool onContactPreSolve(cocos2d::PhysicsContact& contact, cocos2d::PhysicsContactPreSolve& solve);
	void onContactPostSolve(cocos2d::PhysicsContact& contact, const cocos2d::PhysicsContactPostSolve& solve);
	void onContactSeperate(cocos2d::PhysicsContact& contact);

	//���ڴ���ť��ס�¼�
	virtual void update(float delta)override;
protected:
	virtual bool init(int level_id);

	void processPlayerMove(const cocos2d::Vec2& impulse);
    std::string getSystemTime();
private:
	std::vector<cocos2d::EventKeyboard::KeyCode>	m_pressKeyVec;	///<ģ�ⰴ����ס�¼�
	Joystick* m_joyStick; ///<ҡ��
};
#endif // !GAME_SCENE_H