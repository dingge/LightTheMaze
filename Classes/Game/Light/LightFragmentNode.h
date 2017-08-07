/********************************************************************
	date  :	2016/01/30   13:11

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef LIGHTFRAGMENTNODE_H_
#define LIGHTFRAGMENTNODE_H_

#include "cocos2d.h"
#include "Game/Light/LightNode.h"
#include "Game/GameUtils/CollisionProcessNode.h"

/**
*	��֮��Ƭ�ڵ��ࣨ������ײ��⣩
*/
class LightFragmentNode : public CollisionProcessNode
{
public:
	static LightFragmentNode* create(const std::string& fileName, cocos2d::Color4B lightColor, float bodyRadius);

	//��дsetPosition��setScale��Ϊ����ȷ���¹��������Ϣ
	virtual void setPosition(const cocos2d::Vec2& pos)override;
	virtual void setPosition(float x, float y)override;
    virtual void setScale(float scale)override;

	//���ڻ�ȡ�󶨵Ĺ���
	LightNode* getLightNode() { return m_LightNode; }

	//�ռ�״̬
	bool isFragmentCollected()const { return m_isCollected; }
	void setFragmentCollected(bool isCollected);
	std::string getLargeTextureFileName()const;
public:
	virtual void processCollision(CollisionProcessNode* node, const ContactInfo& info);
	virtual void processCollision(LightSpriteNode* node, const ContactInfo& info);
	virtual void processCollision(MechanismCollisionNode* node, const ContactInfo& info);
protected:
	LightFragmentNode() :
		m_LightNode(nullptr),
		m_isCollected(false)
	{}
	~LightFragmentNode() {}

	bool init(const std::string& fileName, cocos2d::Color4B lightColor, float bodyRadius);
	void adjustLightNode();
private:
	LightNode* m_LightNode;             //�󶨵Ĺ���
	cocos2d::Sprite* fragment_sprite_;  //δ�ռ�״̬�µľ�����
	std::string m_textureFileName;      //������ͼ�ļ���
	bool m_isCollected;                 //�Ƿ����ռ���־
};

#endif // LIGHTFRAGMENTNODE_H_
