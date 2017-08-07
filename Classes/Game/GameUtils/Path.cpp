#include "Path.h"

USING_NS_CC;

Path::Path()
{
}

Path::Path(const std::vector<cocos2d::Vec2>& right)
{
    point_array_ = right;

    this->calculateLength();
}

Path::Path(const std::vector<Line>& right)
{
    for (size_t index = 0; index < right.size(); ++index)
    {
        point_array_.push_back(right[index]._p1);
    }
    point_array_.push_back(right.back()._p2);

    this->calculateLength();
}

Path::~Path()
{
}

cocos2d::Vec2 Path::getBeginPos() const
{
    if (point_array_.size() > 0)
    {
        return *point_array_.begin();
    }
    return cocos2d::Vec2();
}

cocos2d::Vec2 Path::getEndPos() const
{
    if (point_array_.size() > 0)
    {
        return point_array_.back();
    }
    return cocos2d::Vec2();
}

cocos2d::Vec2 Path::getLinearLerpPos(float rate) const
{
    if (rate <= 0.0)
    {
        return this->getBeginPos();
    }
    if (rate >= 1.0)
    {
        return this->getEndPos();
    }

    float len = this->getPathLength();

    float temp_len_pre = 0;
    float temp_len_cur = 0;
    size_t array_len = point_array_.size();
    for (int index = 0; index < array_len - 1; ++index)
    {
        float cur_len = (point_array_[index] - point_array_[index + 1]).getLength();
        temp_len_cur += cur_len;
        float pre_rate = temp_len_pre / len;
        float cur_rate = temp_len_cur / len;
        if (pre_rate <= rate && cur_rate >= rate)
        {
            return point_array_[index].lerp(point_array_[index + 1], (rate - pre_rate) / (cur_rate - pre_rate));
        }
        temp_len_pre = temp_len_cur;
    }
    return cocos2d::Vec2();
}

float Path::getPathLength() const
{
    return path_length_;
}

void Path::reverse()
{
    float array_len = point_array_.size();
    for (int index = 0; index < array_len/2; ++index)
    {
        Vec2 tmp = std::move(point_array_[index]);
        point_array_[index] = std::move(point_array_[array_len - 1 - index]);
        point_array_[array_len - 1 - index] = std::move(tmp);
    }
}

void Path::calculateLength()
{
    path_length_ = 0;
    for (int index = 0; index < point_array_.size() - 1; ++index)
    {
        path_length_ += (point_array_[index] - point_array_[index + 1]).getLength();
    }
}
