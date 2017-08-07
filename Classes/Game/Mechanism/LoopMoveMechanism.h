/********************************************************************
	date  :	2016/01/30   13:29

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef LOOPMOVEMECHANISM_H_
#define LOOPMOVEMECHANISM_H_

#include "Game/Mechanism/BaseMechanism.h"
#include "Game/Mechanism/MechanismCollisionNode/MoveMechanism.h"
#include "Game/Extension/QuadTree.h"
#include "Game/GameUtils/Path.h"

/**
*   ѭ���ƶ����أ��޽�����
*   ����ָ���˶�·���˶�
*   ��������󣬿���������·
*/
class LoopMoveMechanism : public BaseMechanism
{
public:
	static LoopMoveMechanism* create(const NodeGroupPtr& line_group, const float begin_rate, const Path& path, float duration, bool is_reverse = false);

	virtual void update(float delta)override;
protected:
	LoopMoveMechanism() :
        mechanism_node_(nullptr),
        duration_(0.0f),
        escape_time_(0.0f),
        is_reverse_(false),
        is_positive_(true)
	{}
	virtual ~LoopMoveMechanism() {}

	bool init(const NodeGroupPtr& line_group, const float begin_rate, const Path& path, float duration, bool is_reverse);
private:
	MoveMechanism* mechanism_node_;
    cocos2d::Vec2 pre_pos_; //��һ·����
    Path path_;             //�˶�·��
    float duration_;        //�˶���ʱ��
    float escape_time_;     //���ʱ�䲻��������ʱ�����ţ����Ǳ���˵�ǰ���·������·���ĳ��ȱ�
    bool is_reverse_;       //���߽��Ƿ����˶�
    bool is_positive_;      //��־��ǰ�Ƿ������ƶ�
};
#endif // LOOPMOVEMECHANISM_H_
