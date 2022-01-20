#pragma once
#include "beatsaber-hook/shared/config/rapidjson-utils.hpp"

namespace Qosmetics::Core
{
    class BasicConfig
    {
    public:
        BasicConfig(){};
        BasicConfig(const rapidjson::Value& val){};
    };
}