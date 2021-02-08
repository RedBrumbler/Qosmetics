#include "Data/CreatorCache.hpp"
#include "Utils/FileDownloader.hpp"
#include <thread>
namespace Qosmetics
{
    void CreatorCache::Download()
    {
        std::string url = "https://raw.githubusercontent.com/RedBrumbler/Qosmetics/master/ExtraFiles/CreatorCache.json";
        std::string path = "";
        new FileDownloader(url, path, [](const FileDownloader& downloader){
            std::string result = downloader.get_result();
            if (result == "") return;

            std::thread cacheThread([](std::string val){
                rapidjson::Document d;
                d.SetObject();
                d.Parse(val.c_str());

                for (rapidjson::Value::ConstMemberIterator i = d.MemberBegin(); i != d.MemberEnd(); ++i)
                {
                    const rapidjson::Value& val = i->value;
                    AddCreator(i->name.GetString(), val);
                }
            }, result);

            cacheThread.detach();
        }, true);
    }
    
    UnityEngine::Color CreatorCache::GetCreatorColor(std::string name)
    {
        std::map<std::string, UnityEngine::Color>::iterator it = colorMap.find(name);

        if (it != colorMap.end())
        {
            return it->second;
        }
        return defaultColor;
    }

    void CreatorCache::AddCreator(std::string name, const rapidjson::Value& val)
    {
        float R = val["R"].GetFloat();
        float G = val["G"].GetFloat();
        float B = val["B"].GetFloat();
        colorMap[name] = UnityEngine::Color(R, G, B, 1.0f);
    }
}