/********************************************************************
	date  :	2016/01/30   14:50

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef TRIGGERMECHANISM_H_
#define TRIGGERMECHANISM_H_

#include "Game/Mechanism/BaseMechanism.h"

class KeyMechanism;

/*Կ�׾��������ļ���*/
#define KEY_PARTICLE_FILE_NAME "particle/keyMechanism.plist"

/**
*  ������������
*  �������غ�������
*/
class TriggerMechanism : public BaseMechanism
{
public:
	virtual void update(float delta)override;
protected:
	TriggerMechanism() :
		key_(nullptr),
		is_opened_(false)
	{}
	virtual ~TriggerMechanism() {}

	bool init(const cocos2d::Vec2& keyPos);
    /**
     *	�������أ���д����Ӧ�ýӿ�
     *  ����ֵ���Ƿ����ɹ�
     */
    virtual bool openMechanism() { return true; }
    /**
    *	�رջ��أ���д����Ӧ�ýӿ�
    *   ����ֵ���Ƿ�رճɹ�
    */
    virtual bool closeMechanism() { return true; }
protected:
	KeyMechanism* key_;
	bool is_opened_; //���ؿ�����־����������ȫ��Ӧ֮�����ô�״̬
};
#endif // TRIGGERMECHANISM_H_