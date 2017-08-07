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
 *	路径类
 *  包含由一组点组成的路径，提供路径插值等功能
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

    ///根据比例获取路径上线性插值的点
    cocos2d::Vec2 getLinearLerpPos(float rate)const;

    ///反转路径
    void reverse();

protected:
    ///重新计算路径总长度
    void calculateLength();

private:
    std::vector<cocos2d::Vec2> point_array_;
    float path_length_;
};
#endif // PATH_H_