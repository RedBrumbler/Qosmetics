#pragma once
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"
#include <vector>

namespace Qosmetics
{
    class NoteConfig
    {
        public:

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
            bool disableBaseGameArrows;
            bool hasBomb;
            bool hasDebris;
    };
}