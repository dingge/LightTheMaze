/********************************************************************
	date  :	2016/05/09   13:54

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef UTILS_H_
#define UTILS_H_

#include "cocos2d.h"

float getFloatMod(float dividend, float divisor);

cocos2d::Value getDictValue(const cocos2d::ValueMap& dic, const std::string& element_name, const cocos2d::Value default_value);

#endif // UTILS_H_