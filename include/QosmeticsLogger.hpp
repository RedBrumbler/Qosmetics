#pragma once
#include "beatsaber-hook/shared/utils/logging.hpp"

class QosmeticsLogger
{
    public:
        static LoggerContextObject& GetContextLogger(std::string context);
        static Logger& GetLogger();
        static void Init();
    private:
        static inline Logger* logger = nullptr;
};