#include "QosmeticsLogger.hpp"
#include "modloader/shared/modloader.hpp"
#include <map>

extern ModInfo modInfo;

std::map <std::string, LoggerContextObject> contextLoggers = {};

LoggerContextObject& QosmeticsLogger::GetContextLogger(std::string context)
{
    std::map <std::string, LoggerContextObject>::iterator it = contextLoggers.find(context);
    if (it != contextLoggers.end())
    {
        return it->second;
    }
    else 
    {
        auto newlyInserted = contextLoggers.insert({context, GetLogger().WithContext(context)});
        return newlyInserted.first->second;
    }
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
