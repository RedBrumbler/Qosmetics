#include "Types/Wall/WallManager.hpp"
#include "Types/Wall/WallItem.hpp"
#include "Data/DescriptorCache.hpp"

DEFINE_CLASS(Qosmetics::WallManager);

namespace Qosmetics
{
    void WallManager::internalSetActiveModel(std::string name)
    {
        // if new set is already the active one, ignore
        if (activeItem->get_descriptor().GetFileName() == name) return;
        Descriptor& newItem = DescriptorCache::GetDescriptor(name);
        // if descriptor doesn't exist for this thing, ignore the setactive
        if (!newItem.isValid()) return;
        delete(activeItem);
        activeItem = new WallItem(newItem, true);
    }
}