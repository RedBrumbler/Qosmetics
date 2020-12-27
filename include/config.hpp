#pragma once
#include "../extern/beatsaber-hook/shared/config/rapidjson-utils.hpp"
#include "../extern/beatsaber-hook/shared/config/config-utils.hpp"
#include "modloader/shared/modloader.hpp"
#include <string>

Configuration& getConfig();
void SaveConfig();
bool LoadConfig();

typedef struct _noteConfig {
    bool overrideNoteSize;
    double noteSize = 1.0f;
    bool forceDefaultBombs = false;
    bool forceDefaultDebris = false;
} noteConfig_t;

typedef enum _TrailType {
    custom,
    baseGame,
    none
} TrailType;

typedef struct _saberConfig {
    double saberWidth = 1.0f;
    bool overrideTrailLength = false;
    double trailLength = 14.0f;
    bool overrideWhiteStep = false;
    double whiteStep = 0.1f;
    bool enableMenuPointer = false;
    TrailType trailType = custom;
} saberConfig_t;

typedef struct _wallConfig {
    bool forceFakeGlowOff = false;
} wallConfig_t;


typedef struct _config {
    saberConfig_t saberConfig;
    noteConfig_t noteConfig;
    wallConfig_t wallConfig;
    std::string lastActiveSaber = "";
    std::string lastActiveNote = "";
    std::string lastActiveWall = "";
} config_t;