/********************************************************************
	date  :	2016/01/30   13:14

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef LIGHTNODE_H_
#define LIGHTNODE_H_

#include "Game/Macros.h"
#include "Game/GameUtils/Line.h"
#include "Game/GameUtils/ShapeManage.h"
#include "Game/GameUtils/DrawNodeSpec.h"
#include "cocos2d.h"
#include "renderer/ccShaders.h"

/**
*	光处理类
*	获取障碍并绘制光照区域的多边形
*	使用RenderTexture去暂存绘制结果
*	使用Shader对结果裁剪+径向模糊+光照衰减
*	由EffectLayer类进行遮罩绘制并显示
*/
class LightNode : public cocos2d::Node
{
public:
	virtual bool init()override;

	CREATE_FUNC(LightNode);

	//光源移动
	virtual void moveLight(const cocos2d::Vec2& delta);
	//光源半径缩放
	void scaleLightRadiusBy(float deltaRadius, float duration);
	void scaleLightRadiusTo(float nextRadius, float duration);

	const cocos2d::Vec2& getLightPos()const { return m_lightPos; }
	void setLightPos(const cocos2d::Vec2& pos) { m_lightPos = pos; }
	inline const cocos2d::Vec2 getLightPosOnScreen() { return convertToScreenPos(m_lightPos); }

    inline void setLightRadius(float radius) { m_lightRadius = radius; }

	const float getLightMaxRadius()const { return m_lightMaxRangeRadius; }
	inline void setLightMaxRadius(float radius) { m_lightMaxRangeRadius = radius; }

	const cocos2d::Color4F getLightColor()const { return m_lightColor; }
	void setLightColor(const cocos2d::Color4F& color) { m_lightColor = color; }

	cocos2d::RenderTexture* getRenderTexture()const { return m_rt; }

	//更新一定半径内的形状
	void updateLight();
	//更新地图位置
	void updateMapPosition(const cocos2d::Vec2& mappos) { m_mapPos = mappos; }

	//测试一个点是否在光照区域内(pos是世界坐标)
	//因为RenderTexture帧缓冲区没有经过shader处理
	bool isPointInLight(const cocos2d::Vec2& worldPos, const cocos2d::Vec2& lightPos);

    //光结点是否在屏幕内
    bool isLightInScreen();
protected:
	LightNode();
	virtual ~LightNode();

	void createLight();
	void renderLight();

	//重建线段角集合
	void createShapeAngle();
	//射线与线段是否有交点，若有交点则返回交点坐标和射线到交点长度
	inline bool isCross(const Line& ray, const Line& segment, cocos2d::Vec2* crossPos, float* len);
	//绘制光线Area(凹多边形)
	void drawConcavePolygons(const cocos2d::Vec2& lightPos, cocos2d::Color4F color);
    //绘制半透明以及阴影区
    void drawTranslucentAndShadow();
	//处理绘制的光，maxRadius是光最大半径
	void handleTexture(float maxRadius);
	void setAlpha(unsigned int* pixel, const unsigned char alpha);
	unsigned char getAlpha(const unsigned int* pixel)const;
	//给结点添加Shader
	void addGLShader(cocos2d::Node* node);
	//把光源位置转换到屏幕坐标（GL坐标系）
	cocos2d::Vec2 convertToScreenPos(const cocos2d::Vec2& pos);

protected:
	struct RayCastResult
	{
		int index_of_line_vec;
		float ray_length;
		bool operator < (const RayCastResult& right)const
		{
			return ray_length > right.ray_length;
		}
	};
protected:
	std::vector<cocos2d::Vec2> m_lightVec;      //光线终点集合
    std::vector<cocos2d::Vec2> m_penumbraVec;   //半影区点集合
    DrawNodeSpec*            m_lightDraw;       //绘制光线
    DrawNodeSpec*            m_translucentDraw; //绘制半透明光线
    DrawNodeSpec*            m_shadowDraw;      //绘制阴影
	std::vector<Edge>        m_shapeVec;        //障碍物形状集合
	std::vector<float>       m_shapeAngleVec;   //障碍物光线待测旋转角集合
	cocos2d::Vec2            m_lightPos;        //光位置（相对于地图位置）
    float                    m_lightRadius;     //光源本身大小
	float                    m_lightMaxRangeRadius;  //最大光照范围
	cocos2d::Color4F         m_lightColor;      //光颜色
	cocos2d::RenderTexture*	 m_rt;              //用于处理和显示光线的贴图
	cocos2d::Vec2            m_mapPos;          //地图位置
};
#endif // LIGHTNODE_H_

