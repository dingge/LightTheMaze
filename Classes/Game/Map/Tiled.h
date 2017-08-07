/********************************************************************
	date  :	2016/01/30   13:24

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef TILED_H
#define TILED_H

#include "cocos2d.h"
#include "Game/Macros.h"
#include "Game/GameUtils/Line.h"
#include "Game/GameUtils/CollisionProcessNode.h"
#include "Game/GameUtils/DrawNodeSpec.h"
#include "Game/Extension/QuadTree.h"

class ShapeManage;

/**
 *	地图类
 *	包含一个加载地图文件的方法以及对象层的接口
 *	包含更新一定范围内的形状生成物理障碍的方法
 *	右下角画了一个方格是因为0个方格的话会直接报错==！
 */
class Tiled : public cocos2d::Layer
{
public:
	static Tiled* create(const std::string& tmxName);

	//获取对象层接口
	cocos2d::TMXObjectGroup* getObjectGroup(const std::string& groupName);
	//设置所有静态形状
    void setStaticShape(const std::vector<NodeGroupPtr>& group_container);
    //绘制静态障碍
    void drawStaticShape();
	//更新碰撞障碍
	void updatePhysicBody();
	
	int getMapWidth()const {return m_mapWidth;}
	int getMapHeight()const {return m_mapHeight;}

	cocos2d::PhysicsBody* getMapPhysicsBody();
	cocos2d::DrawNode* getStaticDrawNode(){return m_staticShapeDraw;}

	void shineTheMap();

#ifdef GAME_DEBUG
	void drawDebugShapeOnMap(const std::vector<Line>& lineVec);
#endif // GAME_DEBUG

private:
	Tiled();
	virtual ~Tiled();

	virtual bool init()override;
	
private:
	std::string					m_tmxName;
	cocos2d::TMXTiledMap*		m_map;
	int							m_mapWidth;			//地图总宽度
	int							m_mapHeight;		//地图总高度
    std::vector<NodeGroupPtr>   m_group_container;  //地图障碍物集合
	StaticPhysicCollisionNode*	m_wall;				//墙（静态障碍）
	DrawNodeSpec*			    m_staticShapeDraw;	//绘制静态障碍
#ifdef GAME_DEBUG
	cocos2d::DrawNode*          m_debugDraw;        //debug用
#endif // GAME_DEBUG
	cocos2d::Layer*				m_shineLayer;		//照亮全地图预留层
};

#endif // !TILED_H
