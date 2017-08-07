#include "LightRadiusAction.h"
#include "Game/Light/LightNode.h"

USING_NS_CC;

//----------------------------------LightRadiusScaleBy----------------------------------
LightRadiusScaleBy* LightRadiusScaleBy::create( float deltaRadius,float duration )
{
	LightRadiusScaleBy* pAction = new LightRadiusScaleBy();
	pAction->initWithDuration(deltaRadius,duration);
	pAction->autorelease();
	return pAction;
}

LightRadiusScaleBy* LightRadiusScaleBy::clone() const 
{
	return LightRadiusScaleBy::create(m_deltaRadius,_duration);
}

LightRadiusScaleBy* LightRadiusScaleBy::reverse() const 
{
	return LightRadiusScaleBy::create(-m_deltaRadius,_duration);
}

void LightRadiusScaleBy::startWithTarget( Node *target )
{
	ActionInterval::startWithTarget(target);
	m_previousRadius = m_startRadius = static_cast<LightNode*>(target)->getLightMaxRadius();
}

void LightRadiusScaleBy::update( float time )
{
	if(_target)
	{
		float currentRadius = static_cast<LightNode*>(_target)->getLightMaxRadius();
		float diff = currentRadius - m_previousRadius;
		m_startRadius += diff;
		float newRadius = m_startRadius + (m_deltaRadius * time);
		static_cast<LightNode*>(_target)->setLightMaxRadius(newRadius);
		m_previousRadius = newRadius;
	}
}

bool LightRadiusScaleBy::initWithDuration( float deltaRadius,float duration )
{
	if (ActionInterval::initWithDuration(duration))
	{
		m_deltaRadius = deltaRadius;
		return true;
	}

	return false;
}

//----------------------------------LightRadiusScaleTo----------------------------------
LightRadiusScaleTo* LightRadiusScaleTo::create( float nextRadius,float duration )
{
	LightRadiusScaleTo* pAction = new LightRadiusScaleTo();
	pAction->initWithDuration(nextRadius,duration);
	pAction->autorelease();
	return pAction;
}

LightRadiusScaleTo* LightRadiusScaleTo::clone() const 
{
	return LightRadiusScaleTo::create(m_nextRadius,_duration);
}

void LightRadiusScaleTo::startWithTarget( Node *target )
{
	ActionInterval::startWithTarget(target);
	m_deltaRadius = m_nextRadius - static_cast<LightNode*>(target)->getLightMaxRadius();
}

bool LightRadiusScaleTo::initWithDuration( float nextRadius,float duration )
{
	if (ActionInterval::initWithDuration(duration))
	{
		m_nextRadius = nextRadius;
		return true;
	}

	return false;
}
