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
*	�⴦����
*	��ȡ�ϰ������ƹ�������Ķ����
*	ʹ��RenderTextureȥ�ݴ���ƽ��
*	ʹ��Shader�Խ���ü�+����ģ��+����˥��
*	��EffectLayer��������ֻ��Ʋ���ʾ
*/
class LightNode : public cocos2d::Node
{
public:
	virtual bool init()override;

	CREATE_FUNC(LightNode);

	//��Դ�ƶ�
	virtual void moveLight(const cocos2d::Vec2& delta);
	//��Դ�뾶����
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

	//����һ���뾶�ڵ���״
	void updateLight();
	//���µ�ͼλ��
	void updateMapPosition(const cocos2d::Vec2& mappos) { m_mapPos = mappos; }

	//����һ�����Ƿ��ڹ���������(pos����������)
	//��ΪRenderTexture֡������û�о���shader����
	bool isPointInLight(const cocos2d::Vec2& worldPos, const cocos2d::Vec2& lightPos);

    //�����Ƿ�����Ļ��
    bool isLightInScreen();
protected:
	LightNode();
	virtual ~LightNode();

	void createLight();
	void renderLight();

	//�ؽ��߶νǼ���
	void createShapeAngle();
	//�������߶��Ƿ��н��㣬���н����򷵻ؽ�����������ߵ����㳤��
	inline bool isCross(const Line& ray, const Line& segment, cocos2d::Vec2* crossPos, float* len);
	//���ƹ���Area(�������)
	void drawConcavePolygons(const cocos2d::Vec2& lightPos, cocos2d::Color4F color);
    //���ư�͸���Լ���Ӱ��
    void drawTranslucentAndShadow();
	//������ƵĹ⣬maxRadius�ǹ����뾶
	void handleTexture(float maxRadius);
	void setAlpha(unsigned int* pixel, const unsigned char alpha);
	unsigned char getAlpha(const unsigned int* pixel)const;
	//��������Shader
	void addGLShader(cocos2d::Node* node);
	//�ѹ�Դλ��ת������Ļ���꣨GL����ϵ��
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
	std::vector<cocos2d::Vec2> m_lightVec;      //�����յ㼯��
    std::vector<cocos2d::Vec2> m_penumbraVec;   //��Ӱ���㼯��
    DrawNodeSpec*            m_lightDraw;       //���ƹ���
    DrawNodeSpec*            m_translucentDraw; //���ư�͸������
    DrawNodeSpec*            m_shadowDraw;      //������Ӱ
	std::vector<Edge>        m_shapeVec;        //�ϰ�����״����
	std::vector<float>       m_shapeAngleVec;   //�ϰ�����ߴ�����ת�Ǽ���
	cocos2d::Vec2            m_lightPos;        //��λ�ã�����ڵ�ͼλ�ã�
    float                    m_lightRadius;     //��Դ�����С
	float                    m_lightMaxRangeRadius;  //�����շ�Χ
	cocos2d::Color4F         m_lightColor;      //����ɫ
	cocos2d::RenderTexture*	 m_rt;              //���ڴ������ʾ���ߵ���ͼ
	cocos2d::Vec2            m_mapPos;          //��ͼλ��
};
#endif // LIGHTNODE_H_

