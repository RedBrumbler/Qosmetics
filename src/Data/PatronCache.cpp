#include "Data/PatronCache.hpp"
#include "Utils/WebUtils.hpp"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include <thread>

void GetEnthusiastic(rapidjson::Value& val);
void GetAmazing(rapidjson::Value& val);
void GetLegendary(rapidjson::Value& val);
void GetPaypal(rapidjson::Value& val);

extern Logger& getLogger();
namespace Qosmetics
{
    void PatronCache::Download()
    {
        WebUtils::GetAsync("https://raw.githubusercontent.com/RedBrumbler/Qosmetics/master/ExtraFiles/Patrons.json", [&](long returnCode, std::string result){
            if (returnCode != 200) return;
            rapidjson::Document d;
            d.Parse(result.c_str());

            GetEnthusiastic(d["enthusiastic"]);
            GetAmazing(d["amazing"]);
            GetLegendary(d["legendary"]);
            GetPaypal(d["paypal"]);
        });
    }

    void PatronCache::GetEnthusiastic(rapidjson::Value& val)
    {
        assert(val.IsArray());
        for (auto& name : val.GetArray())
        {
            enthusiastic.push_back(name.GetString());
        }
    }
    
    void PatronCache::GetAmazing(rapidjson::Value& val)
    {
        assert(val.IsArray());
        for (auto& name : val.GetArray())
        {
            amazing.push_back(name.GetString());
        }
    }
    
    void PatronCache::GetLegendary(rapidjson::Value& val)
    {
        assert(val.IsArray());
        for (auto& name : val.GetArray())
        {
            legendary.push_back(name.GetString());
        }
    }
    
    void PatronCache::GetPaypal(rapidjson::Value& val)
    {
        assert(val.IsArray());
        for (auto& name : val.GetArray())
        {
            paypal.push_back(name.GetString());
        }
    }
}

