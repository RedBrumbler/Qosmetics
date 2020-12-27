#pragma once
#include "beatsaber-hook/shared/utils/logging.hpp"

namespace Qosmetics
{
    class UILogger
    {
        public:
            static inline ModInfo modInfo = {string_format("%s UI", ID), VERSION};
            static const Logger& GetLogger()
            {
                static const Logger& log(modInfo);
                return log;
            }
    };
}