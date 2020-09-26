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
#include "Logging/WallLogger.hpp"

namespace Qosmetics
{
    class QuestWall
    {
        public:
            static ModInfo modInfo;
            static const Logger& getLogger() 
            {
                return WallLogger::GetLogger();
            };

            static inline std::string fileDir = "/sdcard/Qosmetics/walls/";
            static std::vector<std::string> fileNames;
            static std::vector<WallData> loadedWalls;
            
            static inline int selectedWall = 0; 

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
        private:
            static inline bool setColors = false;
            /// @brief makes the folder if not found
            static void makeFolder();
    };
}