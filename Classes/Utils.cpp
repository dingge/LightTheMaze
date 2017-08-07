#include "Utils.h"

float getFloatMod(float dividend, float divisor)
{
    while (dividend < 0.0f)
    {
        dividend += divisor;
    }
    while (dividend > divisor)
    {
        dividend -= divisor;
    }
    return dividend;
}

cocos2d::Value getDictValue(const cocos2d::ValueMap& dic, const std::string& element_name, const cocos2d::Value default_value)
{
    if (dic.find(element_name) != dic.end())
    {
        return dic.at(element_name);
    }
    return default_value;
}
