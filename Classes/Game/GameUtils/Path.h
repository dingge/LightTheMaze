/********************************************************************
	date  :	2016/03/28   23:23

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef PATH_H_
#define PATH_H_

#include "cocos2d.h"
#include "Game/GameUtils/Line.h"

/**
 *	·����
 *  ������һ�����ɵ�·�����ṩ·����ֵ�ȹ���
 */
class Path
{
public:
    Path();
    Path(const std::vector<Line>& right);
	Path(const std::vector<cocos2d::Vec2>& right);
    ~Path();

    cocos2d::Vec2 getBeginPos()const;

    cocos2d::Vec2 getEndPos()const;

    float getPathLength()const;

    ///���ݱ�����ȡ·�������Բ�ֵ�ĵ�
    cocos2d::Vec2 getLinearLerpPos(float rate)const;

    ///��ת·��
    void reverse();

protected:
    ///���¼���·���ܳ���
    void calculateLength();

private:
    std::vector<cocos2d::Vec2> point_array_;
    float path_length_;
};
#endif // PATH_H_