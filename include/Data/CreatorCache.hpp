#pragma once

#include <string>
#include <map>

#include "UnityEngine/Color.hpp"
#include "Utils/FileDownloader.hpp"

namespace Qosmetics
{
    class CreatorCache
    {
        public:
            
            static UnityEngine::Color GetCreatorColor(const std::string& creator);

            static void Download();
            static bool Load();
            static void Save();

        private:
            static inline FileDownloader* dl = nullptr;
            static inline UnityEngine::Color defaultColor = UnityEngine::Color(0.8f, 0.8f, 0.8f, 1.0f);
            static inline std::map<std::string, UnityEngine::Color> creatorMap = {
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