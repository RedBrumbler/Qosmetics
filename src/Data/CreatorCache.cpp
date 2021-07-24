#include "Data/CreatorCache.hpp"
#include "Utils/WebUtils.hpp"
#include <thread>
namespace Qosmetics
{
    void CreatorCache::Download()
    {
        WebUtils::GetAsync("https://raw.githubusercontent.com/RedBrumbler/Qosmetics/master/ExtraFiles/CreatorCache.json", [&](long returnCode, std::string result){
            if (returnCode != 200) return;
            rapidjson::Document d;
            d.Parse(result.c_str());

                for (rapidjson::Value::ConstMemberIterator i = d.MemberBegin(); i != d.MemberEnd(); ++i)
                {
                    const rapidjson::Value& val = i->value;
                    AddCreator(i->name.GetString(), val);
                }
        });
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