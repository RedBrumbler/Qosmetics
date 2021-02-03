#pragma once
#include "Types/Qosmetic/ItemConfig.hpp"
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"

namespace Qosmetics
{
    class NoteConfig : public ItemConfig
    {
        public:
            NoteConfig() {};
            NoteConfig(rapidjson::Document &config)
            {
                this->disableBaseGameArrows = config["disableBaseGameArrows"].GetBool();
                this->hasBomb = config["hasBomb"].GetBool();
                this->hasDebris = config["hasDebris"].GetBool();
            }

            bool get_disableBaseGameArrows()
            {
                return disableBaseGameArrows;
            }

            bool get_hasBomb()
            {
                return hasBomb;
            }

            bool get_hasDebris()
            {
                return hasDebris;
            }

        private:
            bool disableBaseGameArrows = false;
            bool hasBomb = false;
            bool hasDebris = false;
    };
}