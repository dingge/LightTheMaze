/********************************************************************
	date  :	2016/01/30   13:25

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef LIGHT_RADIUS_ACTION_H
#define LIGHT_RADIUS_ACTION_H

#include "cocos2d.h"

/**
 *	光源半径缩放动作类
 *	传入参数：半径偏移、动作持续时间
 */
class LightRadiusScaleBy : public cocos2d::ActionInterval
{
public:
	static LightRadiusScaleBy* create(float deltaRadius,float duration);

	virtual LightRadiusScaleBy* clone()const override;
	virtual LightRadiusScaleBy* reverse()const override;
	virtual void startWithTarget(cocos2d::Node *target)override;
	virtual void update(float time)override;
protected:
	LightRadiusScaleBy():
		m_startRadius(0.0f),
		m_deltaRadius(0.0f),
		m_previousRadius(0.0f)
	{}
	virtual ~LightRadiusScaleBy(){}

	bool initWithDuration(float deltaRadius,float duration);
protected:
	float m_startRadius;
	float m_deltaRadius;
	float m_previousRadius;
private:
	LightRadiusScaleBy(const LightRadiusScaleBy &) = delete;
	LightRadiusScaleBy &operator =(const LightRadiusScaleBy &) = delete;
};

/**
 *	光源半径缩放动作类
 *	传入参数：下一个半径、动作持续时间
 */
class LightRadiusScaleTo : public LightRadiusScaleBy
{
public:
	static LightRadiusScaleTo* create(float nextRadius,float duration);

	virtual void startWithTarget(cocos2d::Node *target) override;
	virtual LightRadiusScaleTo* clone()const override;
protected:
	LightRadiusScaleTo():
		m_nextRadius(0.0f)
	{}
	virtual ~LightRadiusScaleTo(){}

	bool initWithDuration(float nextRadius,float duration);
private:
	float m_nextRadius;	///<下一个半径
private:
	LightRadiusScaleTo(const LightRadiusScaleTo &) = delete;
	LightRadiusScaleTo &operator =(const LightRadiusScaleTo &) = delete;
};
#endif // !LIGHT_RADIUS_ACTION_H
