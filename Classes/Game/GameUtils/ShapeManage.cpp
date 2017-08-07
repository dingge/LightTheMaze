#include "ShapeManage.h"

USING_NS_CC;

void ShapeManage::initWithMap(const cocos2d::Size& map_size)
{
	m_mapSize = map_size;
    m_quadTree = new QuadTree(Rect(0.0f, 0.0f, m_mapSize.width, m_mapSize.height));
}

std::vector<Edge> ShapeManage::getNearbyLine( const Vec2& nowPos,float radius ) const
{
	std::vector<Edge> nearVec;
	float radiusSq = radius*radius;

	m_quadTree->searchByRect(Rect(nowPos.x - radius, nowPos.y - radius, radius * 2, radius * 2), &nearVec);
	nearVec.erase(std::remove_if(nearVec.begin(), nearVec.end(), [&nowPos, radius, radiusSq](const Line& line) {
		//�����޳�
		if ((nowPos - line._p1).dot((line._p2 - line._p1).getRPerp()) < 0)
			return true;

		//�����ڷ�Χ��
		if ((line._p1 - nowPos).getLengthSq() < radiusSq ||
			(line._p2 - nowPos).getLengthSq() < radiusSq)
		{
			return false;
		}
		else
		{
			//�㵽�߶εĴ������߶��ϣ��Ҵ����ڷ�Χ��
			Vec2 vn1 = nowPos - line._p1;
			Vec2 vn2 = nowPos - line._p2;
			Vec2 vit = line._p2 - line._p1;
			if (vn1.dot(vit) >= 0 &&
				vn2.dot(vit) <= 0 &&
				vn1.getLengthSq() - pow(vit.dot(vn1), 2) / vit.getLengthSq() < radiusSq)
			{
				return false;
			}
		}
		return true;
	}), nearVec.end());
	//�Խ�����򣬱�֤�����߶���β���������ͼ��㸴�Ӷ�
	std::sort(nearVec.begin(), nearVec.end());
	
	return nearVec;
}

std::vector<Edge> ShapeManage::getRaycastLine(const Line& ray) const
{
	std::vector<Edge> result;
	m_quadTree->searchByRay(ray, &result);
	return result;
}

void ShapeManage::print() const
{
    m_quadTree->printQuadTree();
}

std::vector<Line> ShapeManage::createShape(const cocos2d::ValueMap& dict, const std::string& element_name) const
{
	if (dict.find(element_name) != dict.end())
	{
		if (dict.at(element_name).asString() == "rectangle")
		{
			return createRectangle(dict);
		}
		if (dict.at(element_name).asString() == "ellipse")
		{
			return createEllipse(dict);
		}
		if (dict.at(element_name).asString() == "polygon")
		{
			return createPolygon(dict);
		}
		if (dict.at(element_name).asString() == "polyline")
		{
			return createPolyLine(dict);
		}
	}
	return std::vector<Line>();
}

std::vector<Line> ShapeManage::createRectangle( const ValueMap& dic ) const
{
	//���µ�ֵ�Ѿ����Զ�ת����ԭ��Ϊ���½ǵ����꣬x+width��y+height�ǶԽ�������
	//����ʱ����ʱ�뷽�򴴽���������ֻ�ⲿ�ɼ����ڲ��޳�
	float x      = (float)dic.at("x").asInt();
	float y      = (float)dic.at("y").asInt();
	float width  = (float)dic.at("width").asInt();
	float height = (float)dic.at("height").asInt();
	int name = dic.at("name").asInt();
	
	std::vector<Line> tempLineVec;
	tempLineVec.push_back(Line(x, y, x + width, y,  (name << 16) + 3));
	tempLineVec.push_back(Line(x + width, y, x + width, y + height, (name << 16) + 2));
	tempLineVec.push_back(Line(x + width, y + height, x, y + height, (name << 16) + 1));
	tempLineVec.push_back(Line(x, y + height, x, y, (name << 16)));
	return tempLineVec;
}

