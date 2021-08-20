#pragma once
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"

namespace Qosmetics
{
    class ItemConfig
    {
        public:
            ItemConfig(rapidjson::Document& doc);
            ItemConfig() {};
    };
}