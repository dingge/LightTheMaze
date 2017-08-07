/********************************************************************
	date  :	2016/01/30   13:26

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef EFFECTLAYER_H_
#define EFFECTLAYER_H_

#include "cocos2d.h"
#include "Game/Macros.h"

/**
 *	效果层
 *	包含一个遮罩层,叠放次序(由低到高):地图层,效果层(遮罩层),人物层
 *	遮罩层用于覆盖视野,根据各种发光物质改变视野（由Shader根据alpha值反向绘制而成）
 */
class EffectLayer :public cocos2d::Layer
{
public:
	static EffectLayer* create();

	//调用将清空遮罩，等待写入
	void drawEffectBegin();
	//调用将添加一个光照处理过的RenderTexture，并作为一个遮罩绘制到遮罩层上
	void addLightEffect(cocos2d::RenderTexture* rt);
	//调用将关闭输入并用Shader处理遮罩
	void drawEffectEnd();

	//测试一个点是否在光照区域内(pos是世界坐标)
	bool isPointInLight(const cocos2d::Vec2& worldPos);

	void closeDark();
private:
	EffectLayer();
	virtual ~EffectLayer();
	
	virtual bool init()override;
	
	//给遮罩添加Shader，这里处理alpha值反向
	void addGLShader(cocos2d::Node* node);
private:
	cocos2d::RenderTexture*	m_shadeRT;		//遮罩层（暗层）
	bool m_isShine;
};

#endif // EFFECTLAYER_H_
