#pragma once
#include "beatsaber-hook/shared/utils/logging.hpp"

namespace Qosmetics
{
    class NoteLogger
    {
        public:
            static inline ModInfo modInfo = {string_format("%s Notes", ID), VERSION};
            static const Logger& GetLogger()
            {
                static const Logger& log(modInfo);
                return log;
            }
    };
}