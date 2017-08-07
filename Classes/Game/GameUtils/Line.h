/********************************************************************
	date  :	2016/01/30   13:26

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef LINE_H
#define LINE_H

#include "cocos2d.h"

/**
 *	线段类
 *	形状的最小单元，包含两端点和一个id，id可以唯一标识线段的所属
 *	id从地图对象层读入，每个id对应一个自定义的单元，可能对应多条线段
 */
class Line 
{
public:
	Line():
		_p1(0,0),_p2(0,0),_id(0){}
	Line(float x1,float y1,float x2,float y2):
		_p1(x1,y1),_p2(x2,y2),_id(0){}
	Line(float x1,float y1,float x2,float y2,const int& id):
		_p1(x1,y1),_p2(x2,y2),_id(id){}
	Line(const cocos2d::Vec2& p1,const cocos2d::Vec2& p2):
		_p1(p1),_p2(p2),_id(0){}
	Line(const cocos2d::Vec2& p1,const cocos2d::Vec2& p2,const int& id):
		_p1(p1),_p2(p2),_id(id){}
    Line(const Line& right) 
    {
        this->_p1 = right._p1;
        this->_p2 = right._p2;
        this->_id = right._id;
    }

	//若有ID，则以ID作为线段的判别方式
	//否则判断线段是否重合
	bool operator ==(const Line& right)const
	{
		if(this->_id != 0 && right._id != 0)
			return this->_id == right._id;
		else 
			return ((this->_p1-right._p1).getLengthSq()<0.1 && 
					(this->_p2-right._p2).getLengthSq()<0.1);
	}

	bool operator <(const Line& right)const
	{
		return this->_id < right._id;
	}

    cocos2d::Rect getRect()const
    {
        float min_x = _p1.x < _p2.x ? _p1.x : _p2.x;
        float min_y = _p1.y < _p2.y ? _p1.y : _p2.y;
        float max_x = _p1.x > _p2.x ? _p1.x : _p2.x;
        float max_y = _p1.y > _p2.y ? _p1.y : _p2.y;
        return cocos2d::Rect(min_x, min_y, max_x - min_x, max_y - min_y);
    }
public:
	cocos2d::Vec2 _p1;			///<线段第一个点
	cocos2d::Vec2 _p2;			///<线段第二个点
	int _id;			        ///<线段专属ID，前2字节表示所属物品的name，后2字节表示序号
};

class Edge : public Line
{
public:
    Edge() :Line(), color_(cocos2d::Color4B::WHITE) {}
    Edge(const Line& line, const cocos2d::Color4B color = cocos2d::Color4B::WHITE) :Line(line), color_(color) {}
    ~Edge() {}

    const cocos2d::Color4B getColor()const { return color_; }
	
private:
    cocos2d::Color4B color_;
};

#endif // !LINE_H

