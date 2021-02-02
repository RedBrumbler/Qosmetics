#pragma once
#include "Data/QosmeticItem.hpp"
#include "Note/NoteConfig.hpp"

namespace Qosmetics
{
    class NoteItem : public virtual QosmeticItem
    {
        public:
            NoteItem(Descriptor& descriptor, bool load = false) : QosmeticItem(descriptor, load) {};
            
            NoteConfig& get_config()
            {
                return config;
            }

            void ConfigCallback(UnityEngine::TextAsset* textAsset);

        private:
            NoteConfig config = NoteConfig();
    };
}