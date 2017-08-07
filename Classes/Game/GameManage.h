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
 *	游戏资源管理类(单例)
 *	读入关卡数据建立所有对象并保持引用
 *	提供统一管理的接口
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
	//总碎片数
	int getAllFragmentNumber()const {return m_needFragmentNumber;}
	//剩余碎片数
	int getRemainFragmentNumber();

	//光照属性相关管理
	void updateMapPositionForLightNode(const cocos2d::Vec2& mappos);
	void updateLightNodes();
	std::vector<LightNode*>& getLightNodesVector();

	//显示提示
	void showTip(const std::string& tip);
	void showTip();
	//显示图鉴
	void showAtlas();
	//照亮全地图
	void shineTheMap();
    //通关失败
    void onLostLevel();
    //通关成功
    void onPassLevel();
    //获取照亮地图贴图路径
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

	//刷新光照结点容器
	void updateLightNodeContainer();
	//刷新暗精灵结点容器
	void updateDarkSpriteContainer();
	//刷新碎片结点容器
	void updateFragmentContainer();
    //暂停场景
    void pauseScene();
    //解锁下一关
    void unlockNextLevel();
    //切换回选关
    void returnToSelectScene();
protected:
	//建立颜色表
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

	//光精灵相对于地图的开始位置
	cocos2d::Vec2 m_beginPosition;

	//本地图需要的碎片总数
	int m_needFragmentNumber;

	//标志
	PlayerType m_playerType;
	//记录带光照属性的结点
	std::vector<LightNode*> m_lightNodeVec;
	//传送阵位置点集
	std::vector<cocos2d::Vec2> m_potalPositionVec;
	//场景锁，用于锁住场景中所有移动物体以及消息响应(暂未使用)
	bool lock_scene_;
private:
	//颜色表，用于记录字符串到颜色的映射
	std::unordered_map<std::string, cocos2d::Color4B> m_colorMap;
};

#endif // !GAME_MANAGE_H
