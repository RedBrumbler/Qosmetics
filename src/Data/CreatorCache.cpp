#include "Data/CreatorCache.hpp"
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/StringBuffer.h"
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/PrettyWriter.h"
#include "Logging/GenericLogger.hpp"
#include "Utils/FileUtils.hpp"

#define FILEPATH "sdcard/Qosmetics/CreatorCache.json"
#define INFO(value...) GenericLogger::GetLogger().WithContext("Creator cache").info(value);
#define ERROR(value...) GenericLogger::GetLogger().WithContext("Creator cache").error(value);

namespace Qosmetics
{
    void CreatorCache::Download()
    {
        // web request or something to download the json from the repo ig
        INFO("Downloading File");
        std::string path = FILEPATH;
        FileUtils::DownloadFileToPath("https://cdn.discordapp.com/attachments/698265739227824190/801217201314988042/CreatorCache.json", path, true);
        Load();
    }

    bool CreatorCache::Load()
    {
        INFO("Loading Creator Cache")
        if (!fileexists(FILEPATH)) 
        {
            ERROR("File didn't exist!");
            return false;
        }
        std::string json = readfile(FILEPATH);
        if (json == "null")
        {
            ERROR("json was literally \"null\"");
            return false;
        } 

        rapidjson::Document d;
        d.Parse(json.c_str());
        for (rapidjson::Value::ConstMemberIterator i = d.MemberBegin(); i != d.MemberEnd(); ++i)
        {
            const rapidjson::Value& val = i->value;
            UnityEngine::Color creatorColor = UnityEngine::Color(val["R"].GetFloat(), val["G"].GetFloat(), val["B"].GetFloat(), 1.0f);
            creatorMap[i->name.GetString()] = creatorColor;
            INFO("Found %s", i->name.GetString());
        }
        return true;
    }

    void CreatorCache::Save()
    {
        rapidjson::Document d;
        d.RemoveAllMembers();
        d.IsObject();
        rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
        for (auto& pair : creatorMap)
        {
            rapidjson::Value val;
            val.IsObject();
            val.AddMember("R", pair.second.r, allocator);
            val.AddMember("G", pair.second.g, allocator);
            val.AddMember("B", pair.second.b, allocator);
            
            rapidjson::Value authorname(pair.first.c_str(), pair.first.size(), allocator);
            d.AddMember(authorname, val, allocator);
        }

        // stringify document
        rapidjson::StringBuffer buffer;
        buffer.Clear();

        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
        d.Accept(writer);

        // make string out of buffer data
        std::string json(buffer.GetString(), buffer.GetSize());

        // write to file
        writefile(FILEPATH, json);
    }

    UnityEngine::Color CreatorCache::GetCreatorColor(const std::string& creator)
    {
        std::map<std::string, UnityEngine::Color>::iterator it = creatorMap.find(creator);

        if (it != creatorMap.end()) return it->second;
        else return defaultColor;
    }
}