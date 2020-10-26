#pragma once
#include "modloader/shared/modloader.hpp"
#include <vector>

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Mathf.hpp"
#include "UnityEngine/Vector4.hpp"
#include "UnityEngine/MeshFilter.hpp"

#include "GlobalNamespace/ColorManager.hpp"
#include "GlobalNamespace/NoteCutDirection.hpp"
#include "GlobalNamespace/NoteDebris.hpp"
#include "GlobalNamespace/BeatmapSaveData.hpp"
#include "GlobalNamespace/BeatmapSaveData_NoteType.hpp"
#include "GlobalNamespace/BeatmapSaveData_NoteData.hpp"
#include "GlobalNamespace/NoteData.hpp"
#include "GlobalNamespace/NoteController.hpp"
#include "GlobalNamespace/BombNoteController.hpp"
#include "GlobalNamespace/ConditionalMaterialSwitcher.hpp"
#include "GlobalNamespace/SaberType.hpp"

#include "Utils/MaterialUtils.hpp"
#include "Utils/UnityUtils.hpp"
#include "Data/NoteData.hpp"
#include "Config/NoteConfig.hpp"
#include "Logging/NoteLogger.hpp"


namespace Qosmetics
{
    class NoteUtils
    {
        public:
            static const Logger& getLogger() 
            {
                return NoteLogger::GetLogger();
            };

            /// @brief replaces the note in notecontroller with customNoteData contents
            /// @param noteController the notecontroller on which to replace stuff
            /// @param customNoteData reference to the selected customNoteData
            static void ReplaceNote(GlobalNamespace::NoteController* noteController, Qosmetics::NoteData &customNoteData);

            /// @brief replaces the debris with custom debris
            static void ReplaceDebris(GlobalNamespace::NoteDebris* noteDebris, GlobalNamespace::BeatmapSaveData::NoteType noteType, UnityEngine::Transform* initTransform, UnityEngine::Vector3 cutPoint, UnityEngine::Vector3 cutNormal, Qosmetics::NoteData &customNoteData);

            /// @brief replaces the bomb
            static void ReplaceBomb(GlobalNamespace::BombNoteController* noteController, Qosmetics::NoteData &customNoteData);

            /// @brief sets the given bools based on the given noteData
            static void FindNoteType(GlobalNamespace::NoteData* noteData, bool &isLeft, bool &isDot, bool &isGhost, bool &isBomb)
            {
                int cutDirection = noteData->cutDirection.value;
                int type = noteData->get_colorType().value;
                switch(type)
                {
                    case 0: // noteA
                        isLeft = true;
                        isBomb = false;
                        isGhost = false;
                        break;
                    case 1: // noteB
                        isLeft = false;
                        isBomb = false;
                        isGhost = false;
                        break;
                    case 2: // Ghost
                        isGhost = true;
                        isBomb = false;
                        break;
                    case 3: //Bomb
                        isBomb = true;
                        isGhost = false;
                        break;
                }
                switch (cutDirection)
                {
                    case 0: //Up
                    case 1: //Down
                    case 2: //Left
                    case 3: //Right
                    case 4: //UpLeft
                    case 5: //UpRight
                    case 6: //DownLeft
                    case 7: //DownRight
                    case 9: //None  
                	    isDot = false;
                        break;
                    case 8: //Any
                        isDot = true;
                        break;
                }

            }
            
            /// @brief sets the appropriate color on all renderers in object
            /// @param object the object to look in
            /// @param isLeft true means left color, false means right color
            static void SetColor(UnityEngine::Transform* object, bool isLeft);

            /// @brief makes the list of materials to use for replacement on original notes
            /// @param noteRoot root of the note before anything gets done with it
            static void MakeMaterialList(UnityEngine::Transform* noteRoot);

            /// @brief Adds Bomb materials to materialList
            /// @param noteRoot root of the bomb before anything gets done with it
            static void AddBombMaterials(UnityEngine::Transform* bombRoot);

            /// @brief Adds debris materials to materialList
            /// @param noteRoot root of the debris before anything gets done with it
            static void AddDebrisMaterials(UnityEngine::Transform* debrisRoot);

            static void clearMaterialList()
            {
                materialList.clear();
                listDefined = false;
                bombDefined = false;
                debrisDefined = false;
            }

        private:

