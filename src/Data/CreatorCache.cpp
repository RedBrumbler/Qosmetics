#include "Data/CreatorCache.hpp"
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/StringBuffer.h"
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/PrettyWriter.h"
#include "Logging/GenericLogger.hpp"
#include "Utils/FileUtils.hpp"
#include "Utils/FileDownloader.hpp"
#include "static-defines.hpp"

#define INFO(value...) GenericLogger::GetLogger().WithContext("Creator cache").info(value);
#define ERROR(value...) GenericLogger::GetLogger().WithContext("Creator cache").error(value);


namespace Qosmetics
{
    void CreatorCache::Download()
    {
        // web request or something to download the json from the repo ig
        INFO("Downloading File");
        std::string path = CREATORCACHE;

        // temporary URL till I get a json file on the Github
        std::string url = "https://pastebin.com/raw/CbdRZa1G";

        dl = new FileDownloader(url, path, [&](const FileDownloader& downloader){
            INFO("Downloader Callback");
            INFO("%s", downloader.get_result().c_str());
            CreatorCache::Load();
        }, true);
    }

    bool CreatorCache::Load()
    {
        INFO("Loading Creator Cache")
        if (!fileexists(CREATORCACHE)) 
        {
            ERROR("File didn't exist!");
            return false;
        }
        std::string json = readfile(CREATORCACHE);
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
        writefile(CREATORCACHE, json);
    }

    UnityEngine::Color CreatorCache::GetCreatorColor(const std::string& creator)
    {
        std::map<std::string, UnityEngine::Color>::iterator it = creatorMap.find(creator);

        if (it != creatorMap.end()) return it->second;
        else return defaultColor;
    }
}