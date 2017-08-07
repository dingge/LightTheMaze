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
 *	�߶���
 *	��״����С��Ԫ���������˵��һ��id��id����Ψһ��ʶ�߶ε�����
 *	id�ӵ�ͼ�������룬ÿ��id��Ӧһ���Զ���ĵ�Ԫ�����ܶ�Ӧ�����߶�
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

	//����ID������ID��Ϊ�߶ε��б�ʽ
	//�����ж��߶��Ƿ��غ�
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
	cocos2d::Vec2 _p1;			///<�߶ε�һ����
	cocos2d::Vec2 _p2;			///<�߶εڶ�����
	int _id;			        ///<�߶�ר��ID��ǰ2�ֽڱ�ʾ������Ʒ��name����2�ֽڱ�ʾ���
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

