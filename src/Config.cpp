#include "Config.hpp"
//#include "Qosmetic/QuestWall.hpp"
//#include "Qosmetic/QuestSaber.hpp"
//#include "Qosmetic/QuestNote.hpp"

static ModInfo modInfo = {ID, VERSION};
extern Logger& getLogger();
config_t config;

Configuration& getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

bool GetScoresDisabled()
{
    bool disabled = false;

    if (config.wallConfig.forceCoreOff) disabled = true;
    if (config.noteConfig.alsoChangeHitboxes) disabled = true;
    //if (Qosmetics::QuestWall::get_scoreDisabled()) disabled = true;

    return disabled;
}


void SaveSaberConfig(rapidjson::Document::AllocatorType& allocator, ConfigDocument& configDoc)
{
    getLogger().info("Saving Saber config");
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

    configDoc.AddMember("saberConfig", saberConfigObject, allocator);
    getLogger().info("Saber config Saved Successfully!");
}

void SaveWallConfig(rapidjson::Document::AllocatorType& allocator, ConfigDocument& configDoc)
{
    getLogger().info("Saving Wall config");
    rapidjson::Value wallConfigObject;
    wallConfigObject.SetObject();
    wallConfig_t& wallConfig = config.wallConfig;
    wallConfigObject.AddMember("forceFakeGlowOff", wallConfig.forceFakeGlowOff, allocator);
    wallConfigObject.AddMember("forceCoreOff", wallConfig.forceCoreOff, allocator);
    wallConfigObject.AddMember("forceFrameOff", wallConfig.forceFrameOff, allocator);

    configDoc.AddMember("wallConfig", wallConfigObject, allocator);
    getLogger().info("Wall config Saved Successfully!");
}

void SaveNoteConfig(rapidjson::Document::AllocatorType& allocator, ConfigDocument& configDoc)
{
    getLogger().info("Saving Note config");
    rapidjson::Value noteConfigObject;
    noteConfigObject.SetObject();
    noteConfig_t& noteConfig = config.noteConfig;
    noteConfigObject.AddMember("overrideNoteSize", noteConfig.overrideNoteSize, allocator);
    noteConfigObject.AddMember("noteSize", noteConfig.noteSize, allocator);
    noteConfigObject.AddMember("alsoChangeHitboxes", noteConfig.alsoChangeHitboxes, allocator);
    noteConfigObject.AddMember("forceDefaultBombs", noteConfig.forceDefaultBombs, allocator);
    noteConfigObject.AddMember("forceDefaultDebris", noteConfig.forceDefaultDebris, allocator);

    configDoc.AddMember("noteConfig", noteConfigObject, allocator);
    getLogger().info("Note config Saved Successfully!");
}

void SaveConfig()
{
    getLogger().info("Saving Configuration...");
    getConfig().config.RemoveAllMembers();
    getConfig().config.SetObject();
    rapidjson::Document::AllocatorType& allocator = getConfig().config.GetAllocator();
    
    SaveSaberConfig(allocator, getConfig().config);
    SaveWallConfig(allocator, getConfig().config);
    SaveNoteConfig(allocator, getConfig().config);

    getConfig().config.AddMember("lastActiveSaber", rapidjson::Value(config.lastActiveSaber.c_str(), config.lastActiveSaber.size(), allocator), allocator);
    getConfig().config.AddMember("lastActiveNote", rapidjson::Value(config.lastActiveNote.c_str(), config.lastActiveNote.size(), allocator), allocator);
    getConfig().config.AddMember("lastActiveWall", rapidjson::Value(config.lastActiveWall.c_str(), config.lastActiveWall.size(), allocator), allocator);
    getConfig().Write();
    getLogger().info("Saved Configuration!");
}

bool LoadSaberConfig(rapidjson::Value& configValue)
{
    getLogger().info("Loading saber config");
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
    if (foundEverything) getLogger().info("Saber config loaded successfully!");
    return foundEverything;
}

bool LoadWallConfig(rapidjson::Value& configValue)
{
    getLogger().info("Loading wall config");
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
    if (foundEverything) getLogger().info("Wall config loaded successfully!");
    return foundEverything;
}

bool LoadNoteConfig(rapidjson::Value& configValue)
{
    getLogger().info("Loading note config");
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
    if (foundEverything) getLogger().info("Note config loaded successfully!");
    return foundEverything;
}

bool LoadConfig()
{
    getLogger().info("Loading Configuration...");
    getConfig().Load();
    bool foundEverything = true;

    if(getConfig().config.HasMember("lastActiveSaber") && getConfig().config["lastActiveSaber"].IsString()){
        config.lastActiveSaber = std::string(getConfig().config["lastActiveSaber"].GetString());    
    }else{
        foundEverything = false;
    }
    if(getConfig().config.HasMember("lastActiveNote") && getConfig().config["lastActiveNote"].IsString()){
        config.lastActiveNote = std::string(getConfig().config["lastActiveNote"].GetString());    
    }else{
        foundEverything = false;
    }
    if(getConfig().config.HasMember("lastActiveWall") && getConfig().config["lastActiveWall"].IsString()){
        config.lastActiveWall = std::string(getConfig().config["lastActiveWall"].GetString());    
    }else{
        foundEverything = false;
    }

    foundEverything = getConfig().config.HasMember("saberConfig") ? LoadSaberConfig(getConfig().config["saberConfig"]) : false;
    foundEverything = getConfig().config.HasMember("wallConfig") ? LoadWallConfig(getConfig().config["wallConfig"]) : false;
    foundEverything = getConfig().config.HasMember("noteConfig") ? LoadNoteConfig(getConfig().config["noteConfig"]) : false;

    if(foundEverything){
        getLogger().info("Loaded Configuration!");
        return true;
    }
    else getLogger().error("Some parts of the configuration could not be loaded, returning false!");
    return false;
}