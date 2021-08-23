#include "Config.hpp"
#include "conditional-dependencies/shared/main.hpp"
#include "Data/DescriptorCache.hpp"
using namespace Qosmetics;

#pragma GCC diagnostic ignored "-Wreturn-type-c-linkage"
#pragma GCC diagnostic push

EXPOSE_API(GetExists, bool) {
    return true;
}

EXPOSE_API(ReloadDescriptors, void) {
    DescriptorCache::Save();
    DescriptorCache::Load();
}

EXPOSE_API(GetConfig, config_t*) {
    return &config;
}

#pragma GCC diagnostic pop