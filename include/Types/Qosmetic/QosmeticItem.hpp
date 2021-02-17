#pragma once
#include "Data/Descriptor.hpp"
#include "Data/ModelLoader.hpp"
#include "Types/Qosmetic/ItemConfig.hpp"

#include "UnityEngine/Object.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/TextAsset.hpp"
#include <functional>

namespace Qosmetics
{
    class QosmeticItem
    {   
        public:
            QosmeticItem(Descriptor& descriptor, bool load = false) : descriptor(descriptor)
            {
                if (load) Load();
            }

            void Load()
            {
                if (!fileexists(descriptor.get_filePath())) return;
                modelProvider.LoadBundle(true);
            }

            void LoadBundle()
            {
                if (!fileexists(descriptor.get_filePath())) return;
                modelProvider.LoadBundle(false);
            }

            void LoadAssets()
            {
                if (!fileexists(descriptor.get_filePath())) return;
                modelProvider.LoadAssets();
            }

            void GameObjectCallback(UnityEngine::GameObject* gameObject);
            virtual void ConfigCallback(UnityEngine::TextAsset* textAsset) {};
            virtual void DescriptorCallback(UnityEngine::TextAsset* textAsset);
            
            UnityEngine::GameObject* get_prefab()
            {
                return prefab;
            }

            ItemType get_type()
            {
                return descriptor.get_type();
            }

            virtual ItemConfig& get_config()
            {
                return config;
            }

            Descriptor& get_descriptor()
            {
                return descriptor;
            }

            ~QosmeticItem()
            {
                UnityEngine::Object::Destroy(prefab);
            }

            bool get_complete()
            {
                return GOloaded && DescriptorLoaded && ConfigLoaded;
            }

        protected:
            bool GOloaded = false;
            bool DescriptorLoaded = false;
            bool ConfigLoaded = false;
            Descriptor& descriptor;
            ModelLoader modelProvider = ModelLoader(this);
            UnityEngine::GameObject* prefab = nullptr;
            ItemConfig config;
    };
}