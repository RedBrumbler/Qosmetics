#pragma once
#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp" 
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "bs-utils/shared/utils.hpp"

#include "GlobalNamespace/ColorManager.hpp"
#include "GlobalNamespace/ObstacleController.hpp"

#include "Data/WallData.hpp"
#include "Config/WallConfig.hpp"
#include "Utils/MaterialUtils.hpp"
#include "Utils/WallUtils.hpp"
#include "Utils/FileUtils.hpp"

#include "static-defines.hpp"

#include "Logging/WallLogger.hpp"
#include <map>

namespace Qosmetics
{
    class QuestWall
    {
        public:
            static inline ModInfo modInfo = {string_format("%s Walls", ID), VERSION};
            static Logger& getLogger() 
            {
                return WallLogger::GetLogger();
            };

            static inline std::vector<std::string> fileNames = {};

            /// @brief called at shader warmup scene
            /// @return false if 0 files found, thus making this part of the mod disabled in main
            static bool ShaderWarmup();

            /// @brief called at GameCore scene
            static void GameCore();

            /// @brief called at Health Warning scene
            static void HealthWarning();

            /// @brief called at menu scene
            static void MenuViewControllers();

            /// @brief runs before the obstacle controller Init function
            static void ObstacleController_Init_Pre(GlobalNamespace::ObstacleController* obstacleController);

            /// @brief runs on the obstacle controller set hide hook
            static void ObstacleController_Set_Hide(GlobalNamespace::ObstacleController* obstacleController, bool value);

            static void ClearAllInternalPointers()
            {
                for (auto &pair : wallMap)
                {
                    pair.second->ClearActive();
                }
            }

            /// @brief handles the colors did change event for walls
            static void HandleColorsDidChangeEvent();

            /// @brief Sets the activeWall pointer to point to the wall that should be active, or handles setting to nulltr (default)
            static void SetActiveWall(Descriptor* wallDescriptor, bool ifLoadAlsoAssets = false)
            {
                previousActiveWall = activeWall;
                if (wallDescriptor->get_type() == qosmeticsType::invalid)
                {
                    activeWall = nullptr;
                    OnActiveWallSet(false);
                    return;
                }

                activeWall = wallMap[wallDescriptor];
                OnActiveWallSet(ifLoadAlsoAssets);
            }
            
            /// @brief Sets the activeWall pointer to point to the wall that should be active, or handles setting to nulltr (default)
            static void SetActiveWall(WallData* wall, bool ifLoadAlsoAssets = false)
            {
                previousActiveWall = activeWall;
                activeWall = wall;
                OnActiveWallSet(ifLoadAlsoAssets);
            }
            /// @brief gets called when the active wall is set
            static void OnActiveWallSet(bool ifLoadAlsoAssets);
            
            /// @brief gives the currently active wall if needed
            static WallData* GetActiveWall()
            {
                return activeWall;
            }

            static std::map<Descriptor*, WallData*>& get_wallMap()
            {
                return wallMap;
            }
            
            /// @return true for different, false for same
            static bool DidSelectDifferentWall()
            {
                return activeWall != previousActiveWall;
            }

            static void SelectionDefinitive()
            {
                previousActiveWall = activeWall;
            }

            static bool get_scoreDisabled()
            {
                if (activeWall && activeWall->get_config()->get_scoreSubmissionDisabled()) return true;
                return false;
            }
        private:
            static inline std::map<Descriptor*, WallData*> wallMap = {};
            static inline WallData* activeWall = nullptr;
            static inline WallData* previousActiveWall = nullptr;

            static inline bool setColors = false;
            /// @brief makes the folder if not found
            static void makeFolder();

            static void CheckScoreDisabling();
    };
}