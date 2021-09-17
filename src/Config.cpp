#include "Config.hpp"
#include "QosmeticsLogger.hpp"
#include "static-defines.hpp"

#include "Utils/FileUtils.hpp"
#include "Utils/DisablingUtils.hpp"

#include "beatsaber-hook/shared/utils/utils.h"

#define INFO(value...) QosmeticsLogger::GetContextLogger("Config").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Config").error(value)

//#include "Qosmetic/QuestWall.hpp"
//#include "Qosmetic/QuestSaber.hpp"
//#include "Qosmetic/QuestNote.hpp"

static ModInfo modInfo = {ID, VERSION};
config_t config;
MasterConfig masterConfig;

Configuration& getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

bool GetScoresDisabled()
{
    bool disabled = false;

    if (config.noteConfig.alsoChangeHitboxes) disabled = true;
    return disabled;
}

void SaveSaberConfig(rapidjson::Document::AllocatorType& allocator, ConfigDocument& configDoc, config_t& config)
{
    INFO("Saving Saber config");
    rapidjson::Value saberConfigObject;
    saberConfigObject.SetObject();
    saberConfig_t& saberConfig = config.saberConfig;
    saberConfigObject.AddMember("saberWidth", saberConfig.saberWidth, allocator);
    saberConfigObject.AddMember("overrideTrailLength", saberConfig.overrideTrailLength, allocator);
    saberConfigObject.AddMember("trailLength", saberConfig.trailLength, allocator);
    saberConfigObject.AddMember("whiteTrail", saberConfig.whiteTrail, allocator);
    saberConfigObject.AddMember("overrideWhiteStep", saberConfig.overrideWhiteStep, allocator);
    saberConfigObject.AddMember("whiteStep", saberConfig.whiteStep, allocator);
    saberConfigObject.AddMember("overrideTrailWidth", saberConfig.overrideTrailWidth, allocator);
    saberConfigObject.AddMember("trailWidth", saberConfig.trailWidth, allocator);
    saberConfigObject.AddMember("enableMenuPointer", saberConfig.enableMenuPointer, allocator);
    saberConfigObject.AddMember("menuPointerSize", saberConfig.menuPointerSize, allocator);
    saberConfigObject.AddMember("trailType", (int)saberConfig.trailType, allocator);
    saberConfigObject.AddMember("disabled", saberConfig.disabled, allocator);

    configDoc.AddMember("saberConfig", saberConfigObject, allocator);
    INFO("Saber config Saved Successfully!");
}

void SaveWallConfig(rapidjson::Document::AllocatorType& allocator, ConfigDocument& configDoc, config_t& config)
{
    INFO("Saving Wall config");
    rapidjson::Value wallConfigObject;
    wallConfigObject.SetObject();
    wallConfig_t& wallConfig = config.wallConfig;
    wallConfigObject.AddMember("forceFakeGlowOff", wallConfig.forceFakeGlowOff, allocator);
    wallConfigObject.AddMember("forceCoreOff", wallConfig.forceCoreOff, allocator);
    wallConfigObject.AddMember("forceFrameOff", wallConfig.forceFrameOff, allocator);
    wallConfigObject.AddMember("disableReflections", wallConfig.disableReflections, allocator);
    wallConfigObject.AddMember("disabled", wallConfig.disabled, allocator);

    configDoc.AddMember("wallConfig", wallConfigObject, allocator);
    INFO("Wall config Saved Successfully!");
}

void SaveNoteConfig(rapidjson::Document::AllocatorType& allocator, ConfigDocument& configDoc, config_t& config)
{
    INFO("Saving Note config");
    rapidjson::Value noteConfigObject;
    noteConfigObject.SetObject();
    noteConfig_t& noteConfig = config.noteConfig;
    noteConfigObject.AddMember("overrideNoteSize", noteConfig.overrideNoteSize, allocator);
    noteConfigObject.AddMember("noteSize", noteConfig.noteSize, allocator);
    noteConfigObject.AddMember("alsoChangeHitboxes", noteConfig.alsoChangeHitboxes, allocator);
    noteConfigObject.AddMember("forceDefaultBombs", noteConfig.forceDefaultBombs, allocator);
    noteConfigObject.AddMember("forceDefaultDebris", noteConfig.forceDefaultDebris, allocator);
    noteConfigObject.AddMember("disableReflections", noteConfig.disableReflections, allocator);
    noteConfigObject.AddMember("disabled", noteConfig.disabled, allocator);

    configDoc.AddMember("noteConfig", noteConfigObject, allocator);
    INFO("Note config Saved Successfully!");
}

