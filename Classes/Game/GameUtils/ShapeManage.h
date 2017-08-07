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

/*��Բ�и����*/
#define ELLIPSE_PART_NUMBER 12

/**
 *	��״�����ࣨ������
 *	���ڹ������ڹ��մ���������ϰ����õ�����״
 *  ʹ���Ĳ��������Ż�
 *	��ȡ��ͼ��Ӧ��ObjectGroupģ�飬���ɶ�Ӧ�߶���
 *	Ϊ��������ܣ��ṩһ��������ȡһ�������ڵ��ϰ����ڹ��մ���
 *	�ṩ���ӡ�ɾ�����޸ġ���ӡ��״����
 */
class ShapeManage : public Singleton<ShapeManage>
{
public:
	void initWithMap(const cocos2d::Size& map_size);

	///��ȡ��ǰ�㸽��һ���뾶�ڵ��߶�
	std::vector<Edge> getNearbyLine(const cocos2d::Vec2& nowPos,float radius) const;
	std::vector<Edge> getRaycastLine(const Line& ray) const;

	///�����״
	void addBody(const NodeGroupPtr& obj);

	///ɾ����״�������������name������������������״ȫ��ɾ����
	void removeBody(const NodeGroupPtr& obj);

	///�޸���״(ͨ������Ĵ��޸���״���ϵ�id����ƥ��)
	void alterBody(const NodeGroupPtr& obj);

    ///�Ӵ�ͳ����ת���������
    NodeGroupPtr convertToGroup(const Line& data, const cocos2d::Color4B& color = cocos2d::Color4B::WHITE) const;
    NodeGroupPtr convertToGroup(const std::vector<Line>& data, const cocos2d::Color4B& color = cocos2d::Color4B::WHITE) const;

    ///��ӡ�Ĳ���
    void print() const;
public:
	std::vector<Line> createShape(const cocos2d::ValueMap& dict, const std::string& element_name) const;
	std::vector<Line> createRectangle(const cocos2d::ValueMap& dic) const;
	std::vector<Line> createEllipse(const cocos2d::ValueMap& dic) const;
	std::vector<Line> createPolygon(const cocos2d::ValueMap& dic) const;
	std::vector<Line> createPolyLine(const cocos2d::ValueMap& dic) const;
	
    ///�����߶ν��㲢�����߶Σ��·����߶β���shape����
	void calculateCrossPos(std::vector<Edge>* line_container) const;
protected:
	ShapeManage():
		m_quadTree(nullptr)
	{}
	~ShapeManage(){}

	friend Singleton<ShapeManage>;
	friend std::auto_ptr<ShapeManage>;

private:
	///�����߶��Ƿ��ཻ
	inline bool isCrossTwoSegment(const Line& l1,const Line& l2, cocos2d::Vec2* crossPos) const;
	///�����Ƿ��غ�
	inline bool isCoincide(const cocos2d::Vec2& v1,const cocos2d::Vec2& v2) const;
private:
	cocos2d::Size            m_mapSize;
	QuadTree*                m_quadTree;
};
#endif // !SHAPE_OBJECT_H
