/********************************************************************
	date  :	2016/01/30   14:55

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef POTALMECHAINISM_H_
#define POTALMECHAINISM_H_

#include "Game/Mechanism/MechanismCollisionNode/PotalMechainismCell.h"

/*�����������ļ�*/
#define POTAL_PARTICLE_FILE_NAME "particle/Phoenix.plist"

/*����״̬*/
enum class PotalFlag
{
	NONE,
	TRIGGER_BEGIN,
	TRIGGER_END
};

/**
 *	��������
 *  �����������͵�Ԫ
 *  ���û���DrawNode
 *  ���ݵ�Ԫ���źŴ��������ת
 */
class PotalMechainism : public cocos2d::Node
{
public:
	static PotalMechainism* create(const cocos2d::Vec2& beginPos,const cocos2d::Vec2& endPos);
	virtual void update(float delta)override;
protected:
	PotalMechainism():
		m_begin(nullptr),
		m_end(nullptr),
		m_flag(PotalFlag::NONE)
	{}
	virtual ~PotalMechainism(){}

	bool init(const cocos2d::Vec2& beginPos,const cocos2d::Vec2& endPos);

private:
	PotalMechainismCell* m_begin;
	PotalMechainismCell* m_end;
	PotalFlag m_flag;
};

#endif // POTALMECHAINISM_H_