#pragma once
#include "beatsaber-hook/shared/utils/logging.hpp"

namespace Qosmetics
{
    class SaberLogger
    {
        public:
            static inline ModInfo modInfo = {string_format("%s Sabers", ID), VERSION};
            static const Logger& GetLogger()
            {
                static const Logger& log(modInfo);
                return log;
            }
    };
}