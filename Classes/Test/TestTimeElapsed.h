/********************************************************************
	date  :	2016/05/15   0:15

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef TESTTIMEELAPSED_H_
#define TESTTIMEELAPSED_H_

#include "Game/GameUtils/Singleton.h"
#include <unordered_map>
#include <string>
#include <iostream>
#include "cocos2d.h"
#include "windows.h"

class TestTimeElapsed :public Singleton<TestTimeElapsed>
{
public:
    TestTimeElapsed(){}
    friend class std::auto_ptr<TestTimeElapsed>;
    friend class Singleton<TestTimeElapsed>;

    void start(const std::string& func_name);

    void end(const std::string& func_name);

    void dump();
protected:
    double getTime();

    struct Data
    {
        double elapsed_time_;
        int call_count_;
        Data(double elapsed_time, int call_count) :
            elapsed_time_(elapsed_time), call_count_(call_count) {}
    };
private:
    std::unordered_map<std::string, Data> map_;
    std::unordered_map<std::string, double> begin_time_map_;
};
#endif // TESTTIMEELAPSED_H_