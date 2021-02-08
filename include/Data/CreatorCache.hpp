#pragma once
#include <string>
#include <map>
#include "UnityEngine/Color.hpp"
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"

namespace Qosmetics
{
    class CreatorCache
    {
        public:
            static void Download();

            static UnityEngine::Color GetCreatorColor(std::string name);
        private:
            static void AddCreator(std::string name, const rapidjson::Value& val);
            static inline UnityEngine::Color defaultColor = UnityEngine::Color(0.8f, 0.8f, 0.8f, 1.0f);
            static inline std::map<std::string, UnityEngine::Color> colorMap = {
                {
                    "RedBrumbler",
                    UnityEngine::Color(1.0f, 1.0f, 1.0f, 1.0f)
                },
                {
                    "Bobbie",
                    UnityEngine::Color(1.0f, 1.0f, 1.0f, 1.0f)
                }
            };
    };
}