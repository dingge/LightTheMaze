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
 *	������մ�����
 *	TODO:ʩ����
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
// 	//�ؽ��߶νǼ���
// 	void createShapeAngle();
// 	//�������߶��Ƿ��н��㣬���н����򷵻ؽ�����������ߵ����㳤��
// 	bool isCross(const Line& ray,const Line& segment,Vec2* crossPos,float* len);
// 	//���ƹ���Area(�������)
// 	void drawConcavePolygons(const Vec2& lightPos,Color4F color);
// 	//������ƵĹ⣬maxRadius�ǹ����뾶
// 	void handleTexture(int maxRadius);
// 	//��������Shader
// 	void addGLShader(Node* node);
protected:
	//���ӷ��߷���(��λ����)
	cocos2d::Vec2 m_mirrorNormal;	
	//����λ��(����,���е�Ϊλ������,����ڵ�ͼλ��)
	Line m_mirrorPosition;		
	//���ƹ���(��һ����һ��)
	cocos2d::DrawNode* m_lightDraw;
	//�����Դ����(��ǰ���������������ӵĹ�Դ�ľ���λ��)
	std::vector<LightNode*> m_lightMirrorVec;		
	//�ϰ�����״����
	std::map<LightNode*,std::vector<Line>> m_shapeVecMap;	
	//�ϰ�����ߴ�����ת�Ǽ���(6*shape��)
	std::map<LightNode*,std::vector<float>> m_shapeAngleVecMap;
	//�����յ㼯��
	std::map<LightNode*,std::vector<cocos2d::Vec2>> m_lightVecMap;
	//���ڴ������ʾ���ߵ���ͼ
	cocos2d::RenderTexture* m_rt;
private:
	MirrorNode(const MirrorNode&) = delete; 
	MirrorNode &operator =(const MirrorNode&) = delete;
};
#endif // !MIRROR_NODE_H
