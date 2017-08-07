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
 *	Ч����
 *	����һ�����ֲ�,���Ŵ���(�ɵ͵���):��ͼ��,Ч����(���ֲ�),�����
 *	���ֲ����ڸ�����Ұ,���ݸ��ַ������ʸı���Ұ����Shader����alphaֵ������ƶ��ɣ�
 */
class EffectLayer :public cocos2d::Layer
{
public:
	static EffectLayer* create();

	//���ý�������֣��ȴ�д��
	void drawEffectBegin();
	//���ý����һ�����մ������RenderTexture������Ϊһ�����ֻ��Ƶ����ֲ���
	void addLightEffect(cocos2d::RenderTexture* rt);
	//���ý��ر����벢��Shader��������
	void drawEffectEnd();

	//����һ�����Ƿ��ڹ���������(pos����������)
	bool isPointInLight(const cocos2d::Vec2& worldPos);

	void closeDark();
private:
	EffectLayer();
	virtual ~EffectLayer();
	
	virtual bool init()override;
	
	//���������Shader�����ﴦ��alphaֵ����
	void addGLShader(cocos2d::Node* node);
private:
	cocos2d::RenderTexture*	m_shadeRT;		//���ֲ㣨���㣩
	bool m_isShine;
};

#endif // EFFECTLAYER_H_
