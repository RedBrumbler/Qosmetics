#pragma once
#include "UnityEngine/AssetBundle.hpp"
#include "bs-utils/shared/AssetBundle.hpp"

namespace Qosmetics
{
    class QosmeticItem;

    class ModelProvider
    {
        public:
            ModelProvider() {};
            ModelProvider(QosmeticItem* item) 
            {
                this->item = item;
            };

            void LoadBundle(bool alsoLoadAssets = false);
            void LoadAssets();
            
            bool get_complete()
            {
                return objectLoaded && descriptorLoaded && configLoaded;
            }

            ~ModelProvider()
            {
                if (bundle) UnloadBundle();
            }

        private:
            QosmeticItem* item = nullptr;
            bs_utils::AssetBundle* bundle = nullptr;
            void OnComplete();
            void UnloadBundle();

            static inline Il2CppReflectionType* GameObjectType = nullptr;
            static inline Il2CppReflectionType* TextAssetType = nullptr;

            bool objectLoaded = false;
            bool descriptorLoaded = false;
            bool configLoaded = false;
    };
}