/********************************************************************
	date  :	2016/01/30   14:55

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef BEACON_NODE_H
#define BEACON_NODE_H

#include "cocos2d.h"
#include "Game/Mechanism/MechanismCollisionNode/MechanismCollisionNode.h"

class LightSpriteNode;

#define BEACON_BG_FILE_NAME "item/beacon.png"
#define BEACON_PARTICLE_FILE_NAME "particle/light.plist"

/**
 *	����
 *  ����������أ����������Ϩ����¼�
 */
class BeaconNode : public MechanismCollisionNode
{
public:
	static BeaconNode* create(const std::string& bgFileName,const std::string& particleFileName);

	//��������
	void lit();

	//Ϩ�����
	void extinguish();

	void processCollision(LightSpriteNode* node, const ContactInfo& info);
protected:
	BeaconNode();
	virtual ~BeaconNode();

	virtual bool init(const std::string& bgFileName,const std::string& particleFileName);
private:
	cocos2d::Sprite* m_bg;
	cocos2d::ParticleSystemQuad* m_particle;
	std::string m_particleFileName;
	bool m_isLitted;
};

#endif // !BEACON_NODE_H