void SaveConfig(rapidjson::Document& d, config_t& config)
{
    rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
    
    SaveSaberConfig(allocator, d, config);
    SaveWallConfig(allocator, d, config);
    SaveNoteConfig(allocator, d, config);

    d.AddMember("lastActiveSaber", rapidjson::Value(config.lastActiveSaber.c_str(), config.lastActiveSaber.size(), allocator), allocator);
    d.AddMember("lastActiveNote", rapidjson::Value(config.lastActiveNote.c_str(), config.lastActiveNote.size(), allocator), allocator);
    d.AddMember("lastActiveWall", rapidjson::Value(config.lastActiveWall.c_str(), config.lastActiveWall.size(), allocator), allocator);
}

void SaveConfig()
{
    INFO("Saving Configuration...");
    
    masterConfig.config = config;
    Qosmetics::Config::SaveConfig();
    
    getConfig().config.RemoveAllMembers();
    getConfig().config.SetObject();
    rapidjson::Document::AllocatorType& allocator = getConfig().config.GetAllocator();

    getConfig().config.AddMember("lastUsedConfig", masterConfig.lastUsedConfig, allocator);

    getConfig().Write();
    
    INFO("Saved Configuration!");
}

bool LoadSaberConfig(rapidjson::Value& configValue, config_t& config)
{
    INFO("Loading saber config");
    bool foundEverything = true;
    if(configValue.HasMember("saberWidth") && configValue["saberWidth"].IsDouble()){
        config.saberConfig.saberWidth = configValue["saberWidth"].GetDouble();    
    }else{
        foundEverything = false;
    } 
    if(configValue.HasMember("overrideTrailLength") && configValue["overrideTrailLength"].IsBool()){
        config.saberConfig.overrideTrailLength = configValue["overrideTrailLength"].GetBool();    
    }else{
        foundEverything = false;
    } 
    if(configValue.HasMember("trailLength") && configValue["trailLength"].IsDouble()){
        config.saberConfig.trailLength = configValue["trailLength"].GetDouble();  
        if (config.saberConfig.trailLength <= 1.0) config.saberConfig.trailLength = 1.0;
    }else{
        foundEverything = false;
    }
    if(configValue.HasMember("whiteTrail") && configValue["whiteTrail"].IsBool()){
        config.saberConfig.whiteTrail = configValue["whiteTrail"].GetBool();    
    }else{
        foundEverything = false;
    }
    if(configValue.HasMember("overrideWhiteStep") && configValue["overrideWhiteStep"].IsBool()){
        config.saberConfig.overrideWhiteStep = configValue["overrideWhiteStep"].GetBool();    
    }else{
        foundEverything = false;
    } 
    if(configValue.HasMember("whiteStep") && configValue["whiteStep"].IsDouble()){
        config.saberConfig.whiteStep = configValue["whiteStep"].GetDouble();    
    }else{
        foundEverything = false;
    } 
    if(configValue.HasMember("overrideTrailWidth") && configValue["overrideTrailWidth"].IsBool()){
        config.saberConfig.overrideTrailWidth = configValue["overrideTrailWidth"].GetBool();    
    }else{
        foundEverything = false;
    } 
    if(configValue.HasMember("trailWidth") && configValue["trailWidth"].IsDouble()){
        config.saberConfig.trailWidth = configValue["trailWidth"].GetDouble();    
    }else{
        foundEverything = false;
    } 
    if(configValue.HasMember("enableMenuPointer") && configValue["enableMenuPointer"].IsBool()){
        config.saberConfig.enableMenuPointer = configValue["enableMenuPointer"].GetBool();    
    }else{
        foundEverything = false;
    }
    if(configValue.HasMember("menuPointerSize") && configValue["menuPointerSize"].IsDouble()){
        config.saberConfig.menuPointerSize = configValue["menuPointerSize"].GetDouble();    
    }else{
        foundEverything = false;
    } 
    if(configValue.HasMember("trailType") && configValue["trailType"].IsInt()){
        config.saberConfig.trailType = (TrailType)configValue["trailType"].GetInt();    
    }else{
        foundEverything = false;
    } 
    if(configValue.HasMember("disabled") && configValue["disabled"].IsBool()){
        config.saberConfig.disabled = configValue["disabled"].GetBool();    
    }else{
        foundEverything = false;
    } 
    if (foundEverything) INFO("Saber config loaded successfully!");
    return foundEverything;
}

