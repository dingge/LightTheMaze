/********************************************************************
	date  :	2016/01/30   15:30

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef POTALMECHAINISMCELL_H_
#define POTALMECHAINISMCELL_H_

#include "Game/Mechanism/MechanismCollisionNode/MechanismCollisionNode.h"

class LightSpriteNode;

/*传送阵碰撞掩码，与光精灵不发生碰撞反应*/
#define POTAL_COLLISION_BITMASK 0x00000000

/**
*	传送阵机关单元
*  包含一个粒子，包含刚体
*  处理与光精灵的接触检测并设置相应信号
*/
class PotalMechainismCell : public MechanismCollisionNode
{
public:
	static PotalMechainismCell* create(const std::string& fileName);

	virtual void processCollision(LightSpriteNode* node, const ContactInfo& info)override;
	virtual void update(float delta)override;

	//重置上一次的位置为当前位置
	void resetPosition();
protected:
	PotalMechainismCell() :
		m_isTrigger(false),
		m_particle(nullptr),
		m_isLeaved(false)
	{}
	virtual ~PotalMechainismCell() {}

	bool init(const std::string& fileName);
private:
	cocos2d::Vec2 m_prePos;
public:
	bool m_isTrigger; //标志是否已接触
	bool m_isLeaved; //标志是否正在离开传送阵，避免多次接触测试
	cocos2d::ParticleSystemQuad* m_particle;
};
#endif // POTALMECHAINISMCELL_H_