std::vector<Line> ShapeManage::createEllipse( const ValueMap& dic ) const
{
	//����Ellipseʱ����ʱ�뷽�򴴽���������ֻ�ⲿ�ɼ����ڲ��޳�
	float x      = (float)dic.at("x").asInt();
	float y      = (float)dic.at("y").asInt();
	float width  = (float)dic.at("width").asInt();
	float height = (float)dic.at("height").asInt();
	//���ﻳ����û�ж�ȡ��ת��ֵ�ԣ������ת��Ҫ�ֶ����һ���Զ�������angle���������Ƕȣ�Ĭ��Ϊ0
	float rotation = CC_DEGREES_TO_RADIANS((dic.find("angle")!=dic.end())?dic.at("angle").asFloat():0);
	int name = dic.at("name").asInt();

	//ʹ����Բ�Ĳ������̽������ƵĶ����
	//��Ϊtiled����Բԭ�㲻ȷ������ʱ���ܶ���Բ������ת
	std::vector<Line> tempLineVec;
	float a = width/2;
	float b = height/2;
	float centerX = x - width/2*(-cos(rotation)) - height/2*sin(rotation);
	float centerY = y - height/2*(-cos(rotation)) + width/2*sin(rotation);
	float sa = sin(rotation);
	float ca = cos(rotation);
	float lastX = centerX + a*ca;
	float lastY = centerY + a*sa;
	for (int i=1;i<=ELLIPSE_PART_NUMBER;++i)
	{
		float t = CC_DEGREES_TO_RADIANS(360/ELLIPSE_PART_NUMBER*i);
		float st = sin(t);
		float ct = cos(t);
		float tx = centerX + a*ct*ca - b*st*sa;
		float ty = centerY + a*ct*sa + b*st*ca;
		tempLineVec.push_back(Line(lastX,lastY,tx,ty,(name<<16)+i));
		lastX = tx;
		lastY = ty;
	}
	return tempLineVec;
}

std::vector<Line> ShapeManage::createPolygon( const ValueMap& dic ) const
{
	//���ݴ��������ʱ��˳(��)ʱ�뷽�򴴽���������ʱ�����ⲿ�ɼ����ڲ��޳���˳ʱ�뷴֮
	int originX = dic.at("x").asInt();
	int originY = dic.at("y").asInt();
	int name = dic.at("name").asInt();

	std::vector<Line> tempLineVec;
	std::vector<Vec2> poslist;
	for(const auto& points : dic.at("points").asValueVector())
	{
		const auto vm = points.asValueMap();
		int ix = originX + vm.at("x").asInt();
		int iy = originY - vm.at("y").asInt();
		poslist.push_back(Vec2((float)ix, (float)iy));
	}

	for (size_t i = 0;i < poslist.size()-1;++i)
	{
		tempLineVec.push_back(Line(poslist[i], poslist[i + 1], (name << 16) + i));
	}
	//�տ�
	tempLineVec.push_back(Line(poslist.back(), *poslist.begin(), (name << 16) + poslist.size() - 1));
	return tempLineVec;
}

std::vector<Line> ShapeManage::createPolyLine( const ValueMap& dic ) const
{
	//���ݴ���Polylineʱ��˳(��)ʱ�뷽�򴴽���������ʱ�����ⲿ�ɼ����ڲ��޳���˳ʱ�뷴֮
	//������Polyline�տڣ����������ֵ��޳�����
	int originX = dic.at("x").asInt();
	int originY = dic.at("y").asInt();
	int name = dic.at("name").asInt();

	std::vector<Vec2> poslist;
	for(const auto& points : dic.at("polylinePoints").asValueVector())
	{
		const auto vm = points.asValueMap();
		int ix = originX + vm.at("x").asInt();
		int iy = originY - vm.at("y").asInt();
		poslist.push_back(Vec2((float)ix, (float)iy));
	}

	std::vector<Line> tempLineVec;
	for (size_t i = 0;i<poslist.size()-1;++i)
	{
		tempLineVec.push_back(Line(poslist[i],poslist[i+1],(name<<16)+i));
	}
	return tempLineVec;
}

