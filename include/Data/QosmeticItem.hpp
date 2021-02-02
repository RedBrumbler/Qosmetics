#pragma once
#include "Data/Descriptor.hpp"
#include "Data/ModelProvider.hpp"
#include "Data/ItemConfig.hpp"

#include "UnityEngine/Object.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/TextAsset.hpp"

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

        protected:
            Descriptor& descriptor;
            ModelProvider modelProvider = ModelProvider(this);
            UnityEngine::GameObject* prefab;
            ItemConfig config;
    };
}