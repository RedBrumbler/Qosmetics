#pragma once
#include "beatsaber-hook/shared/utils/logging.hpp"

namespace Qosmetics
{
    class GenericLogger
    {
        public:
            static inline ModInfo modInfo = {ID, VERSION};
            static const Logger& GetLogger()
            {
                static const Logger& log(modInfo);
                return log;
            }
    };
}