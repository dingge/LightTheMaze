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
*	光之碎片节点类（附加碰撞检测）
*/
class LightFragmentNode : public CollisionProcessNode
{
public:
	static LightFragmentNode* create(const std::string& fileName, cocos2d::Color4B lightColor, float bodyRadius);

	//重写setPosition和setScale，为了正确更新光照相关信息
	virtual void setPosition(const cocos2d::Vec2& pos)override;
	virtual void setPosition(float x, float y)override;
    virtual void setScale(float scale)override;

	//用于获取绑定的光结点
	LightNode* getLightNode() { return m_LightNode; }

	//收集状态
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
	LightNode* m_LightNode;             //绑定的光结点
	cocos2d::Sprite* fragment_sprite_;  //未收集状态下的精灵结点
	std::string m_textureFileName;      //精灵贴图文件名
	bool m_isCollected;                 //是否已收集标志
};

#endif // LIGHTFRAGMENTNODE_H_
