#include "LightColorAction.h"
#include "Game/Light/LightNode.h"

USING_NS_CC;

//----------------------------------LightColorActionBy----------------------------------
LightColorActionBy* LightColorActionBy::create( GLshort deltaColorRed,GLshort deltaColorGreen,GLshort deltaColorBlue,GLshort deltaColorAlpha,float duration )
{
	LightColorActionBy* pAction = new LightColorActionBy();
	pAction->initWithDuration(deltaColorRed,deltaColorGreen,deltaColorBlue,deltaColorAlpha,duration);
	pAction->autorelease();
	return pAction;
}

LightColorActionBy* LightColorActionBy::clone() const 
{
	return LightColorActionBy::create((GLubyte)m_deltaColorR,
										(GLubyte)m_deltaColorG,
										(GLubyte)m_deltaColorB,
										(GLubyte)m_deltaColorA,
										_duration);
}

LightColorActionBy* LightColorActionBy::reverse() const 
{
	return LightColorActionBy::create(-m_deltaColorR,
										-m_deltaColorG,
										-m_deltaColorB,
										-m_deltaColorA,_duration);
}

void LightColorActionBy::startWithTarget( Node *target )
{
	ActionInterval::startWithTarget(target);
	Color4B color= Color4B(static_cast<LightNode*>(target)->getLightColor());
	m_startColorR = color.r;
	m_startColorG = color.g;
	m_startColorB = color.b;
	m_startColorA = color.a;
}

void LightColorActionBy::update( float time )
{
	if(_target)
	{
		Color4B newColor = Color4B((GLubyte)(m_startColorR + (m_deltaColorR * time)),
									(GLubyte)(m_startColorG + (m_deltaColorG * time)),
									(GLubyte)(m_startColorB + (m_deltaColorB * time)),
									(GLubyte)(m_startColorA + (m_deltaColorA * time)));
		static_cast<LightNode*>(_target)->setLightColor(Color4F(newColor));
	}
}

bool LightColorActionBy::initWithDuration( GLshort deltaColorRed,GLshort deltaColorGreen,GLshort deltaColorBlue,GLshort deltaColorAlpha,float duration )
{
	if (ActionInterval::initWithDuration(duration))
	{
		m_deltaColorR = deltaColorRed;
		m_deltaColorG = deltaColorGreen;
		m_deltaColorB = deltaColorBlue;
		m_deltaColorA = deltaColorAlpha;
		return true;
	}

	return false;
}

//----------------------------------LightColorActionTo----------------------------------
LightColorActionTo* LightColorActionTo::create(GLubyte deltaColorRed, GLubyte deltaColorGreen, GLubyte deltaColorBlue, GLubyte deltaColorAlpha, float duration )
{
	LightColorActionTo *pAction = new LightColorActionTo();
	pAction->initWithDuration(deltaColorRed,deltaColorGreen,deltaColorBlue,deltaColorAlpha,duration);
	pAction->autorelease();

	return pAction;
}

LightColorActionTo* LightColorActionTo::clone() const 
{
	return LightColorActionTo::create(m_endColor.r,m_endColor.g,m_endColor.b,m_endColor.a,_duration);
}

LightColorActionTo* LightColorActionTo::reverse() const 
{
 	CCASSERT(false, "reverse() not supported in LightColorActionTo");
 	return nullptr;
}

void LightColorActionTo::startWithTarget( Node *target )
{
	ActionInterval::startWithTarget(target);
	if (_target)
	{
		m_startColor = Color4B(static_cast<LightNode*>(_target)->getLightColor());
	}
}

void LightColorActionTo::update( float time )
{
	if (_target)
	{
		Color4B newColor = Color4B((GLubyte)(m_startColor.r + (m_endColor.r - m_startColor.r) * time),
			(GLubyte)(m_startColor.g + (m_endColor.g - m_startColor.g) * time),
			(GLubyte)(m_startColor.b + (m_endColor.b - m_startColor.b) * time),
			(GLubyte)(m_startColor.a + (m_endColor.a - m_startColor.a) * time));
		static_cast<LightNode*>(_target)->setLightColor(Color4F(newColor));
	}  
}

bool LightColorActionTo::initWithDuration(GLubyte deltaColorRed, GLubyte deltaColorGreen, GLubyte deltaColorBlue, GLubyte deltaColorAlpha, float duration )
{
	if (ActionInterval::initWithDuration(duration))
	{
		m_endColor = Color4B(deltaColorRed, deltaColorGreen, deltaColorBlue,deltaColorAlpha);
		return true;
	}

	return false;
}