            static inline std::vector<UnityEngine::Material*> materialList = std::vector<UnityEngine::Material*>();
            static inline bool listDefined = false;
            static inline bool definingList = false;
            static inline bool bombDefined = false;
            static inline bool definingBomb = false;
            static inline bool debrisDefined = false;
            static inline bool definingDebris = false;
            
            /// @brief will try to find a note of the type specified in the notedata
            /// @param note object to look in
            /// @param noteData the noteData to look for
            /// @return true if found, false if not found
            static bool FindOldNote(UnityEngine::Transform* note, GlobalNamespace::NoteData* noteData);

            /// @brief will try to find a note of the type specified
            /// @param note object to look in
            /// @param type note type to look for
            /// @return true if found, false if not found
            static bool FindOldDebris(UnityEngine::Transform* note, GlobalNamespace::BeatmapSaveData::NoteType type);

            /// @brief will disable all types of notes EXCEPT the given note type
            /// @param note the object to look in
            /// @param noteData the type of note TO LEAVE ON
            static void DisableOldNote(UnityEngine::Transform* note, GlobalNamespace::NoteData* noteData);

            /// @brief disables old debris of the other kind, but if debris is found of this note type it will enable that
            /// @param noteDebrisMesh the transform in which to look
            /// @param noteType the type of the note (L/R)
            static void DisableOldDebris(UnityEngine::Transform* noteDebrisMesh, GlobalNamespace::BeatmapSaveData::NoteType noteType);

            /// @brief disables all base game items in the given object, but will check the config for base game arrows
            /// @param note the object to look in for the base game note objects
            /// @param disableArrows wether or not to also disable arrows
            static void DisableBaseGameNotes(UnityEngine::Transform* noteCube, bool disableArrows);

            /// @brief disables all base game items in the given object, but for the tutorial
            /// @param note the object to look in for the base game note objects
            /// @param disableArrows wether or not to also disable arrows
            static void DisableBaseGameTutorialNotes(UnityEngine::Transform* noteCube, bool disableArrows);

            /// @brief disables all base game debris in this (just the meshrenderer though)
            /// @param debris the transform on which to look
            /// @param customNoteData reference to custom note data
            static void DisableBaseGameDebris(UnityEngine::Transform* noteDebrisMesh);

            /// @brief instantiates a note in hte note transform, from the customNoteData
            /// @param note the transform to use for parenting
            /// @param noteController reference to the original controller used for data
            /// @param customNoteData reference to the custom note data 
            static void AddNote(UnityEngine::Transform* note, GlobalNamespace::NoteController* noteController, Qosmetics::NoteData &customNoteData);

            /// @brief Adds debris to the given transform
            /// @param noteDebrisMesh transform to which is parented
            /// @param noteType used to determine which debris to Add
            /// @param customNoteData reference to the used notedata
            static void AddDebris(UnityEngine::Transform* noteDebrisMesh, GlobalNamespace::BeatmapSaveData::NoteType noteType, Qosmetics::NoteData &customNoteData);

            /// @brief sets the vector data that the debris needs to render as cut
            /// @param base the transform in which to look for renderers to set the vector data on
            static void SetDebrisVectorData(UnityEngine::Transform* base, UnityEngine::Transform* initTransform, UnityEngine::Vector3 cutPoint, UnityEngine::Vector3 cutNormal);

            /// @brief replaces all materials on the custom note with official game materials
            /// @param note root of the note before anything gets done with it
            /// @param customNoteData reference to the used noteData so that stuff may be edited on it and retrieved from it
            static void ReplaceNoteMaterials(UnityEngine::Transform* note, Qosmetics::NoteData &customNoteData);

            /// @brief replaces the bomb materials after trying to add the bomb materials to the mat list
            /// @param bomb root of the note before anything gets done with it
            /// @param customNoteData reference to the used noteData so that stuff may be edited on it and retrieved from it
            static void ReplaceBombMaterials(UnityEngine::Transform* bomb, Qosmetics::NoteData &customNoteData);
            
            /// @brief replaces the debris materials after trying to add the debris materials to the mat list
            /// @param debris root of the note before anything gets done with it
            /// @param customNoteData reference to the used noteData so that stuff may be edited on it and retrieved from it
            static void ReplaceDebrisMaterials(UnityEngine::Transform* debris, Qosmetics::NoteData &customNoteData);

            /// @brief makes string lowercase
            /// @param in string to make lowercase
            /// @return input string but lowercase
            static std::string toLowerCase(std::string in)
            {
                std::string out = "";

                for (char letter : in) out += tolower(letter);

                return out;
            }
    };
}