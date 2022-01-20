#pragma once
#include "ConfigRegister.hpp"

namespace Qosmetics::Core::Config
{
    bool LoadConfig();
    void SaveConfig();
    bool LoadSpecificConfig(std::string_view name);
    void SaveSpecificConfig(std::string_view name);

    void OnProfileSwitched();

    struct Config
    {
        std::string lastUsedConfig = "Default";
    };

    extern Config config;
}