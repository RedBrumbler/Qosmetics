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
            
            SaberConfig& get_config()
            {
                return config;
            }

            void ConfigCallback(UnityEngine::TextAsset* textAsset);
            void DescriptorCallback(UnityEngine::TextAsset* textAsset);

            void SetCompleteCallback(std::function<void(SaberItem& item)> input)
            {
                callback = input;
            }

            void RunCompleteCallback()
            {
                callback(*this);
            }
            

        private:
            SaberConfig config = SaberConfig();
            std::function<void(SaberItem& item)> callback = [](SaberItem& item){};
    };
}