#pragma once
#include "Data/Descriptor.hpp"
#include "Data/ModelProvider.hpp"
#include "Data/ItemConfig.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/TextAsset.hpp"

namespace Qosmetics
{
    class QosmeticItem
    {   
        public:
            QosmeticItem(Descriptor& descriptor) : descriptor(descriptor)
            {
                
            }

            void GameObjectCallback(UnityEngine::GameObject* gameObject);
            virtual void ConfigCallback(UnityEngine::TextAsset* textAsset) {};
            virtual void DescriptorCallback(UnityEngine::GameObject* textAsset){};
            
            ItemType get_type()
            {
                return descriptor.get_type();
            }

            virtual ItemConfig& get_config();

        private:
            Descriptor& descriptor;
            ModelProvider modelProvider;
            UnityEngine::GameObject* prefab;
    };
}