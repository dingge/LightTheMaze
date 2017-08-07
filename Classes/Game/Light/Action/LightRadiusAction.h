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
 *	��Դ�뾶���Ŷ�����
 *	����������뾶ƫ�ơ���������ʱ��
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
 *	��Դ�뾶���Ŷ�����
 *	�����������һ���뾶����������ʱ��
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
	float m_nextRadius;	///<��һ���뾶
private:
	LightRadiusScaleTo(const LightRadiusScaleTo &) = delete;
	LightRadiusScaleTo &operator =(const LightRadiusScaleTo &) = delete;
};
#endif // !LIGHT_RADIUS_ACTION_H
