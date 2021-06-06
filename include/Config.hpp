#pragma once
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include <string>

Configuration& getConfig();
void SaveConfig();
bool LoadConfig();
bool GetScoresDisabled();

typedef struct _noteConfig {
    bool overrideNoteSize;
    double noteSize = 1.0f;
    bool alsoChangeHitboxes = false;
    bool forceDefaultBombs = false;
    bool forceDefaultDebris = false;
    bool disableReflections = false;
} noteConfig_t;

enum TrailType {
    custom,
    baseGame,
    none
};

typedef struct _saberConfig {
    double saberWidth = 1.0f;
    bool overrideTrailLength = false;
    double trailLength = 14.0f;
    bool whiteTrail = false;
    bool overrideWhiteStep = false;
    double whiteStep = 0.1f;
    bool overrideTrailWidth = false;
    double trailWidth = 1.0f;
    bool enableMenuPointer = false;
    double menuPointerSize = 0.4f;
    TrailType trailType = TrailType::custom;
} saberConfig_t;

typedef struct _wallConfig {
    bool forceFakeGlowOff = false;
    bool forceCoreOff = false;
    bool forceFrameOff = false;
    bool disableReflections = false;
} wallConfig_t;


typedef struct _config {
    saberConfig_t saberConfig;
    noteConfig_t noteConfig;
    wallConfig_t wallConfig;
    std::string lastActiveSaber = "";
    std::string lastActiveNote = "";
    std::string lastActiveWall = "";
} config_t;

struct MasterConfig {
    config_t config;
    bool saberConfigRedo = false;
    bool wallConfigRedo = false;
    bool noteConfigRedo = false;
    std::string lastUsedConfig = "Default";
};

extern MasterConfig masterConfig;
extern config_t config;

namespace Qosmetics
{
    class Config
    {
        public:
            static bool LoadConfig(std::string name);
            static void SaveConfig();
            static void Init();

            static std::string get_configName(int index);

            static std::vector<std::string>& get_configNames()
            {
                return configs;
            }
        private:
            static void Write(rapidjson::Document& d, std::string path);
            static inline std::vector<std::string> configs = {};
    };
}