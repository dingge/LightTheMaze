/********************************************************************
	date  :	2016/01/30   13:26

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef SHAPE_OBJECT_H
#define SHAPE_OBJECT_H

#include "cocos2d.h"
#include "Game/Macros.h"
#include "Game/GameUtils/Line.h"
#include "Game/GameUtils/Singleton.h"
#include "Game/Extension/QuadTree.h"
#include "Game/Mechanism/MechanismCollisionNode/MechanismCollisionNode.h"

/*椭圆切割份数*/
#define ELLIPSE_PART_NUMBER 12

/**
 *	形状管理类（单例）
 *	用于管理用于光照处理和物理障碍所用到的形状
 *  使用四叉树进行优化
 *	读取地图相应的ObjectGroup模块，生成对应线段组
 *	为了提高性能，提供一个方法获取一定区域内的障碍用于光照处理。
 *	提供增加、删除、修改、打印形状功能
 */
class ShapeManage : public Singleton<ShapeManage>
{
public:
	void initWithMap(const cocos2d::Size& map_size);

	///获取当前点附近一定半径内的线段
	std::vector<Edge> getNearbyLine(const cocos2d::Vec2& nowPos,float radius) const;
	std::vector<Edge> getRaycastLine(const Line& ray) const;

	///添加形状
	void addBody(const NodeGroupPtr& obj);

	///删除形状（从所属物件的name，把相关物件的所有形状全部删除）
	void removeBody(const NodeGroupPtr& obj);

	///修改形状(通过传入的待修改形状集合的id进行匹配)
	void alterBody(const NodeGroupPtr& obj);

    ///从传统数据转换到结点组
    NodeGroupPtr convertToGroup(const Line& data, const cocos2d::Color4B& color = cocos2d::Color4B::WHITE) const;
    NodeGroupPtr convertToGroup(const std::vector<Line>& data, const cocos2d::Color4B& color = cocos2d::Color4B::WHITE) const;

    ///打印四叉树
    void print() const;
public:
	std::vector<Line> createShape(const cocos2d::ValueMap& dict, const std::string& element_name) const;
	std::vector<Line> createRectangle(const cocos2d::ValueMap& dic) const;
	std::vector<Line> createEllipse(const cocos2d::ValueMap& dic) const;
	std::vector<Line> createPolygon(const cocos2d::ValueMap& dic) const;
	std::vector<Line> createPolyLine(const cocos2d::ValueMap& dic) const;
	
    ///计算线段交点并分裂线段，新分裂线段插入shape集合
	void calculateCrossPos(std::vector<Edge>* line_container) const;
protected:
	ShapeManage():
		m_quadTree(nullptr)
	{}
	~ShapeManage(){}

	friend Singleton<ShapeManage>;
	friend std::auto_ptr<ShapeManage>;

private:
	///两个线段是否相交
	inline bool isCrossTwoSegment(const Line& l1,const Line& l2, cocos2d::Vec2* crossPos) const;
	///两点是否重合
	inline bool isCoincide(const cocos2d::Vec2& v1,const cocos2d::Vec2& v2) const;
private:
	cocos2d::Size            m_mapSize;
	QuadTree*                m_quadTree;
};
#endif // !SHAPE_OBJECT_H
