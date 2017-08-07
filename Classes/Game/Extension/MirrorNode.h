/********************************************************************
	date  :	2016/01/30   13:26

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef MIRROR_NODE_H
#define MIRROR_NODE_H

#include "Game/Macros.h"
#include "Game/GameUtils/Line.h"
#include "Game/GameUtils/ShapeManage.h"
#include "cocos2d.h"
#include "renderer/ccShaders.h"

class Tiled;
class LightNode;

/**
 *	镜像光照处理类
 *	TODO:施工中
 */
class MirrorNode : public cocos2d::Node
{
public:
	virtual bool init()override;
	static MirrorNode* create(Line mirrorPos);

	cocos2d::RenderTexture* getRenderTexture()const {return m_rt;}

// 	void updateNearByLight();
// 	void updateNearbyShape(const ShapeManage* manage);
protected:
	MirrorNode(Line mirrorPos);
	virtual ~MirrorNode();

// 	void createLight();
// 	void renderLight();
// 
// 	//重建线段角集合
// 	void createShapeAngle();
// 	//射线与线段是否有交点，若有交点则返回交点坐标和射线到交点长度
// 	bool isCross(const Line& ray,const Line& segment,Vec2* crossPos,float* len);
// 	//绘制光线Area(凹多边形)
// 	void drawConcavePolygons(const Vec2& lightPos,Color4F color);
// 	//处理绘制的光，maxRadius是光最大半径
// 	void handleTexture(int maxRadius);
// 	//给结点添加Shader
// 	void addGLShader(Node* node);
protected:
	//镜子法线方向(单位向量)
	cocos2d::Vec2 m_mirrorNormal;	
	//镜子位置(两点,以中点为位置中心,相对于地图位置)
	Line m_mirrorPosition;		
	//绘制光线(画一次清一次)
	cocos2d::DrawNode* m_lightDraw;
	//镜像光源集合(当前所有亮区包含镜子的光源的镜像位置)
	std::vector<LightNode*> m_lightMirrorVec;		
	//障碍物形状集合
	std::map<LightNode*,std::vector<Line>> m_shapeVecMap;	
	//障碍物光线待测旋转角集合(6*shape数)
	std::map<LightNode*,std::vector<float>> m_shapeAngleVecMap;
	//光线终点集合
	std::map<LightNode*,std::vector<cocos2d::Vec2>> m_lightVecMap;
	//用于处理和显示光线的贴图
	cocos2d::RenderTexture* m_rt;
private:
	MirrorNode(const MirrorNode&) = delete; 
	MirrorNode &operator =(const MirrorNode&) = delete;
};
#endif // !MIRROR_NODE_H
