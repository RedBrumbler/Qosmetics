#pragma once
#include "Data/QosmeticItem.hpp"
#include "Saber/SaberConfig.hpp"

namespace Qosmetics
{
    class SaberItem : public QosmeticItem
    {
        public:
            ItemConfig& get_config()
            {
                return config;
            }
            void ConfigCallback(TextAsset* textAsset);
            void DescriptorCallback(UnityEngine::GameObject* textAsset);

        private:
            SaberConfig config;
    };
}