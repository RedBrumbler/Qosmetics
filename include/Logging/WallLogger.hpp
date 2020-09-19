#pragma once
#include "beatsaber-hook/shared/utils/logging.hpp"

namespace Qosmetics
{
    class WallLogger
    {
        public:
            static inline ModInfo modInfo = ModInfo();
            static const Logger& GetLogger()
            {
                static const Logger& log(modInfo);
                return log;
            }
    };
}