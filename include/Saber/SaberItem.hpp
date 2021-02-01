#pragma once
#include "Data/QosmeticItem.hpp"
#include "Saber/SaberConfig.hpp"

namespace Qosmetics
{
    class SaberItem : public QosmeticItem
    {
        public:
            SaberConfig& get_config()
            {
                return config;
            }

            void ConfigCallback(UnityEngine::TextAsset* textAsset);
            void DescriptorCallback(UnityEngine::TextAsset* textAsset);

        private:
            SaberConfig config;
    };
}