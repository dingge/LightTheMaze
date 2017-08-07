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

/*��������ײ���룬��⾫�鲻������ײ��Ӧ*/
#define POTAL_COLLISION_BITMASK 0x00000000

/**
*	��������ص�Ԫ
*  ����һ�����ӣ���������
*  ������⾫��ĽӴ���Ⲣ������Ӧ�ź�
*/
class PotalMechainismCell : public MechanismCollisionNode
{
public:
	static PotalMechainismCell* create(const std::string& fileName);

	virtual void processCollision(LightSpriteNode* node, const ContactInfo& info)override;
	virtual void update(float delta)override;

	//������һ�ε�λ��Ϊ��ǰλ��
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
	bool m_isTrigger; //��־�Ƿ��ѽӴ�
	bool m_isLeaved; //��־�Ƿ������뿪�����󣬱����νӴ�����
	cocos2d::ParticleSystemQuad* m_particle;
};
#endif // POTALMECHAINISMCELL_H_