/********************************************************************
	date  :	2016/01/30   15:35

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef GAME_MANAGE_H
#define GAME_MANAGE_H

#include "Game/GameUtils/Singleton.h"
#include "Game/Map/Tiled.h"
#include "Game/Effect/EffectLayer.h"
#include "Game/Dark/DarkSprite.h"
#include "Game/Light/LightSpriteNode.h"
#include "Game/Mechanism/BeaconNode.h"

/**
 *	��Ϸ��Դ������(����)
 *	����ؿ����ݽ������ж��󲢱�������
 *	�ṩͳһ����Ľӿ�
 */
class GameManage : public Singleton<GameManage>
{
public:
	enum PlayerType
	{
		FIRST,
		SECOND
	};
public:
	void setPhysicsWorld(cocos2d::PhysicsWorld* world) {m_physicsWorld = world;}
	cocos2d::PhysicsWorld* getPhysicsWorld() {return m_physicsWorld;}

	cocos2d::Node* loadLevelFromFile(const int level_id);
	
	void clearResourse();

	const std::string& getLevelFileName()const {return m_levelName;}
	LightSpriteNode* getNowPlayer();
	LightSpriteNode* getPlayerFirst(){return m_playerFirst;}
	LightSpriteNode* getPlayerSecond(){return m_playerSecond;}
	Tiled* getMap() {return m_mapLayer;}
	EffectLayer* getEffectLayer() {return m_effectLayer;}
	std::vector<DarkSprite*>& getDarkVector();
	std::vector<LightFragmentNode*>& getFragmentVector();
	std::vector<cocos2d::Vec2>& getPotalPositionVec() {return m_potalPositionVec;}
    cocos2d::Vec2 getBeaconPos()const { return m_beacon->getPosition(); }
	//����Ƭ��
	int getAllFragmentNumber()const {return m_needFragmentNumber;}
	//ʣ����Ƭ��
	int getRemainFragmentNumber();

	//����������ع���
	void updateMapPositionForLightNode(const cocos2d::Vec2& mappos);
	void updateLightNodes();
	std::vector<LightNode*>& getLightNodesVector();

	//��ʾ��ʾ
	void showTip(const std::string& tip);
	void showTip();
	//��ʾͼ��
	void showAtlas();
	//����ȫ��ͼ
	void shineTheMap();
    //ͨ��ʧ��
    void onLostLevel();
    //ͨ�سɹ�
    void onPassLevel();
    //��ȡ������ͼ��ͼ·��
	std::string getLevelShineMapFileName();
protected:
	GameManage();
	~GameManage();

	friend Singleton<GameManage>;
	friend std::auto_ptr<GameManage>;

    void loadMapObject(const cocos2d::TMXObjectGroup* mapGroup, cocos2d::Node* root);
	void loadNpcObject(const cocos2d::TMXObjectGroup* npcGroup, cocos2d::Node* root);
	void loadItemObject(const cocos2d::TMXObjectGroup* itemGroup, cocos2d::Node* root);
	void loadMechanismObject(const cocos2d::TMXObjectGroup* mechanismGroup, cocos2d::Node* root);

	cocos2d::Action* loadMoveAction(const cocos2d::ValueVector& obj,const int actionName);

	const cocos2d::ValueMap findDictionaryByName(const cocos2d::ValueVector& obj,int name);

	//ˢ�¹��ս������
	void updateLightNodeContainer();
	//ˢ�°�����������
	void updateDarkSpriteContainer();
	//ˢ����Ƭ�������
	void updateFragmentContainer();
    //��ͣ����
    void pauseScene();
    //������һ��
    void unlockNextLevel();
    //�л���ѡ��
    void returnToSelectScene();
protected:
	//������ɫ��
	void createColorMap();

	std::string getFragmentPicByColor(const cocos2d::Color4B& color);
private:
    int level_id_;
	std::string m_levelName;

	std::string m_levelTip;
	bool m_isBeginTipShow;
	//Node
	cocos2d::Node* m_root;
	LightSpriteNode* m_playerFirst;
	LightSpriteNode* m_playerSecond;
	std::vector<DarkSprite*> m_darkVector;
	std::vector<LightFragmentNode*> m_collectedFragmentVector;
	std::vector<LightFragmentNode*> m_allFragmentVector;
	EffectLayer* m_effectLayer;
	Tiled* m_mapLayer;
	BeaconNode* m_beacon;

	cocos2d::PhysicsWorld* m_physicsWorld;

	//�⾫������ڵ�ͼ�Ŀ�ʼλ��
	cocos2d::Vec2 m_beginPosition;

	//����ͼ��Ҫ����Ƭ����
	int m_needFragmentNumber;

	//��־
	PlayerType m_playerType;
	//��¼���������ԵĽ��
	std::vector<LightNode*> m_lightNodeVec;
	//������λ�õ㼯
	std::vector<cocos2d::Vec2> m_potalPositionVec;
	//��������������ס�����������ƶ������Լ���Ϣ��Ӧ(��δʹ��)
	bool lock_scene_;
private:
	//��ɫ�����ڼ�¼�ַ�������ɫ��ӳ��
	std::unordered_map<std::string, cocos2d::Color4B> m_colorMap;
};

#endif // !GAME_MANAGE_H
