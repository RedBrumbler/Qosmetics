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

#define LOGCSTRING(string) INFO("%s: %s", #string, string)
#define LOGSTRING(string) INFO("%s: %s", #string, string.c_str())
#define LOGINT(int) INFO("%s: %d", #int, int)
#define LOGFLOAT(float) INFO("%s: %f", #float, float)
#define LOGPTR(pointer) INFO("%s: %p", #pointer, pointer)
#define LOGCOLOR(color) INFO("%s: %.2f, %.2f, %.2f, %.2f", #color, color.r, color.g, color.b, color.a)