bool LoadWallConfig(rapidjson::Value& configValue, config_t& config)
{
    INFO("Loading wall config");
    bool foundEverything = true;
    if(configValue.HasMember("forceFakeGlowOff") && configValue["forceFakeGlowOff"].IsBool()){
        config.wallConfig.forceFakeGlowOff = configValue["forceFakeGlowOff"].GetBool();    
    }else{
        foundEverything = false;
    } 
    if(configValue.HasMember("forceCoreOff") && configValue["forceCoreOff"].IsBool()){
        config.wallConfig.forceCoreOff = configValue["forceCoreOff"].GetBool();    
    }else{
        foundEverything = false;
    } 
    if(configValue.HasMember("forceFrameOff") && configValue["forceFrameOff"].IsBool()){
        config.wallConfig.forceFrameOff = configValue["forceFrameOff"].GetBool();    
    }else{
        foundEverything = false;
    } 
    if(configValue.HasMember("disableReflections") && configValue["disableReflections"].IsBool()){
        config.wallConfig.disableReflections = configValue["disableReflections"].GetBool();    
    }else{
        foundEverything = false;
    } 
    if(configValue.HasMember("disabled") && configValue["disabled"].IsBool()){
        config.wallConfig.disabled = configValue["disabled"].GetBool();    
    }else{
        foundEverything = false;
    } 
    if (foundEverything) INFO("Wall config loaded successfully!");
    return foundEverything;
}

bool LoadNoteConfig(rapidjson::Value& configValue, config_t& config)
{
    INFO("Loading note config");
    bool foundEverything = true;
    if(configValue.HasMember("overrideNoteSize") && configValue["overrideNoteSize"].IsBool()){
        config.noteConfig.overrideNoteSize = configValue["overrideNoteSize"].GetBool();    
    }else{
        foundEverything = false;
    } 
    if(configValue.HasMember("noteSize") && configValue["noteSize"].IsDouble()){
        config.noteConfig.noteSize = configValue["noteSize"].GetDouble();    
    }else{
        foundEverything = false;
    } 
    if(configValue.HasMember("alsoChangeHitboxes") && configValue["alsoChangeHitboxes"].IsBool()){
        config.noteConfig.alsoChangeHitboxes = configValue["alsoChangeHitboxes"].GetBool();    
    }else{
        foundEverything = false;
    } 
    if(configValue.HasMember("forceDefaultBombs") && configValue["forceDefaultBombs"].IsBool()){
        config.noteConfig.forceDefaultBombs = configValue["forceDefaultBombs"].GetBool();    
    }else{
        foundEverything = false;
    } 
    if(configValue.HasMember("forceDefaultDebris") && configValue["forceDefaultDebris"].IsBool()){
        config.noteConfig.forceDefaultDebris = configValue["forceDefaultDebris"].GetBool();    
    }else{
        foundEverything = false;
    } 
    if(configValue.HasMember("disableReflections") && configValue["disableReflections"].IsBool()){
        config.noteConfig.disableReflections = configValue["disableReflections"].GetBool();    
    }else{
        foundEverything = false;
    } 
    if(configValue.HasMember("disabled") && configValue["disabled"].IsBool()){
        config.noteConfig.disabled = configValue["disabled"].GetBool();    
    }else{
        foundEverything = false;
    } 
    if (foundEverything) INFO("Note config loaded successfully!");
    return foundEverything;
}

bool LoadConfig(rapidjson::Document& d, config_t& config);

bool LoadConfig()
{
    INFO("Loading Config...");
    getConfig().Load();
    bool foundEverything = true;
    
    if(getConfig().config.HasMember("lastUsedConfig") && getConfig().config["lastUsedConfig"].IsString()){
        masterConfig.lastUsedConfig = getConfig().config["lastUsedConfig"].GetString();    
    }else{
        foundEverything = false;
    }

    if (!Qosmetics::Config::LoadConfig(masterConfig.lastUsedConfig))
    {
        foundEverything = false;
    } 
    INFO("Config Loaded!");
    return foundEverything;
    //return LoadConfig(getConfig().config, config);
}

