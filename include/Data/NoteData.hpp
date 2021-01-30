#pragma once
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"
#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp" 
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "bs-utils/shared/AssetBundle.hpp"

#include <string>

#include "UnityEngine/TextAsset.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Renderer.hpp"

#include "Config/NoteConfig.hpp"
#include "Data/Descriptor.hpp"
#include "Logging/NoteLogger.hpp"

namespace Qosmetics
{
    class NoteData
    {
        public:
            static Logger& getLogger() 
            {
                return NoteLogger::GetLogger();
            };

            NoteData(){}

            NoteData(std::string filePath)
            {
                this->filePath = filePath;
            }

            NoteData(Descriptor* descriptor)
            {
                this->noteDescriptor = descriptor;
                this->filePath = descriptor->get_filePath();
            }

            NoteData(std::string filePath, bool alsoLoadBundle)
            {
                this->filePath = filePath;
                if (alsoLoadBundle) LoadBundle();
            }

            Qosmetics::Descriptor* get_descriptor()
            {
                return noteDescriptor;
            }

            Qosmetics::NoteConfig* get_config()
            {
                return noteConfig;
            }

            std::string get_filePath()
            {
                return filePath;
            }

            UnityEngine::GameObject* get_notePrefab()
            {
                return notePrefab;
            }

            bool get_finishedNoteLoad()
            {
                return finishedNoteLoad;
            }

            /// @brief loads asset bundle using bs utils, only use if filePath is already given
            void LoadBundle(bool alsoLoadAssets = false);

            /// @brief loads asset bundle using bs utils
            /// @param filePath path to load the bundle from
            void LoadBundle(std::string filePath)
            {
                this->filePath = filePath;
                LoadBundle();
            }

            void UnloadBundle();

            UnityEngine::GameObject* get_leftArrow()
            {
                if (leftArrow != nullptr) return leftArrow;

                UnityEngine::Transform* leftArrowTransform = this->notePrefab->get_transform()->Find(il2cpp_utils::createcsstr("LeftArrow"));
                if (leftArrowTransform == nullptr)
                {
                    getLogger().error("Left arrow was not found");
                    return nullptr;
                }
                this->leftArrow = leftArrowTransform->get_gameObject();
                
                return leftArrow;
            }

            UnityEngine::GameObject* get_rightArrow()
            {
                if (rightArrow != nullptr) return rightArrow;

                UnityEngine::Transform* rightArrowTransform = this->notePrefab->get_transform()->Find(il2cpp_utils::createcsstr("RightArrow"));
                if (rightArrowTransform == nullptr)
                {
                    getLogger().error("Right arrow was not found");
                    return nullptr;
                }
                
                this->rightArrow = rightArrowTransform->get_gameObject();
                
                return rightArrow;
            }

            UnityEngine::GameObject* get_leftDot()
            {
                if (leftDot != nullptr) return leftDot;

                UnityEngine::Transform* leftDotTransform = this->notePrefab->get_transform()->Find(il2cpp_utils::createcsstr("LeftDot"));
                if (leftDotTransform == nullptr)
                {
                    getLogger().error("Left Dot was not found");
                    return nullptr;
                }
                
                this->leftDot = leftDotTransform->get_gameObject();
                
                return leftDot;
            }

            UnityEngine::GameObject* get_rightDot()
            {
                if (rightDot != nullptr) return rightDot;

                UnityEngine::Transform* rightDotTransform = this->notePrefab->get_transform()->Find(il2cpp_utils::createcsstr("RightDot"));
                if (rightDotTransform == nullptr)
                {
                    getLogger().error("Right Dot was not found");
                    return nullptr;
                }
                
                this->rightDot = rightDotTransform->get_gameObject();
                
                return rightDot;
            }

            UnityEngine::GameObject* get_leftDebris()
            {
                if (leftDebris != nullptr) return leftDebris;

                UnityEngine::Transform* leftDebrisTransform = this->notePrefab->get_transform()->Find(il2cpp_utils::createcsstr("LeftDebris"));
                if (leftDebrisTransform == nullptr)
                {
                    getLogger().error("Left Debris was not found");
                    return nullptr;
                }
                
                this->leftDebris = leftDebrisTransform->get_gameObject();
                
                return leftDebris;
            }

            UnityEngine::GameObject* get_rightDebris()
            {
                if (rightDebris != nullptr) return rightDebris;

                UnityEngine::Transform* rightDebrisTransform = this->notePrefab->get_transform()->Find(il2cpp_utils::createcsstr("RightDebris"));
                if (rightDebrisTransform == nullptr)
                {
                    getLogger().error("Right Debris was not found");
                    return nullptr;
                }
                
                this->rightDebris = rightDebrisTransform->get_gameObject();
                
                return rightDebris;
            }

            UnityEngine::GameObject* get_bomb()
            {
                if (bomb != nullptr) return bomb;

                UnityEngine::Transform* bombTransform = this->notePrefab->get_transform()->Find(il2cpp_utils::createcsstr("Bomb"));
                if (bombTransform == nullptr)
                {
                    getLogger().error("Bomb was not found");
                    return nullptr;
                }
                
                this->bomb = bombTransform->get_gameObject();
                
                return bomb;
            }

