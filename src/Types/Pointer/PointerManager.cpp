#include "Types/Pointer/PointerManager.hpp"
#include "Types/Saber/SaberItem.hpp"
#include "Data/DescriptorCache.hpp"

DEFINE_TYPE(Qosmetics::PointerManager);

namespace Qosmetics
{
    void PointerManager::internalSetActiveModel(std::string name)
    {
        // if new set is already the active one, ignore
        if (activeItem->get_descriptor().GetFileName() == name) return;
        Descriptor& newItem = DescriptorCache::GetDescriptor(name);
        // if descriptor doesn't exist for this thing, ignore the setactive
        if (!newItem.isValid()) return;
        delete(activeItem);
        activeItem = new SaberItem(newItem, true);
    }
}