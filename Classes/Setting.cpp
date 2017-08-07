#include "Setting.h"

USING_NS_CC;

Setting::Setting() :
	setting_root_(ValueMap()),
    custom_root_(ValueMap())
{

}

bool Setting::readJson(const std::string& filename, rapidjson::Document* doc)
{
	std::string file_data = FileUtils::getInstance()->getStringFromFile(filename);
	
    doc->Parse(file_data.c_str());
	if (doc->HasParseError())
	{
		CCLOG("Json parse error! %d", doc->GetParseError());
		return false;
	}

	return true;
}

void Setting::readSetting()
{
    FileUtils::getInstance()->addSearchPath(FileUtils::getInstance()->getWritablePath());

    rapidjson::Document settting_document;
    if (readJson(SETTING_JSON_FILE, &settting_document))
    {
        rapidValueToCCValue(settting_document, &setting_root_);
    }

    rapidjson::Document custom_document;
    if (readJson(CUSTOM_JSON_FILE, &custom_document))
    {
        rapidValueToCCValue(custom_document, &custom_root_);
    }
}

const cocos2d::ValueMap& Setting::getLightSpriteInfo() const
{
	return setting_root_.asValueMap().at("light_sprite").asValueMap();
}

const cocos2d::ValueMap& Setting::getDarkSpriteInfo() const
{
	return setting_root_.asValueMap().at("dark_sprite").asValueMap();
}

const cocos2d::ValueMap& Setting::getFragmentInfo() const
{
	return setting_root_.asValueMap().at("fragment").asValueMap();
}

const cocos2d::ValueMap& Setting::getMapInfo() const
{
	return setting_root_.asValueMap().at("map").asValueMap();
}

const cocos2d::ValueVector& Setting::getLevelInfo() const
{
	return setting_root_.asValueMap().at("level").asValueVector();
}

std::string Setting::getEnFontName() const
{
    return setting_root_.asValueMap().at("font_en").asString();
}

std::string Setting::getZhFontName() const
{
    return setting_root_.asValueMap().at("font_zh").asString();
}

std::string Setting::getInfomationText() const
{
    return setting_root_.asValueMap().at("information_text").asString();
}

std::string Setting::getLevelNameById(int id) const
{
    const ValueVector& v = getLevelInfo();
    if (id >= 0 && id < v.size())
    {
        return v[id].asValueMap().at("file_path").asString();
    }
    return "";
}

int Setting::getNewestLevel() const
{
    ValueMap vm = custom_root_.asValueMap();
    if (vm.find("newest_level") != vm.end())
    {
        return vm.at("newest_level").asInt();
    }
    return 0;
}

void Setting::setNewestLevel(int level_id)
{
    const ValueVector& v = getLevelInfo();
    if (level_id >= 0 && level_id < v.size())
    {
        custom_root_.asValueMap()["newest_level"] = level_id;
        writeCustomJsonFile();
    }
}

int Setting::getDifficulty() const
{
	return setting_root_.asValueMap().at("difficulty").asInt();
}

float Setting::getDarkSpriteRateLimit() const
{
	return getDifficulty() * DARK_SPRITE_RATE_LIMIT;
}

float Setting::getLossVelocity() const
{
	return getDifficulty() * 1.0f;
}