bool LoadConfig(rapidjson::Document& d, config_t& config)
{
    INFO("Loading Configuration...");
    bool foundEverything = true;

    if(d.HasMember("lastActiveSaber") && d["lastActiveSaber"].IsString()){
        config.lastActiveSaber = std::string(d["lastActiveSaber"].GetString());    
    }else{
        foundEverything = false;
    }
    if(d.HasMember("lastActiveNote") && d["lastActiveNote"].IsString()){
        config.lastActiveNote = std::string(d["lastActiveNote"].GetString());    
    }else{
        foundEverything = false;
    }
    if(d.HasMember("lastActiveWall") && d["lastActiveWall"].IsString()){
        config.lastActiveWall = std::string(d["lastActiveWall"].GetString());    
    }else{
        foundEverything = false;
    }

    if (!(d.HasMember("saberConfig") && LoadSaberConfig(d["saberConfig"], config))) foundEverything = false;
    if (!(d.HasMember("wallConfig") && LoadWallConfig(d["wallConfig"], config))) foundEverything = false;
    if (!(d.HasMember("noteConfig") && LoadNoteConfig(d["noteConfig"], config))) foundEverything = false;

    if(foundEverything){
        INFO("Loaded Configuration!");
        return true;
    }
    else ERROR("Some parts of the configuration could not be loaded, returning false!");
    return false;
}

namespace Qosmetics
{
    bool Config::LoadConfig(std::string_view name)
    {
        INFO("Loading config for user %s", name.data());
        if (name == "") 
        {
            masterConfig.lastUsedConfig = "Default";
            name = "Default";
            Config::SaveConfig();
        }

        std::string configPath = string_format("%s%s.json", CONFIGPATH.c_str(), name.data());
        INFO("FilePath: %s", configPath.c_str());
        if (!fileexists(configPath)) 
        {
            masterConfig.lastUsedConfig = name;
            Config::SaveConfig();
            return false;
        }

        std::string configString = readfile(configPath);
        rapidjson::Document d;
        d.Parse(configString);
        config_t newConfig;
        bool foundEverything = ::LoadConfig(d, newConfig);

        masterConfig.config = newConfig;
        masterConfig.lastUsedConfig = name;
        config = masterConfig.config;

        INFO("Loaded Config!");

        static ModInfo disablingInfo = {ID, VERSION};
        if (config.noteConfig.disabled) Disabling::RegisterDisablingInfo(disablingInfo, ItemType::note);
        else Disabling::UnregisterDisablingInfo(disablingInfo, ItemType::note);
        if (config.saberConfig.disabled) Disabling::RegisterDisablingInfo(disablingInfo, ItemType::saber);
        else Disabling::UnregisterDisablingInfo(disablingInfo, ItemType::saber);
        if (config.wallConfig.disabled) Disabling::RegisterDisablingInfo(disablingInfo, ItemType::wall);
        else Disabling::UnregisterDisablingInfo(disablingInfo, ItemType::wall);
        
        return foundEverything;
    }

    void Config::SaveConfig()
    {
        INFO("Saving config for user %s", masterConfig.lastUsedConfig.c_str());
        FileUtils::makeFolder(CONFIGPATH);
        std::string configPath = string_format("%s%s.json", CONFIGPATH.c_str(), masterConfig.lastUsedConfig.c_str());

        rapidjson::Document d;
        
        d.RemoveAllMembers();
        d.SetObject();
        
        ::SaveConfig(d, masterConfig.config);

        Write(d, configPath);
        INFO("Finished saving user config");
    }

    void Config::Write(rapidjson::Document& d, std::string path)
    {
        INFO("Writing Config to file");
        // stringify document
        rapidjson::StringBuffer buffer;
        buffer.Clear();

        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
        d.Accept(writer);

        // make string out of buffer data
        std::string json(buffer.GetString(), buffer.GetSize());

        // write to file
        writefile(path, json);
    }

    void Config::Init()
    {
        FileUtils::GetFilesInFolderPath("json", CONFIGPATH, configs);
        for (auto& p : configs)
        {
            p.erase(p.find(".json"));
        }

        if (configs.size() == 0)
        {
            ERROR("After Init, configs.size was still 0");
            configs.push_back("Default");
            SaveConfig();            
        }
    }
}