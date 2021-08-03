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

#pragma GCC diagnostic pop