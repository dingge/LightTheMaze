#include "LightFragmentNode.h"
#include "Game/GameUtils/GeneralCircleBody.h"
#include "Game/Light/LightSpriteNode.h"
#include "Game/Mechanism/MechanismCollisionNode/MechanismCollisionNode.h"

USING_NS_CC;

//----------------------------------LightFragmentNode----------------------------------
LightFragmentNode* LightFragmentNode::create(const std::string& fileName, Color4B lightColor, float bodyRadius)
{
	LightFragmentNode *lightFragment = new (std::nothrow) LightFragmentNode();
	if (lightFragment && lightFragment->init(fileName, lightColor, bodyRadius))
	{
		lightFragment->autorelease();
		return lightFragment;
	}
	CC_SAFE_DELETE(lightFragment);
	return nullptr;
}

bool LightFragmentNode::init(const std::string& fileName, Color4B lightColor, float bodyRadius)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!CollisionProcessNode::init());

		m_textureFileName = fileName;

		m_LightNode = LightNode::create();
        m_LightNode->setLightRadius(bodyRadius);
		m_LightNode->setLightColor(Color4F(lightColor));
		this->addChild(m_LightNode);

		fragment_sprite_ = Sprite::create(m_textureFileName);
		fragment_sprite_->setColor(Color3B(lightColor));
		fragment_sprite_->setScale(bodyRadius*2.0f / fragment_sprite_->getContentSize().width);
		this->addChild(fragment_sprite_);

        this->setColor(Color3B(lightColor));

		//质量无穷大
		auto pBody = GeneralCircleBody::createCircleBody(bodyRadius, PHYSICS_INFINITY);
		this->setPhysicsBody(pBody);

		this->setFragmentCollected(false);

		adjustLightNode();

		this->scheduleUpdate();

		bRet = true;
	} while (0);
	return bRet;
}

void LightFragmentNode::processCollision(CollisionProcessNode* node, const ContactInfo& info)
{
	node->processCollision(this, info);
}

void LightFragmentNode::processCollision(LightSpriteNode* node, const ContactInfo& info)
{
	//log("LightFragmentNode...LightSpriteNode collision!");
	//由光精灵处理。。。
	node->processCollision(this,info);
}

void LightFragmentNode::processCollision(MechanismCollisionNode* node, const ContactInfo& info)
{
	//log("LightFragmentNode...MechanismCollisionNode collision!");
	//由机关处理。。。
	//node->processCollision(this, info);
}

void LightFragmentNode::setFragmentCollected(bool isCollected)
{
	if (m_isCollected == true)
		return;

	m_isCollected = isCollected;
	if (m_isCollected == false)
	{
		m_LightNode->setLightMaxRadius(LIGHT_STANDRAD_RADIUS*0.1f);
	}
	else if (m_LightNode->getNumberOfRunningActions() == 0)
	{
		m_LightNode->scaleLightRadiusTo(LIGHT_STANDRAD_RADIUS, 3.0f);
		fragment_sprite_->initWithFile("fragment_sprite.png");
		adjustLightNode();
	}
}

std::string LightFragmentNode::getLargeTextureFileName() const
{
	std::string largeTextureFileName = m_textureFileName;
	int dotpos = largeTextureFileName.find('.');
	largeTextureFileName.erase(dotpos, largeTextureFileName.length() - dotpos);
	largeTextureFileName += "_large.png";
	return largeTextureFileName;
}

void LightFragmentNode::setPosition(const cocos2d::Vec2& pos)
{
	setPosition(pos.x, pos.y);
}

void LightFragmentNode::setPosition(float x, float y)
{
	Node::setPosition(x, y);
	//把物理运算结果赋予光的位置
	m_LightNode->setLightPos(this->getPosition());
	adjustLightNode();
}

void LightFragmentNode::adjustLightNode()
{
	if (m_LightNode)
	{
		//这是把渲染光照的画布一直固定在世界坐标的原点
		Vec2 lightNodePos = this->convertToNodeSpace(Vec2::ZERO);
		m_LightNode->setPosition(lightNodePos);
	}
}

void LightFragmentNode::setScale(float scale)
{
    Node::setScale(scale);

    if (m_LightNode)
    {
        //画布需要维持原大小
        m_LightNode->setScale(1.0f / scale);
    }
}
