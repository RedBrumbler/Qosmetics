#pragma once

#include "modloader/shared/modloader.hpp"
#include "Data/Descriptor.hpp"

namespace Qosmetics::Disabling
{
    bool get_enabled(Qosmetics::ItemType type);

    void RegisterDisablingInfo(ModInfo info, Qosmetics::ItemType type);
    void UnregisterDisablingInfo(ModInfo info, Qosmetics::ItemType type);
}