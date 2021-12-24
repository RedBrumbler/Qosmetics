#pragma once

#include "beatsaber-hook/shared/utils/logging.hpp"
#include <vector>

class Hooks
{
private:
    static inline std::vector<void (*)(Logger&)> installFuncs;

public:
    static void AddFunc(void (*installFunc)(Logger& logger))
    {
        installFuncs.push_back(installFunc);
    }

    static void InstallHooks(Logger& logger)
    {
        for (auto installFunc : installFuncs)
        {
            installFunc(logger);
        }
    }
};

#define QOS_INSTALL_HOOKS(func)   \
    struct __QosRegister##func    \
    {                             \
        __QosRegister##func()     \
        {                         \
            Hooks::AddFunc(func); \
        }                         \
    };                            \
                                  \
    static __QosRegister##func __QosRegisterInstance##func;

#define SIMPLE_INSTALL_HOOK(identifier) INSTALL_HOOK(logger, identifier);