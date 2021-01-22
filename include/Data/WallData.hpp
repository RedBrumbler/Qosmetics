#pragma once
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"
#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp" 
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "bs-utils/shared/AssetBundle.hpp"

#include "UnityEngine/TextAsset.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/MeshFilter.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/Material.hpp"

#include "Utils/UnityUtils.hpp"

#include "Config/WallConfig.hpp"
#include "Data/Descriptor.hpp"

#include <string>

#include "Logging/WallLogger.hpp"
namespace Qosmetics
{
    class WallData
    {
        public:

            static Logger& getLogger() 
            {
                return WallLogger::GetLogger();
            };
            
            WallData(){}
            
            WallData(std::string filePath)
            {
                this->filePath = filePath;
            }

            WallData(Descriptor* descriptor)
            {
                this->wallDescriptor = descriptor;
                this->filePath = descriptor->get_filePath();
            }

            WallData(std::string filePath, bool alsoLoadBundle)
            {
                this->filePath = filePath;
                if (alsoLoadBundle) LoadBundle();
            }

            /// @brief loads asset bundle using bs utils, only use if filePath is already given
            void LoadBundle(bool alsoLoadAssets = false);

            /// @brief loads all assets from the bundle
            void LoadAssets();

            /// @brief loads asset bundle using bs utils
            /// @param filePath path to load the bundle from
            void LoadBundle(std::string filePath)
            {
                this->filePath = filePath;
                LoadBundle();
            }

            void UnloadBundle();

            bool get_finishedWallLoad()
            {
                return finishedWallLoad;
            }
            /// @brief tries to find the mesh on core
            /// @return mesh pointer, or nullptr if not found
            UnityEngine::Mesh* get_coreMesh();

            /// @brief tries to find the mesh on frame
            /// @return mesh pointer, or nullptr if not found
            UnityEngine::Mesh* get_frameMesh();

            /// @brief tries to find the material on core
            /// @return material pointer, or nullptr if not found
            UnityEngine::Material* get_coreMaterial();

            /// @brief tries to find the material on frame
            /// @return material pointer, or nullptr if not found
            UnityEngine::Material* get_frameMaterial();

            /// @brief tries to find the materials on frame
            /// @return material array pointer, or nullptr if not found
            Array<UnityEngine::Material*>* get_frameSharedMaterials();

            /// @brief tries to find the materials on core
            /// @return material array pointer, or nullptr if not found
            Array<UnityEngine::Material*>* get_coreSharedMaterials();

            /// @brief tries to find the renderer on core
            /// @return meshrenderer pointer, or nullptr if not found
            UnityEngine::MeshRenderer* get_coreRenderer();

            /// @brief tries to find the renderer on frame;
            /// @return meshrenderer pointer, or nullptr if not found
            UnityEngine::MeshRenderer* get_frameRenderer();

            UnityEngine::GameObject* get_wallPrefab()
            {
                return wallPrefab;
            }

            UnityEngine::GameObject* get_core()
            {
                if (wallPrefab == nullptr) return nullptr;

                UnityEngine::Transform* coreT = wallPrefab->get_transform()->Find(il2cpp_utils::createcsstr("Core"));

                if (coreT != nullptr) this->core = coreT->get_gameObject();
                return this->core;
            }

            UnityEngine::GameObject* get_frame()
            {
                if (wallPrefab == nullptr) return nullptr;

                UnityEngine::Transform* frameT = wallPrefab->get_transform()->Find(il2cpp_utils::createcsstr("Frame"));

                if (frameT != nullptr) this->frame = frameT->get_gameObject();
                return this->frame;
            }

            Qosmetics::WallConfig* get_config()
            {
                return wallConfig;
            }

            Qosmetics::Descriptor* get_descriptor()
            {
                return wallDescriptor;
            }

            bool get_isLoading()
            {
                if (finishedWallLoad) isLoading = false;
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

            bool get_complete()
            {
                if (finishedWallLoad) return true;
        
                finishedWallLoad = objectComplete && configComplete && descriptorComplete;

                return finishedWallLoad;
            }


            void SetReferences()
            {
                if (wallPrefab == nullptr) return;
                getLogger().info("Settign references to wall objects...");
                UnityEngine::Transform* prefab = wallPrefab->get_transform();

                UnityEngine::Transform* core = prefab->Find(il2cpp_utils::createcsstr("Core"));
                UnityEngine::Transform* frame = prefab->Find(il2cpp_utils::createcsstr("Frame"));

                if (core != nullptr) this->core = core->get_gameObject();
                if (frame != nullptr) this->frame = frame->get_gameObject();

                getLogger().info("References to wall objects set!");
            }

            void ClearActive()
            {
                //getLogger().info("Clearing active wall pointers");
                activeFrameMat = nullptr;
                activeCoreMat = nullptr;
                activeFrameMesh = nullptr;
                activeCoreMesh = nullptr;
                coreSharedMats = nullptr;
                frameSharedMats = nullptr;
            }

            void FindPrefab();
        
        private:
            Qosmetics::Descriptor* wallDescriptor = nullptr;
            Qosmetics::WallConfig* wallConfig = nullptr;

            std::string filePath = "";
            UnityEngine::GameObject* wallPrefab = nullptr;

            bool finishedWallLoad = false;
            bool objectComplete = false;
            bool configComplete = false;
            bool descriptorComplete = false;
            bool isLoading = false;
            bool bundleLoading = false;
            
            bool replacedMaterials;
            
            bool moreThan1core = false;
            bool moreThan1frame = false;

            bs_utils::AssetBundle* bundle = nullptr;

            UnityEngine::GameObject* core = nullptr;
            UnityEngine::GameObject* frame = nullptr;

            UnityEngine::Mesh* activeCoreMesh = nullptr;
            UnityEngine::Mesh* activeFrameMesh = nullptr;
            
            UnityEngine::Material* activeCoreMat = nullptr;
            UnityEngine::Material* activeFrameMat = nullptr;

            Array<UnityEngine::Material*>* coreSharedMats = nullptr;
            Array<UnityEngine::Material*>* frameSharedMats = nullptr;

            UnityEngine::MeshRenderer* coreRenderer = nullptr;
            UnityEngine::MeshRenderer* frameRenderer = nullptr;
            
            void OnWallLoadComplete(UnityEngine::GameObject* wall, bool DontDestroyOnLoad);

            void OnConfigLoadComplete(UnityEngine::TextAsset* configAsset);
            
            void OnDescriptorLoadComplete(UnityEngine::TextAsset* descriptorAsset);

            void OnTextureLoadComplete(UnityEngine::Texture2D* texture);

            void OnComplete();
    };
}