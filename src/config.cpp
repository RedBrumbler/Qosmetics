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
    getConfig().config.AddMember("lastActiveSaber", rapidjson::Value(config.lastActiveSaber.c_str(), config.lastActiveSaber.size(), allocator), allocator);
    getConfig().config.AddMember("lastActiveBloq", rapidjson::Value(config.lastActiveBloq.c_str(), config.lastActiveBloq.size(), allocator), allocator);
    getConfig().config.AddMember("lastActiveWall", rapidjson::Value(config.lastActiveWall.c_str(), config.lastActiveWall.size(), allocator), allocator);
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
    if(getConfig().config.HasMember("lastActiveSaber") && getConfig().config["lastActiveSaber"].IsString()){
        config.lastActiveSaber = std::string(getConfig().config["lastActiveSaber"].GetString());    
    }else{
        foundEverything = false;
    }
    if(getConfig().config.HasMember("lastActiveBloq") && getConfig().config["lastActiveBloq"].IsString()){
        config.lastActiveBloq = std::string(getConfig().config["lastActiveBloq"].GetString());    
    }else{
        foundEverything = false;
    }
    if(getConfig().config.HasMember("lastActiveWall") && getConfig().config["lastActiveWall"].IsString()){
        config.lastActiveWall = std::string(getConfig().config["lastActiveWall"].GetString());    
    }else{
        foundEverything = false;
    }
    if(foundEverything){
        getLogger().info("Loaded Configuration!");
        return true;
    }
    return false;
}