void Setting::rapidValueToCCValue(const rapidjson::Value& src_value, Value* dest_value)
{
	if (src_value.IsObject())
	{
		for (auto it = src_value.MemberBegin(); it != src_value.MemberEnd(); ++it)
		{
			std::string menber_name = it->name.GetString();
			ValueMap& dest_value_ref = dest_value->asValueMap();

			if (it->value.IsObject())
			{
				Value sub_value_map = Value(ValueMap());
				rapidValueToCCValue(it->value, &sub_value_map);
				dest_value_ref.insert(std::make_pair(menber_name, sub_value_map));
			}
			else if (it->value.IsArray())
			{
				Value sub_value_vector = Value(ValueVector());
				rapidValueToCCValue(it->value, &sub_value_vector);
				dest_value_ref.insert(std::make_pair(menber_name, sub_value_vector));
			}
			else if (it->value.IsBool())
			{
				dest_value_ref.insert(std::make_pair(menber_name, Value(it->value.GetBool())));
			}
			else if (it->value.IsDouble())
			{
				dest_value_ref.insert(std::make_pair(menber_name, Value(it->value.GetDouble())));
			}
			else if (it->value.IsInt())
			{
				dest_value_ref.insert(std::make_pair(menber_name, Value(it->value.GetInt())));
			}
			else if (it->value.IsString())
			{
				dest_value_ref.insert(std::make_pair(menber_name, Value(it->value.GetString())));
			}
		}
	}
	else if (src_value.IsArray())
	{
		for (size_t i = 0; i < src_value.Size(); ++i)
		{
			Value sub_value_map = Value(ValueMap());
			rapidValueToCCValue(src_value[i], &sub_value_map);
			dest_value->asValueVector().push_back(sub_value_map);
		}
	}
}

void Setting::ccValueToRapidValue(const cocos2d::Value& src_value, rapidjson::Value* dest_value, rapidjson::Document& document)
{
    if (src_value.getType() == Value::Type::MAP)
    {
        for (auto it = src_value.asValueMap().begin(); it != src_value.asValueMap().end(); ++it)
        {
            std::string menber_name = it->first;
            dest_value->SetObject();

            if (it->second.getType() == Value::Type::MAP)
            {
                rapidjson::Value sub_value_map(rapidjson::kObjectType);
                ccValueToRapidValue(it->second, &sub_value_map, document);
                dest_value->AddMember(rapidjson::Value(menber_name.c_str(), document.GetAllocator()), sub_value_map, document.GetAllocator());
            }
            else if (it->second.getType() == Value::Type::VECTOR)
            {
                rapidjson::Value sub_value_array(rapidjson::kArrayType);
                ccValueToRapidValue(it->second, &sub_value_array, document);
                dest_value->AddMember(rapidjson::Value(menber_name.c_str(), document.GetAllocator()), sub_value_array, document.GetAllocator());
            }
            else if (it->second.getType() == Value::Type::BOOLEAN)
            {
                dest_value->AddMember(rapidjson::Value(menber_name.c_str(), document.GetAllocator()), rapidjson::Value(it->second.asBool()), document.GetAllocator());
            }
            else if (it->second.getType() == Value::Type::DOUBLE)
            {
                dest_value->AddMember(rapidjson::Value(menber_name.c_str(), document.GetAllocator()), rapidjson::Value(it->second.asDouble()), document.GetAllocator());
            }
            else if (it->second.getType() == Value::Type::INTEGER)
            {
                dest_value->AddMember(rapidjson::Value(menber_name.c_str(), document.GetAllocator()), rapidjson::Value(it->second.asInt()), document.GetAllocator());
            }
            else if (it->second.getType() == Value::Type::STRING)
            {
                rapidjson::Value string_value(rapidjson::kStringType);
                string_value.SetString(it->second.asString().c_str(), it->second.asString().size(), document.GetAllocator());
                dest_value->AddMember(rapidjson::Value(menber_name.c_str(), document.GetAllocator()), string_value, document.GetAllocator());
            }
        }
    }
    else if (src_value.getType() == Value::Type::VECTOR)
    {
        const ValueVector& src_value_vector = src_value.asValueVector();
        for (size_t i = 0; i < src_value_vector.size(); ++i)
        {
            rapidjson::Value sub_value_map(rapidjson::kObjectType);
            ccValueToRapidValue(src_value_vector[i], &sub_value_map, document);
            dest_value->PushBack(sub_value_map, document.GetAllocator());
        }
    }
}

void Setting::writeCustomJsonFile()
{
    rapidjson::Document document;
    ccValueToRapidValue(custom_root_, &document, document);
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);

    std::string writeable_path = FileUtils::getInstance()->getWritablePath() + CUSTOM_JSON_FILE;
    FileUtils::getInstance()->writeStringToFile(buffer.GetString(), writeable_path);
}
