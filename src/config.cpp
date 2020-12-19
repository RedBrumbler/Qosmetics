#include "config.hpp"

static ModInfo modInfo = {ID, VERSION};
extern Logger& getLogger();
config_t config;

Configuration& getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

void SaveConfig()
{
    getLogger().info("Saving Configuration...");
    getConfig().config.RemoveAllMembers();
    getConfig().config.SetObject();
    rapidjson::Document::AllocatorType& allocator = getConfig().config.GetAllocator();
    getConfig().config.AddMember("enableMenuPointer", config.enableMenuPointer, allocator);
    getConfig().Write();
    getLogger().info("Saved Configuration!");
}

bool LoadConfig()
{
    getLogger().info("Loading Configuration...");
    getConfig().Load();
    bool foundEverything = true;
    if(getConfig().config.HasMember("enableMenuPointer") && getConfig().config["enableMenuPointer"].IsBool()){
        config.enableMenuPointer = getConfig().config["enableMenuPointer"].GetBool();    
    }else{
        foundEverything = false;
    }
    if(foundEverything){
        getLogger().info("Loaded Configuration!");
        return true;
    }
    return false;
}