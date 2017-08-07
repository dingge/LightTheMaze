/********************************************************************
	date  :	2016/01/30   13:26

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef FULL_MAP_EFFECT_H
#define FULL_MAP_EFFECT_H

#include "cocos2d.h"
#include "Game/Macros.h"
#include "CCGL.h"

class FullMapEffect : public cocos2d::RenderTexture
{
public:
	static FullMapEffect* create(int w, int h);
protected:
	FullMapEffect(){}
	virtual ~FullMapEffect(){}

	virtual bool initWithWidthAndHeight(int w, int h, cocos2d::Texture2D::PixelFormat format);
	inline void drawPixel(GLubyte* buffer,int x,int y,int bufferWidth,const cocos2d::Color4B& fillcolor);
private:
};
#endif // !FULL_MAP_EFFECT_H
