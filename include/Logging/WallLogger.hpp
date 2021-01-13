#pragma once
#include "beatsaber-hook/shared/utils/logging.hpp"

namespace Qosmetics
{
    class WallLogger
    {
        public:
            static inline ModInfo modInfo = ModInfo();
            static Logger& GetLogger()
            {
                static Logger* log = new Logger(modInfo, LoggerOptions(false, true));
                return *log;
            }
    };
}