void ShapeManage::addBody( const NodeGroupPtr& obj )
{
    m_quadTree->insert(obj);
}

void ShapeManage::removeBody(const NodeGroupPtr& obj)
{
    m_quadTree->remove(obj);
}

void ShapeManage::calculateCrossPos(std::vector<Edge>* line_container) const
{
	for (size_t i = 0;i<line_container->size();++i)
	{
		for (size_t j = 0;j<line_container->size();++j)
		{
			Vec2 crossPos;
			//�ж����߶��Ƿ��ཻ
			if (isCrossTwoSegment((*line_container)[i],(*line_container)[j],&crossPos) == true)
			{
				//�����ཻ�߶η��ѳ��ĸ�
				Vec2 temp = (*line_container)[i]._p2;
                (*line_container)[i]._p2 = crossPos;
                (*line_container).push_back(Edge(Line(crossPos,temp), (*line_container)[i].getColor()));

				temp = (*line_container)[j]._p2;
                (*line_container)[j]._p2 = crossPos;
                (*line_container).push_back(Edge(Line(crossPos,temp), (*line_container)[j].getColor()));
			}
		}
	}
}

bool ShapeManage::isCrossTwoSegment( const Line& l1,const Line& l2,Vec2* crossPos ) const
{
// 	//���˵��ཻ�����
// 	if(isCoincide(l1._p1,l2._p1) || isCoincide(l1._p1,l2._p2) || isCoincide(l1._p2,l2._p1) || isCoincide(l1._p2,l2._p2))
// 		return false;
//     
// 	Vec2 v1 = l1._p2-l1._p1;
// 	Vec2 v2 = l2._p2 - l2._p1;
// 	float crossVW = v1.cross(v2);
// 	//ƽ��
// 	if(fabs(crossVW)< 0.01)
// 		return false;
// 
// 	Vec2 u = l1._p1 - l2._p1;
// 	float t1 = v2.cross(u)/crossVW;
// 	float t2 = v1.cross(u)/crossVW;
// 	//�����߶�
// 	if(t1<0 || t1>1 || t2<0 || t2>1)
// 		return false;
// 
// 	*crossPos = Vec2(v1*t1+l1._p1);
// 
// 	return true;

    if (l1._p1 == l2._p1 || l1._p1 == l2._p2 || l1._p2 == l2._p1 || l1._p2 == l2._p2)
        return false;

    float s = 0.0f, t = 0.0f;
    bool is_crossed = Vec2::isLineIntersect(l1._p1, l1._p2, l2._p1, l2._p2, &s, &t);
    if (is_crossed && s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        *crossPos = l1._p1 + s*(l1._p2 - l1._p1);
        return true;
    }
    return false;
}

bool ShapeManage::isCoincide( const Vec2& v1,const Vec2& v2 ) const
{
	if((v1 - v2).lengthSquared()<1.0)
		return true;
	return false;
}

void ShapeManage::alterBody(const NodeGroupPtr& obj)
{
    m_quadTree->alter(obj);
}

NodeGroupPtr ShapeManage::convertToGroup(const Line & data, const cocos2d::Color4B& color) const
{
    NodeGroupPtr p = std::make_shared<NodeGroup<Edge>>();
    p->addNode(Edge(data,color));
    p->fixUpRect();
    return p;
}

NodeGroupPtr ShapeManage::convertToGroup(const std::vector<Line>& data, const cocos2d::Color4B& color) const
{
    NodeGroupPtr p = std::make_shared<NodeGroup<Edge>>();
    for (auto it : data)
    {
        p->addNode(Edge(it, color));
    }
    p->fixUpRect();
    return p;
}