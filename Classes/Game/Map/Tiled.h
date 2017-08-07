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
 *	��ͼ��
 *	����һ�����ص�ͼ�ļ��ķ����Լ������Ľӿ�
 *	��������һ����Χ�ڵ���״���������ϰ��ķ���
 *	���½ǻ���һ����������Ϊ0������Ļ���ֱ�ӱ���==��
 */
class Tiled : public cocos2d::Layer
{
public:
	static Tiled* create(const std::string& tmxName);

	//��ȡ�����ӿ�
	cocos2d::TMXObjectGroup* getObjectGroup(const std::string& groupName);
	//�������о�̬��״
    void setStaticShape(const std::vector<NodeGroupPtr>& group_container);
    //���ƾ�̬�ϰ�
    void drawStaticShape();
	//������ײ�ϰ�
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
	int							m_mapWidth;			//��ͼ�ܿ��
	int							m_mapHeight;		//��ͼ�ܸ߶�
    std::vector<NodeGroupPtr>   m_group_container;  //��ͼ�ϰ��Ｏ��
	StaticPhysicCollisionNode*	m_wall;				//ǽ����̬�ϰ���
	DrawNodeSpec*			    m_staticShapeDraw;	//���ƾ�̬�ϰ�
#ifdef GAME_DEBUG
	cocos2d::DrawNode*          m_debugDraw;        //debug��
#endif // GAME_DEBUG
	cocos2d::Layer*				m_shineLayer;		//����ȫ��ͼԤ����
};

#endif // !TILED_H
