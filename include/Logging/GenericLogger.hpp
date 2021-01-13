#pragma once
#include "beatsaber-hook/shared/utils/logging.hpp"

namespace Qosmetics
{
    class GenericLogger
    {
        public:
            static inline ModInfo modInfo = {ID, VERSION};
            static Logger& GetLogger()
            {
                static Logger* log = new Logger(modInfo, LoggerOptions(false, true));
                return *log;
            }
    };
}