#pragma once
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"
#include "UnityEngine/Color.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

#define ValueToColor(val) UnityEngine::Color(val["R"].GetFloat(), val["G"].GetFloat(), val["B"].GetFloat(), val["A"].GetFloat())

namespace Qosmetics
{
    class TrailConfig
    {
        enum trailColorType {
            leftSaber,
            rightSaber,
            custom
        };
        public:
            TrailConfig(const rapidjson::Value& val)
            {
                std::string name = val["name"].GetString();
                this->name = il2cpp_utils::createcsstr(name, il2cpp_utils::StringType::Manual);
                colorType = (trailColorType)val["colorType"].GetInt();
                trailColor = ValueToColor(val["trailColor"]);
                multiplier = ValueToColor(val["multiplierColor"]);
                length = val["length"].GetInt();
                whiteStep = val["whiteStep"].GetInt();
            }

            Il2CppString* get_name()
            {
                return name;
            }

            trailColorType get_colorType()
            {
                return colorType;
            }

            UnityEngine::Color get_trailColor()
            {
                return trailColor;
            }

            UnityEngine::Color get_multiplierColor()
            {
                return multiplier;
            }

            UnityEngine::Color get_color()
            {
                return trailColor * multiplier;
            }

            int get_length()
            {
                return length;
            }

            int get_whiteStep()
            {
                return whiteStep;
            }
            
        private:
            Il2CppString* name = nullptr;
            trailColorType colorType = custom;
            UnityEngine::Color trailColor;
            UnityEngine::Color multiplier;
            int length;
            int whiteStep;
    };
}