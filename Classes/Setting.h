/********************************************************************
	date  :	2016/01/30   15:35

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

#ifndef SETTING_H
#define SETTING_H

#include "Game/GameUtils/Singleton.h"
#include "Game/Macros.h"
#include "cocos2d.h"
#include "json\rapidjson.h"
#include "json\document.h"
#include "json\writer.h"
#include "json\stringbuffer.h"

/**
 *	全局设置类（单例）
 *  给游戏中有关难度的参数提供一个获取接口
 */
class Setting : public Singleton<Setting>
{
public:
    const std::string SETTING_JSON_FILE = "config.json";
    const std::string CUSTOM_JSON_FILE = "custom.json";

    void readSetting();

	const cocos2d::ValueMap& getLightSpriteInfo()const;

	const cocos2d::ValueMap& getDarkSpriteInfo()const;

	const cocos2d::ValueMap& getFragmentInfo()const;

	const cocos2d::ValueMap& getMapInfo()const;

	const cocos2d::ValueVector& getLevelInfo()const;

    std::string getEnFontName()const;

    std::string getZhFontName()const;

    std::string getInfomationText()const;

    std::string getLevelNameById(int id)const;

    int getNewestLevel()const;
    void setNewestLevel(int level_id);

	int getDifficulty()const;

	//根据难度等级获取暗精灵的最大速度限制
	float getDarkSpriteRateLimit()const;

	//根据难度等级获取光精灵与暗精灵的消融速率
	float getLossVelocity()const;
protected:
	Setting();
	~Setting(){}

	friend class Singleton<Setting>;
	friend class std::auto_ptr<Setting>;

    bool readJson(const std::string& filename, rapidjson::Document* doc);

	void rapidValueToCCValue(const rapidjson::Value& src_value, cocos2d::Value* dest_value);

    void ccValueToRapidValue(const cocos2d::Value& src_value, rapidjson::Value* dest_valueconst, rapidjson::Document& document);

    void writeCustomJsonFile();
private:
	cocos2d::Value setting_root_;
    cocos2d::Value custom_root_;
};
#endif // !SETTING_H
