#pragma once
#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp" 
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "bs-utils/shared/utils.hpp"

#include "GlobalNamespace/SimpleColorSO.hpp"
#include "GlobalNamespace/GameplayModifiers.hpp"
#include "GlobalNamespace/BombNoteController.hpp"
#include "GlobalNamespace/PlayerSpecificSettings.hpp"

#include "Data/NoteData.hpp"
#include "Data/Descriptor.hpp"
#include "Config/NoteConfig.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Object.hpp"

#include <vector>

#include "Utils/FileUtils.hpp"
#include "Utils/NoteUtils.hpp"

#include "Logging/NoteLogger.hpp"



namespace Qosmetics
{
    class QuestNote
    {
        public:
            static ModInfo modInfo;
            static const Logger& getLogger() 
            {
                return NoteLogger::GetLogger();
            };

            static inline std::string fileDir = "/sdcard/Qosmetics/notes/";
            static std::vector<std::string> fileNames;
            static std::vector<NoteData> loadedNotes;

            static inline UnityEngine::GameObject* selectedNoteGO = nullptr; 
            static inline int selectedNote = 0; 

            /// @brief called at shader warmup scene
            /// @return false if 0 files found, thus making this part of the mod disabled in main
            static bool ShaderWarmup();

            /// @brief called at GameCore scene
            static void GameCore();

            /// @brief called at Health Warning scene
            static void HealthWarning();

            /// @brief called at menu scene
            static void MenuViewControllers();

            /// @brief disables score submission for scoresaber if ghost notes or disappearing arrows is selected as a modifier since these are not compatible with custom notes
            /// @param modifiers the modifier class used in this level
            static void ModifierScoreDisableCheck(GlobalNamespace::GameplayModifiers* modifiers)
            {
                getLogger().info("Checking game modifiers in order to disable scores if needed...");
                if(modifiers->get_ghostNotes() || modifiers->get_disappearingArrows())
                {
                    getLogger().info("Ghost notes or disappearing arrows active!");
                    bs_utils::Submission::disable(modInfo);
                }
                else
                {
                    getLogger().info("Ghost notes or disappearing arrows not active!");
                    bs_utils::Submission::enable(modInfo);
                }
            }

            /// @brief sets the debris disable bool based on what reduce debris is
            /// @param playerSpecificSettings the currently active player settings
            static void ReducedDebrisDisableCheck(GlobalNamespace::PlayerSpecificSettings* playerSpecificSettings)
            {
                // is reduce debris on or off?
                bool reduceDebris = playerSpecificSettings->get_reduceDebris();
                if (reduceDebris)
                {
                    getLogger().info("reduce debris was active! not spawning any custom debris");
                }
                else
                {
                    getLogger().info("reduce debris was not active, we can spawn custom debris now :)");
                }
                // if reduce debris is on, debris should be disabled
                disableDebris = reduceDebris;
            }

            /// @brief runs after the note controller init function
            static void NoteController_Init_Post(GlobalNamespace::NoteController* noteController);;

            /// @brief runs after the debris init function
            static void NoteDebris_Init_Post(GlobalNamespace::NoteDebris* noteDebris, GlobalNamespace::NoteType noteType, UnityEngine::Transform* initTransform, UnityEngine::Vector3 cutPoint, UnityEngine::Vector3 cutNormal);

            /// @brief runs after the bomb controller function
            static void BombController_Init_Post(GlobalNamespace::BombNoteController* noteController);
        
        private:
            static inline bool setColors = false;
            static inline bool disableDebris = false;

            /// @brief makes the folder if not found
            static void makeFolder();
    };
}