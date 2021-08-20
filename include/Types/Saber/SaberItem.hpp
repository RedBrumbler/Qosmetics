#pragma once
#include "Types/Qosmetic/QosmeticItem.hpp"
#include "Types/Saber/SaberConfig.hpp"
#include "UnityEngine/Transform.hpp"

namespace Qosmetics
{
    class SaberItem : public virtual QosmeticItem
    {
        public:
            SaberItem(Descriptor& descriptor, bool load = false) : QosmeticItem(descriptor, load) {};
            
            SaberConfig& get_config() override
            {
                return config;
            }

            void ConfigCallback(UnityEngine::TextAsset* textAsset) override;
            void DescriptorCallback(UnityEngine::TextAsset* textAsset) override;

        private:
            SaberConfig config = SaberConfig();
    };
}