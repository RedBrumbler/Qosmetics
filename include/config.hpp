#pragma once
#include <string>
#include "../extern/beatsaber-hook/shared/config/rapidjson-utils.hpp"
#include "../extern/beatsaber-hook/shared/config/config-utils.hpp"
#include "modloader/shared/modloader.hpp"

Configuration& getConfig();
void SaveConfig();
bool LoadConfig();

typedef struct _config {
    bool enableMenuPointer = true;
} config_t;