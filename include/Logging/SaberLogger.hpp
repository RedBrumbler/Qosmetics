#pragma once
#include "beatsaber-hook/shared/utils/logging.hpp"

namespace Qosmetics
{
    class SaberLogger
    {
        public:
            static inline ModInfo modInfo = {string_format("%s Sabers", ID), VERSION};
            static Logger& GetLogger()
            {
                static Logger* log = new Logger(modInfo, LoggerOptions(false, true));
                return *log;
            }
    };
}