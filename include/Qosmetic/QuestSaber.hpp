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
#include "Data/QosmeticsTrail.hpp"
#include "Config/SaberConfig.hpp"
#include "Utils/TrailUtils.hpp"
#include "UnityEngine/Shader.hpp"
#include "GlobalNamespace/Saber.hpp"
#include "GlobalNamespace/SaberTrail.hpp"
#include "GlobalNamespace/SaberTrailRenderer.hpp"
#include "GlobalNamespace/SaberModelController.hpp"

#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/MeshFilter.hpp"
#include "UnityEngine/XR/XRNode.hpp"

#include <vector>
#include <map>
#include "Utils/UnityUtils.hpp"
#include "GlobalNamespace/ConditionalMaterialSwitcher.hpp"
#include <iostream>
#include <thread>
#include "GlobalNamespace/ObstacleSaberSparkleEffect.hpp"
#include "GlobalNamespace/ObstacleSaberSparkleEffectManager.hpp"
#include "UnityEngine/ParticleSystem.hpp"
#include "Logging/SaberLogger.hpp"
#include "Utils/MaterialUtils.hpp"

#include "static-defines.hpp"

namespace Qosmetics
{
    class QuestSaber
    {
        public:
            static inline ModInfo modInfo = {string_format("%s Sabers", ID), VERSION};
            static Logger& getLogger() 
            {
                return SaberLogger::GetLogger();
            };
            
            static inline std::string legacyFileDir = "/sdcard/Android/data/com.beatgames.beatsaber/files/sabers/";
            static inline std::vector<std::string> fileNames = {};
            static inline std::vector<std::string> legacyFileNames = {};
            static inline std::vector<std::string> filePaths = {};

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
            
            /// @brief sets the trail intensity float for the trails.
            static void SetTrailIntensity(float intensity)
            {
                Qosmetics::QosmeticsTrail::set_trailIntensity(intensity);
            }

            static void ClearAllInternalPointers()
            {
                for (auto &pair : saberMap)
                {
                    pair.second->ClearActive();
                }
            }

            /// @brief handles the colors did change event for sabers
            static void HandleColorsDidChangeEvent();

            /// @brief exchanges the menu pointers for the loaded saber, if the saber is even loaded
            static void ReplaceMenuPointers(UnityEngine::Transform* controller, UnityEngine::XR::XRNode node);
            
            /// @brief Sets the activeSaber pointer to point to the saber that should be active, or handles setting to nulltr (default)
            static void SetActiveSaber(Descriptor* saberDescriptor, bool ifLoadAlsoAssets = false)
            {
                previousActiveSaber = activeSaber;
                if (saberDescriptor->get_type() == qosmeticsType::invalid)
                {
                    activeSaber = nullptr;
                    OnActiveSaberSet(false);
                    return;
                }

                activeSaber = saberMap[saberDescriptor];
                OnActiveSaberSet(ifLoadAlsoAssets);
            }
            
            /// @brief Sets the activeSaber pointer to point to the saber that should be active, or handles setting to nulltr (default)
            static void SetActiveSaber(SaberData* saber, bool ifLoadAlsoAssets = false)
            {
                previousActiveSaber = activeSaber;
                activeSaber = saber;
                OnActiveSaberSet(ifLoadAlsoAssets);
            }
            /// @brief gets called when the active saber is set
            static void OnActiveSaberSet(bool ifLoadAlsoAssets);
            
            /// @brief gives the currently active saber if needed
            static SaberData* GetActiveSaber()
            {
                return activeSaber;
            }

            static std::map<Descriptor*, SaberData*>& get_saberMap()
            {
                return saberMap;
            }

            /// @return true for different, false for same
            static bool DidSelectDifferentSaber()
            {
                return activeSaber != previousActiveSaber;
            }

            static void UpdateMenuPointers(UnityEngine::Transform* controller, UnityEngine::XR::XRNode node);
            
            static void SelectionDefinitive()
            {
                previousActiveSaber = activeSaber;
            }

        private:
            static inline std::map<Descriptor*, SaberData*> saberMap = {};

            static inline SaberData* activeSaber = nullptr;
            static inline SaberData* previousActiveSaber = nullptr;

            /// @brief makes the folder if not found
            static void makeFolder(std::string directory);
    };
}