#include "Data/PatreonStorage.hpp"
#include "Utils/FileDownloader.hpp"
#include <thread>

void GetEnthusiastic(rapidjson::Value& val);
void GetAmazing(rapidjson::Value& val);
void GetLegendary(rapidjson::Value& val);
void GetPaypal(rapidjson::Value& val);
    
namespace Qosmetics
{
    void PatreonStorage::Download()
    {
        std::string url = "https://raw.githubusercontent.com/RedBrumbler/Qosmetics/master/ExtraFiles/Patreons.json";
        new FileDownloader(url, "", [&](const FileDownloader& downloader){
            std::thread loadpatreons([](std::string json){
                if (json == "") return;
                rapidjson::Document d;
                d.Parse(json.c_str());
                GetEnthusiastic(d["enthusiastic"]);
                GetAmazing(d["amazing"]);
                GetLegendary(d["legendary"]);
                GetPaypal(d["paypal"]);
                PatreonStorage::setloaded();
            }, downloader.get_result());
            loadpatreons.detach();
        }, true);
    }

    void PatreonStorage::GetEnthusiastic(rapidjson::Value& val)
    {
        assert(val.IsArray());
        for (auto& name : val.GetArray())
        {
            enthusiastic.push_back(name.GetString());
        }
    }
    
    void PatreonStorage::GetAmazing(rapidjson::Value& val)
    {
        assert(val.IsArray());
        for (auto& name : val.GetArray())
        {
            amazing.push_back(name.GetString());
        }
    }
    
    void PatreonStorage::GetLegendary(rapidjson::Value& val)
    {
        assert(val.IsArray());
        for (auto& name : val.GetArray())
        {
            legendary.push_back(name.GetString());
        }
    }
    
    void PatreonStorage::GetPaypal(rapidjson::Value& val)
    {
        assert(val.IsArray());
        for (auto& name : val.GetArray())
        {
            paypal.push_back(name.GetString());
        }
    }
}

