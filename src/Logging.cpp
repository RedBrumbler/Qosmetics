#include "Logging.hpp"
#include "modloader/shared/modloader.hpp"

extern ModInfo modInfo;

LoggerContextObject QosmeticsLogger::GetContextLogger(std::string context)
{
    return GetLogger().WithContext(context);
}

Logger& QosmeticsLogger::GetLogger()
{
    if (!logger) Init();
    return *logger;
}

void QosmeticsLogger::Init()
{
    logger = new Logger(modInfo, LoggerOptions(false, true));
}
