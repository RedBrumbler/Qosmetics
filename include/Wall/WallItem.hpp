#pragma once
#include "Data/QosmeticItem.hpp"
#include "Wall/WallConfig.hpp"

namespace Qosmetics
{
    class WallItem : public virtual QosmeticItem
    {
        public:
            WallItem(Descriptor& descriptor, bool load = false) : QosmeticItem(descriptor, load) {};
            
            WallConfig& get_config()
            {
                return config;
            }

            void ConfigCallback(UnityEngine::TextAsset* textAsset);

        private:
            WallConfig config = WallConfig();
    };
}