/********************************************************************
	date  :	2016/01/30   13:24

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef LIGHT_COLOR_ACTION_H
#define LIGHT_COLOR_ACTION_H

#include "cocos2d.h"

/**
 *	光颜色变化类
 */
class LightColorActionBy : public cocos2d::ActionInterval
{
public:
	static LightColorActionBy* create(GLshort deltaColorRed,
		GLshort deltaColorGreen,
		GLshort deltaColorBlue,
		GLshort deltaColorAlpha,
		float duration);

	virtual LightColorActionBy* clone()const override;
	virtual LightColorActionBy* reverse()const override;
	virtual void startWithTarget(cocos2d::Node *target)override;
	virtual void update(float time)override;
protected:
	LightColorActionBy():
		m_startColorR(0),m_deltaColorR(0),
		m_startColorG(0),m_deltaColorG(0),
		m_startColorB(0),m_deltaColorB(0),
		m_startColorA(0),m_deltaColorA(0)
	{}
	virtual ~LightColorActionBy(){}

	bool initWithDuration(GLshort deltaColorRed,
		GLshort deltaColorGreen,
		GLshort deltaColorBlue,
		GLshort deltaColorAlpha,
		float duration);
private:
	GLshort m_startColorR;
	GLshort m_startColorG;
	GLshort m_startColorB;
	GLshort m_startColorA;

	GLshort m_deltaColorR;
	GLshort m_deltaColorG;
	GLshort m_deltaColorB;
	GLshort m_deltaColorA;
private:
	LightColorActionBy(const LightColorActionBy &) = delete;
	LightColorActionBy &operator =(const LightColorActionBy &) = delete;
};

/**
 *	光颜色变化类
 */
class LightColorActionTo : public cocos2d::ActionInterval
{
public:
	static LightColorActionTo* create(GLubyte deltaColorRed, GLubyte deltaColorGreen, GLubyte deltaColorBlue, GLubyte deltaColorAlpha, float duration);

	virtual LightColorActionTo* clone()const override;
	virtual LightColorActionTo* reverse()const override;
	virtual void startWithTarget(cocos2d::Node *target)override;
	virtual void update(float time)override;

protected:
	LightColorActionTo(){}
	virtual ~LightColorActionTo(){}

	bool initWithDuration(GLubyte deltaColorRed, GLubyte deltaColorGreen, GLubyte deltaColorBlue, GLubyte deltaColorAlpha, float duration);
protected:
	cocos2d::Color4B m_endColor;
	cocos2d::Color4B m_startColor;
private:
	LightColorActionTo(const LightColorActionTo &) = delete;
	LightColorActionTo &operator =(const LightColorActionTo &) = delete;
};
#endif // !LIGHT_COLOR_ACTION_H
