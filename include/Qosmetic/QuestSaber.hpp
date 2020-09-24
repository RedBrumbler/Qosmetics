#pragma once
#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp" 
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "Utils/FileUtils.hpp"
#include "GlobalNamespace/Saber.hpp"
#include "UnityEngine/GameObject.hpp"
#include "include/Utils/SaberUtils.hpp"
#include "Data/SaberData.hpp"
#include "Config/SaberConfig.hpp"
#include "Utils/TrailUtils.hpp"
#include "UnityEngine/Shader.hpp"
#include "GlobalNamespace/Saber.hpp"
#include "Xft/XWeaponTrail.hpp"
#include "GlobalNamespace/XWeaponTrailRenderer.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/MeshFilter.hpp"
#include <vector>
#include "Utils/UnityUtils.hpp"
#include "GlobalNamespace/ConditionalMaterialSwitcher.hpp"
#include <iostream>
#include <thread>
#include "GlobalNamespace/ObstacleSaberSparkleEffect.hpp"
#include "GlobalNamespace/ObstacleSaberSparkleEffectManager.hpp"
#include "UnityEngine/ParticleSystem.hpp"
#include "Logging/SaberLogger.hpp"
#include "Utils/MaterialUtils.hpp"
namespace Qosmetics
{
    class QuestSaber
    {
        public:
            static ModInfo modInfo; 
            static const Logger& getLogger() 
            {
                return SaberLogger::GetLogger();
            };
            
            static inline std::string fileDir = "/sdcard/Qosmetics/sabers/";
            static inline std::string legacyFileDir = "/sdcard/Android/data/com.beatgames.beatsaber/files/sabers/";
            static std::vector<std::string> fileNames;
            static std::vector<std::string> legacyFileNames;
            static std::vector<std::string> filePaths;

            static std::vector<SaberData> loadedSabers;

            static inline UnityEngine::GameObject* selectedSaberGO = nullptr; 
            static inline int selectedSaber = 0; 

            /// @brief called with the saber start hook
            static void SaberStart(GlobalNamespace::Saber* instance);

            /// @brief called at shader warmup scene
            /// @return false if 0 files found, thus making this part of the mod disabled in main
            static bool ShaderWarmup();

            /// @brief called at GameCore scene
            static void GameCore();

            /// @brief called at Health Warning scene
            static void HealthWarning();

            /// @brief called at menu scene
            static void MenuViewControllers();

        private:
            /// @brief makes the folder if not found
            static void makeFolder(std::string directory);
    };
}