#include "conditional-dependencies/shared/main.hpp"
#include "Data/DescriptorCache.hpp"

using namespace Qosmetics;

EXPOSE_API(GetExists, bool) {
    return true;
}

EXPOSE_API(ReloadDescriptors, void) {
    DescriptorCache::Save();
    DescriptorCache::Load();
}