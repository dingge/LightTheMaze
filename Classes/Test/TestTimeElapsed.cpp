#include "TestTimeElapsed.h"

USING_NS_CC;

void TestTimeElapsed::start(const std::string & func_name)
{
    auto iter = map_.find(func_name);
    if (iter == map_.end())
    {
        map_.insert(std::make_pair(func_name, Data(0.0, 0)));
        begin_time_map_.insert(std::make_pair(func_name, 0.0));
    }
    begin_time_map_.at(func_name) = getTime();
}

void TestTimeElapsed::end(const std::string & func_name)
{
    auto iter = map_.find(func_name);
    if (iter != map_.end())
    {
        iter->second.elapsed_time_ += getTime() - begin_time_map_.at(func_name);
        iter->second.call_count_ += 1;
    }
}

void TestTimeElapsed::dump()
{
    log("dump TestTimeElapsed record");
    for (auto it : map_)
    {
        log("========%s========", it.first.c_str());
        log("elapsed time : %lf", it.second.elapsed_time_);
        log("call count : %d", it.second.call_count_);
        log("once call elapse time : %lf", it.second.elapsed_time_ / it.second.call_count_);
    }
}

double TestTimeElapsed::getTime()
{
    LARGE_INTEGER f, time;
    QueryPerformanceFrequency(&f);                //��ȡӲ��֧�ֵĸ߾��ȼ�������Ƶ�ʣ�����֧���򷵻�0
    QueryPerformanceCounter(&time);               //��ȡ��ǰ�������ļ���
    return time.QuadPart / (double)f.QuadPart;    //����Ƶ�ʣ����ǵ�ǰʱ�䣬��������λ
}