            bool get_isLoading()
            {
                if (finishedNoteLoad) isLoading = false;
                return isLoading || bundleLoading;
            }

            bool get_replacedMaterials()
            {
                return replacedMaterials;
            }

            void set_replacedMaterials(bool value)
            {
                replacedMaterials = value;
            }

            bool get_replacedBombMaterials()
            {
                return replacedBombMaterials;
            }

            void set_replacedBombMaterials(bool value)
            {
                replacedBombMaterials = value;
            }

            bool get_replacedDebrisMaterials()
            {
                return replacedDebrisMaterials;
            }

            void set_replacedDebrisMaterials(bool value)
            {
                replacedDebrisMaterials = value;
            }

            bool get_complete()
            {
                if (finishedNoteLoad) return true;
        
                finishedNoteLoad = objectComplete && configComplete && descriptorComplete;

                return finishedNoteLoad;
            }

            /// @brief loads all the neccesary assets from the bundle
            void LoadAssets();

            /// @brief sets the references to the different parts of the notes, nullptr are not a problem due to the config that says if those should or should not be valid
            void SetReferences()
            {
                return;
                if (notePrefab == nullptr) 
                {
                    getLogger().error("Tried setting note references while note object was not loaded");
                    return;
                }
                
                UnityEngine::Transform* prefab = notePrefab->get_transform();

                UnityEngine::Transform* leftArrow = prefab->Find(il2cpp_utils::createcsstr("LeftArrow"));
                UnityEngine::Transform* rightArrow = prefab->Find(il2cpp_utils::createcsstr("RightArrow"));
                UnityEngine::Transform* leftDot = prefab->Find(il2cpp_utils::createcsstr("LeftDot"));
                UnityEngine::Transform* rightDot = prefab->Find(il2cpp_utils::createcsstr("RightDot"));
                
                UnityEngine::Transform* leftDebris = prefab->Find(il2cpp_utils::createcsstr("LeftDebris"));
                UnityEngine::Transform* rightDebris = prefab->Find(il2cpp_utils::createcsstr("RightDebris"));
                
                UnityEngine::Transform* bomb = prefab->Find(il2cpp_utils::createcsstr("Bomb"));

                if (leftArrow != nullptr) this->leftArrow = leftArrow->get_gameObject();
                if (rightArrow != nullptr) this->rightArrow = rightArrow->get_gameObject();
                if (leftDot != nullptr) this->leftDot = leftDot->get_gameObject();
                if (rightDot != nullptr) this->rightDot = rightDot->get_gameObject();

                if (leftDebris != nullptr) this->leftDebris = leftDebris->get_gameObject();
                if (rightDebris != nullptr) this->rightDebris = rightDebris->get_gameObject();

                if (bomb != nullptr) this->bomb = bomb->get_gameObject();
            }

            void ClearActive()
            {
                //getLogger().info("Clearing Active bloq pointers");
                leftArrow = nullptr;
                rightArrow = nullptr;
                leftDot = nullptr;
                rightDot = nullptr;

                leftDebris = nullptr;
                rightDebris = nullptr;

                bomb = nullptr;
            }

            void ClearMatVectors()
            {
                leftNoteCCmats.clear();
                rightNoteCCmats.clear();
            }

            std::vector<UnityEngine::Material*>& get_leftNoteCCmaterials();
            std::vector<UnityEngine::Material*>& get_rightNoteCCmaterials();

            void FindPrefab();
            
        private:
            Qosmetics::Descriptor* noteDescriptor = nullptr;
            Qosmetics::NoteConfig* noteConfig = nullptr;

            std::string filePath = "";
            UnityEngine::GameObject* notePrefab = nullptr;

            bool finishedNoteLoad = false;
            bool objectComplete = false;
            bool configComplete = false;
            bool descriptorComplete = false;
            bool isLoading = false;
        	bool replacedMaterials = false;
        	bool replacedBombMaterials = false;
        	bool replacedDebrisMaterials = false;
            bool bundleLoading = false;
            bs_utils::AssetBundle* bundle = nullptr;

            UnityEngine::GameObject* leftArrow = nullptr;
            UnityEngine::GameObject* rightArrow = nullptr;
            UnityEngine::GameObject* leftDot = nullptr;
            UnityEngine::GameObject* rightDot = nullptr;

            UnityEngine::GameObject* leftDebris = nullptr;
            UnityEngine::GameObject* rightDebris = nullptr;

            UnityEngine::GameObject* bomb = nullptr;

            std::vector<UnityEngine::Material*> leftNoteCCmats;
            std::vector<UnityEngine::Material*> rightNoteCCmats;

            void OnNoteLoadComplete(UnityEngine::GameObject* note, bool DontDestroyOnLoad);

            void OnConfigLoadComplete(UnityEngine::TextAsset* configAsset);
            
            void OnDescriptorLoadComplete(UnityEngine::TextAsset* descriptorAsset);

            void OnTextureLoadComplete(UnityEngine::Texture2D* texture);

            void OnComplete();
    };
}