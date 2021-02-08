#pragma once
#include <string>
#include <vector>
#include "UnityEngine/Color.hpp"

class TextUtils
{
    public:
        static bool shouldRainbow(std::string name);
        
        static bool shouldRainbow(UnityEngine::Color color);

        static std::string rainbowify(std::string);

        static std::string toLower(std::string in);
    private:
        static inline int rainbowIndex = rand() % 12;
        static inline const std::vector<std::string> colors = {
            "#ff6060",
            "#ffa060",
            "#ffff60",
            "#a0ff60",
            "#60ff60",
            "#60ffa0",
            "#60ffff",
            "#60a0ff",
            "#6060ff",
            "#a060ff",
            "#ff60ff",
            "#ff60a0"
        };
};