/********************************************************************
	date  :	2016/01/30   18:28

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef FRAGMENTSHOW_H_
#define FRAGMENTSHOW_H_

#include "cocos2d.h"
#include "Game/Light/LightFragmentNode.h"

class FragmentShow : public cocos2d::Layer
{
public:
	static FragmentShow* create(const std::vector<LightFragmentNode*>& fragmentVec);

protected:
	FragmentShow():
		m_number(0)
	{}
	virtual ~FragmentShow(){}

	bool init(const std::vector<LightFragmentNode*>& fragmentVec);

	void createbyNumber(const std::vector<LightFragmentNode*>& fragmentVec);

	void addSprite(float x, float y, const LightFragmentNode* p);

	void addGLShader(cocos2d::Node* node);
private:
	int m_number;
};
#endif // FRAGMENTSHOW_H_

